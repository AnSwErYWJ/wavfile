/*************************************************************************
	> File Name: wavfile.c
	> Author: answer
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Mon 03 Jun 2019 05:15:34 PM CST
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include "wavfile.h"

static void wavfile_info(waveHeader_t *wavheader) {
    printf("wavheader info:\n");
    printf("riff_tag: %.*s\n", 4, wavheader->riff_tag);
    printf("riff_length: %d\n", wavheader->riff_length);
    printf("format: %.*s\n", 4, wavheader->format);
    printf("fmt_tag: %.*s\n", 4, wavheader->fmt_tag);
    printf("fmt_length: %d\n", wavheader->fmt_length);
    printf("fmt_audio_code: %hd\n", wavheader->fmt_audio_code);
    printf("fmt_channel_num: %hd\n", wavheader->fmt_channel_num);
    printf("fmt_sample_rate: %d\n", wavheader->fmt_sample_rate);
    printf("fmt_byte_rate: %d\n", wavheader->fmt_byte_rate);
    printf("fmt_block_align: %hd\n", wavheader->fmt_block_align);
    printf("fmt_bit_per_sample: %hd\n", wavheader->fmt_bit_per_sample);
    printf("data_tag: %.*s\n", 4, wavheader->data_tag);
    printf("data_length: %d\n", wavheader->data_length);
    printf("\n");
}

FILE *wavfile_read_open(const char *filename, waveHeader_t *header) {
    if (!header) {
        return NULL;
    }

    FILE *fp = NULL;

    fp = fopen(filename, "rb+");
    if (!fp) {
        fprintf(stderr, "read open file failed\n");        
        return NULL;
    }

    size_t size = fread(header, sizeof(waveHeader_t), 1, fp);
    if (size != 1) {
        fprintf(stderr, "read file header failed\n");        
        fclose(fp);
        return NULL;
    }

    wavfile_info(header);

    return fp;
}

int wavfile_read(FILE *fp, const char *data, size_t size) {
    return fread((void *)data, sizeof(char), size, fp);
}

void wavfile_read_close(FILE *fp) {
    if (!fp) {
        fprintf(stderr, "read close file is NULL\n");        
        return;
    }

    fclose(fp);
}

FILE *wavfile_write_open(const char *filename, int channels) {
    waveHeader_t header;

    strncpy(header.riff_tag, "RIFF", 4);
    header.riff_length = 0;
    strncpy(header.format, "WAVE", 4);
    strncpy(header.fmt_tag,  "fmt ", 4);
    header.fmt_length = 16;
    header.fmt_audio_code = 1;
    header.fmt_channel_num = channels;
    header.fmt_sample_rate = WAVFILE_SAMPLE_RATE;
    header.fmt_bit_per_sample = WAVFILE_BIT_PER_SAMPLE;
    header.fmt_block_align = (header.fmt_bit_per_sample / 8) * header.fmt_channel_num;
    header.fmt_byte_rate = header.fmt_sample_rate * header.fmt_channel_num * (header.fmt_bit_per_sample / 8);
    strncpy(header.data_tag, "data", 4);
    header.data_length = 0;

    FILE *fp = fopen(filename, "wb+");
    if (!fp) {
        fprintf(stderr, "write open file failed\n");
        return NULL;
    }

    fwrite(&header, sizeof(header), 1, fp);

    fflush(fp);
    return fp;
}

int wavfile_write(FILE *fp, const char *data, size_t size) {
    return fwrite(data, sizeof(char), size, fp);
}

void wavfile_write_close(FILE *fp) {
    if (!fp) {
        fprintf(stderr, "write close file is NULL\n");        
        return;
    }

    int file_length = ftell(fp);
    int riff_length = file_length - sizeof(int) - (4 * sizeof(char));
    int data_length = file_length - sizeof(waveHeader_t);

    printf("file_length: %d, riff_length: %d, data_length:%d\n", file_length, riff_length, data_length);

    fseek(fp, 4, SEEK_SET);
    fwrite(&riff_length, sizeof(riff_length), 1, fp);

    fseek(fp, sizeof(waveHeader_t) - sizeof(int), SEEK_SET);
    fwrite(&data_length, sizeof(data_length), 1, fp);
    
    fclose(fp);
}