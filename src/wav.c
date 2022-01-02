/*************************************************************************
	> File Name: wav.c
	> Author: answerywj
	> Mail: yuanweijie1993@gmail.com
	> Created Time: 2021年12月26日 星期日 15时03分52秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include "import/version.h"
#include "import/log.h"
#include "wav.h"

#define BITS_PER_BYTE       8
#define HEADER_SIZE_MAX     256

typedef struct {
    char  		 id[4];     // "RIFF".
    unsigned int size;      // filesize - 8, exclude riff_id[4] and riff_size.
    char  		 format[4];   // "WAVE".
} wav_riff_t;

typedef struct {
    char       			 id[4];          // "fmt ".
    unsigned   int       size;           // fmt chunk data size, 16 or other.
    unsigned   short     audioFormat;    // 1 for PCM.
    unsigned   short     numChannels;    // 1(mono) or 2(stereo).
    unsigned   int       sampleRate;     // samples per second.
    unsigned   int       byteRate;       // sampleRate * numChannels * bitsPerSample / 8.
    unsigned   short     blockAlign;     // bytes per sample, bitsPerSample * numChannels / 8.
    unsigned   short     bitsPerSample;	 // bits of each sample(8,16,32...).
} wav_fmt_t;

typedef struct {
    char  		 id[4];     // "data".
    unsigned int size;      // filesize - headerSize - tailSize, pcm data size.
} wav_data_t;


/**
 * @description: return version string.
 * @param 
 * @return version string.
 * @Date: 2021-12-28 17:30:57
 */
const char *wavfile_version_str(void) {
    return VERSION_TEXT;
}


/**
 * @description: read wav header from wav file.
 * @param FILE *fp : wav file descriptor to read.
 * @param wav_riff_t *riff : to store wav header riff chunk.
 * @param wav_fmt_t *fmt : to store wav header fmt chunk.
 * @param wav_data_t * data : to store wav header data chunk.
 * @return success - header size; failed - -1.
 * @Date: 2021-12-28 18:56:23
 */
static int _read_header(FILE *fp, wav_riff_t *riff, wav_fmt_t *fmt, wav_data_t * data) {
    if (NULL == fp) {
        LogE("fp is NULL!\n");
        return -1;
    }

    if (NULL == riff || NULL == fmt || NULL == data) {
        LogE("read wav header input is NULL!\n");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    if (fileSize <= 0) {
        LogE("fileSize is invaild %ld!\n", fileSize);
        return -1;
    }
    fseek(fp, 0, SEEK_SET);

    unsigned int chunkInfoSize = 4 * sizeof(char) + sizeof(unsigned int);
    unsigned long usSize = sizeof(unsigned short);
    unsigned int headerSize = 0;

    /* read riff chunk 12 */
    unsigned int riffSize = sizeof(wav_riff_t);
    memset(riff, 0x0, riffSize);

    if (riffSize != fread(riff, 1, riffSize, fp)) {
        LogE("read wav_riff_t failed!\n");
        return -1;
    }
    headerSize += riffSize;

    if (0 != strncmp(riff->id, "RIFF", 4)) {
        LogE("riff->id: %.*s is not RIFF!\n", 4, riff->id);
        return -1;
    }

    if (riff->size != fileSize - chunkInfoSize) {
        LogE("riff->id: %u is not RIFF!\n", riff->size);
        return -1;
    }

    if (0 != strncmp(riff->format, "WAVE", 4)) {
        LogE("riff->format: %.*s is not WAVE!\n", 4, riff->format);
        return -1;
    }

    /* read fmt chunk 24 */
    unsigned int fmtSize = sizeof(wav_fmt_t);
    memset(fmt, 0x0, fmtSize);

    if (fmtSize != fread(fmt, 1, fmtSize, fp)) {
        LogE("read wav_fmt_t failed!\n");
        return -1;
    }
    headerSize += fmtSize;

    if (0 != strncmp(fmt->id, "fmt ", 4)) {
        LogE("fmt->id: %.*s is not fmt !\n", 4, fmt->id);
        return -1;
    }

    /* read fmt chunk ext */
    unsigned int fmtChunkSize = fmtSize - chunkInfoSize;
    unsigned short extSize = 0;

    if (fmtChunkSize == fmt->size) {
    } else if (fmtChunkSize < fmt->size) {
        if (usSize != fread(&extSize, 1, usSize, fp)) {
            LogE("read extSize failed!\n");
            return -1;
        }
        headerSize += usSize;
        if (extSize > 0) {
            fseek(fp, extSize, SEEK_CUR);
            headerSize += extSize;
        }
    } else if (fmtSize > fmt->size) {
        LogE("fmt->size: %u is invaild!\n", fmt->size);
        return -1;
    }

    /* skip fact/List/etc. chunk, read data chunk */
    unsigned int dataSize = sizeof(wav_data_t);

    while (1) {
        memset(data, 0x0, dataSize);

        if (dataSize != fread(data, 1, dataSize, fp)) {
            LogE("read wav_data_t failed!\n");
            return -1;
        }
        headerSize += dataSize;

        /* is data chunk */
        if (0 == strncmp(data->id, "data", 4)) {
            break;
        } else {
            fseek(fp, data->size, SEEK_CUR);
            headerSize += data->size;
        }

        if (headerSize > HEADER_SIZE_MAX) {
            LogE("headerSize %u is out of limit %d !\n", headerSize, HEADER_SIZE_MAX);
            return -1;
        }
    }

    if (data->size + headerSize > fileSize) {
        LogE("%u(data->size) + %u(headerSize) > %ld(fileSize) !\n", data->size, headerSize, fileSize);
        return -1;
    }

    return headerSize;
}

/**
 * @description: add wav header to wav file.
 * @param FILE *fp : wav file descriptor to write.
 * @param int channels : channels of wav file.
 * @param int samplerate : samplerate of wav file.
 * @param int bits : bits of wav file.
 * @param unsigned int size: pcm data size of wav file.
 * @return success - 0; failed - -1.
 * @Date: 2021-12-28 17:27:48
 */
static int _write_header(FILE *fp, int channels, int samplerate, int bits, unsigned int size) {
    if (NULL == fp) {
        LogE("fp is NULL!\n");
        return -1;
    }

    wav_riff_t riff;
    wav_fmt_t fmt;
    wav_data_t data;

    unsigned int headerSize = sizeof(wav_riff_t) + sizeof(wav_fmt_t) + sizeof(wav_data_t);
    unsigned int chunkInfoSize = 4 * sizeof(char) + sizeof(unsigned int);

    /* wav_riff_t */
    memcpy(riff.id, "RIFF", strlen("RIFF"));
    riff.size = headerSize - chunkInfoSize + size;
    memcpy(riff.format, "WAVE", strlen("WAVE"));
    
    fseek(fp, 0, SEEK_SET);
    fwrite(&riff, sizeof(wav_riff_t), 1, fp);

    /* wav_fmt_t */
    memcpy(fmt.id, "fmt ", strlen("fmt "));
    fmt.size = sizeof(wav_fmt_t) - chunkInfoSize;
    fmt.audioFormat = 1;
    fmt.numChannels = channels;
    fmt.sampleRate = samplerate;
    fmt.bitsPerSample = bits;
    fmt.byteRate = fmt.sampleRate * fmt.numChannels * fmt.bitsPerSample / BITS_PER_BYTE;
    fmt.blockAlign = fmt.bitsPerSample * fmt.numChannels / BITS_PER_BYTE;
    
    fseek(fp, sizeof(wav_riff_t), SEEK_SET);
    fwrite(&fmt, sizeof(wav_fmt_t), 1, fp);

    /* wav_data_t */
    memcpy(data.id, "data", strlen("data"));
    data.size = size;
    
    fseek(fp, sizeof(wav_riff_t) + sizeof(wav_fmt_t), SEEK_SET);
    fwrite(&data, sizeof(wav_data_t), 1, fp);
    
    fflush(fp);

    return 0;
}

/**
 * @description: open wav file to read.
 * @param char *filename : wav file path.
 * @param unsigned int *size : pcm data size of wav file.
 * @return wav file descriptor, skip this wav header.
 * @Date: 2021-12-28 18:37:29
 */
FILE *wavfile_read_open(const char *filename, unsigned int *size) {
    if (NULL == filename) {
        LogE("filename to be read is NULL!\n");
        return NULL;
    }
    
    if (NULL == size) {
        LogE("pointer size is NULL!\n");
        return NULL;
    }

    FILE *fp = NULL;

    fp = fopen(filename, "rb+");
    if (NULL == fp) {
        LogE("read open file failed\n");        
        goto end;
    }

    wav_riff_t riff;
    wav_fmt_t fmt;
    wav_data_t data;

    if (_read_header(fp, &riff, &fmt, &data) < 0) {
        LogE("wav read header failed!\n");
        fclose(fp);
        fp = NULL;
        goto end;
    }

    *size = data.size;

end:    
    return fp;
}

/**
 * @description: read data from wav file.
 * @param FILE *fp : wav file descriptor.
 * @param char *data : pcm data to read.
 * @param size_t size : pcm data size to read.
 * @return success - number of bytes read; failed - zero or less than zero.
 * @Date: 2021-12-28 18:37:32
 */
int wavfile_read(FILE *fp, void *data, size_t size) {
    return fread(data, sizeof(char), size, fp);
}

/**
 * @description: close wav file.
 * @param FILE *fp: wav file descriptor.
 * @return success - 0; failed - -1.
 * @Date: 2021-12-28 18:37:34
 */
int wavfile_read_close(FILE *fp) {
    if (NULL == fp) {
        LogE("fp is NULL!\n");
        return -1;
    }

    fclose(fp);

    return 0;
}

/**
 * @description: open wav file to write.
 * @param char *filename : wav file path to write.
 * @return wav file descriptor, skip 44 bytes of header.
 * @Date: 2021-12-28 17:31:17
 */
FILE *wavfile_write_open(const char *filename) {
    if (NULL == filename) {
        LogE("filename to be written is NULL!\n");
        return NULL;
    }

    FILE *fp = fopen(filename, "wb+");
    if (NULL == fp) {
        LogE("write open file failed\n");
        return NULL;
    }

    fseek(fp, sizeof(wav_riff_t) + sizeof(wav_fmt_t) + sizeof(wav_data_t), SEEK_SET);

    return fp;
}

/**
 * @description: write pcm data to wav file.
 * @param FILE *fp : wav file descriptor to write.
 * @param char *data : pcm data to write.
 * @param size_t size : pcm data size to write.
 * @return success - size is written; failed - 0 or less than the size written.
 * @Date: 2021-12-28 17:32:24
 */
int wavfile_write(FILE *fp, const void *data, size_t size) {
    return fwrite(data, 1, size, fp);
}

/**
 * @description: close wav file.
 * @param FILE *fp : wav file descriptor.
 * @param int channels : channels of wav file.
 * @param int bits : bits of wav file.
 * @param int samplerate : samplerate of wav file.
 * @return success - 0; failed - -1.
 * @Date: 2021-12-28 17:35:34
 */
int wavfile_write_close(FILE *fp, int channels, int bits, int samplerate) {
    if (NULL == fp) {
        LogE("fp is NULL!\n");
        return -1;
    }

    int ret = -1;

    fseek(fp, 0, SEEK_END);

    long fileSize = ftell(fp);
    if (fileSize <= 0) {
        LogE("fileSize is invaild %ld!\n", fileSize);
        return -1;
    }
    unsigned int headerSize = sizeof(wav_riff_t) + sizeof(wav_fmt_t) + sizeof(wav_data_t);
    unsigned int dataSize = fileSize - headerSize;

    if (0 != _write_header(fp, channels, samplerate, bits, dataSize)) {
        LogE("wav write header failed!\n");
        goto end;
    }

    ret = 0;
end:
    if (fp) fclose(fp);
    return ret;
}

/**
 * @description: get wav header info in struct wavfile_header_t.
 * @param FILE *fp : wav file descriptor.
 * @param wavfile_header_t *header : struct wavfile_header_t for saving wav header info.
 * @return success - 0; failed - -1.
 * @Date: 2021-12-29 17:34:28
 */
int wavfile_info(FILE *fp, wavfile_header_t *header) {
    if (NULL == fp) {
        LogE("fp is NULL!\n");
        return -1;
    }

    if (NULL == header) {
        LogE("header is NULL!\n");
        return -1;
    }

    fseek(fp, 0, SEEK_SET);

    wav_riff_t riff;
    wav_fmt_t fmt;
    wav_data_t data;

    int headerSize = _read_header(fp, &riff, &fmt, &data);

    if (headerSize < 0) {
        LogE("wav read header failed!\n");
        return -1;
    }

    header->channels = fmt.numChannels;
    header->sampleRate = fmt.sampleRate;
    header->byteRate = fmt.byteRate;
    header->blockSize = fmt.blockAlign;
    header->bitsPerSample = fmt.bitsPerSample;
    header->headerSize = headerSize;
    header->pcmSize = data.size;

    return 0;
}
