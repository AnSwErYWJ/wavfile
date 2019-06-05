/*************************************************************************
	> File Name: wavfile.h
	> Author: answer
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Mon 03 Jun 2019 05:15:29 PM CST
 ************************************************************************/

#ifndef __WAVFILE__H
#define __WAVFILE__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#if (!(defined WAVFILE_CALL) || !(defined WAVFILE_IMPORT_OR_EXPORT))
	#if defined _WIN32
		#if defined _WIN64
			#define WAVFILE_CALL __stdcall
		#else
			#define WAVFILE_CALL
		#endif
		#ifdef WAVFILE_IMPLEMENTION
			#define WAVFILE_IMPORT_OR_EXPORT __declspec(dllexport)
		#else
			#define WAVFILE_IMPORT_OR_EXPORT __declspec(dllimport)
		#endif
	#elif defined __ANDROID__
		#define WAVFILE_CALL
		#define WAVFILE_IMPORT_OR_EXPORT
		#undef  JNIEXPORT
		#define JNIEXPORT __attribute ((visibility("default")))
	#elif defined __APPLE__
		#define WAVFILE_CALL
		#define WAVFILE_IMPORT_OR_EXPORT
	#elif defined __unix__
		#define WAVFILE_CALL
		#define WAVFILE_IMPORT_OR_EXPORT __attribute ((visibility("default")))
	#else
		#define WAVFILE_CALL
		#define WAVFILE_IMPORT_OR_EXPORT
	#endif
#endif

#define WAVFILE_SAMPLE_RATE     (16000)
#define WAVFILE_BIT_PER_SAMPLE  (16)

typedef struct {
    char    riff_tag[4];
    int     riff_length;
    char    format[4];
    char    fmt_tag[4];
    int     fmt_length;
    short   fmt_audio_code;
    short   fmt_channel_num;
    int     fmt_sample_rate;
    int     fmt_byte_rate;
    short   fmt_block_align;
    short   fmt_bit_per_sample;
    char    data_tag[4];
    int     data_length;
} waveHeader_t;

/* open read file and get the wav header */
WAVFILE_IMPORT_OR_EXPORT FILE *wavfile_read_open(const char *filename, waveHeader_t *header);

/* read audio data from file */
WAVFILE_IMPORT_OR_EXPORT int wavfile_read(FILE *fp, const char *data, size_t size);

/* close read file */
WAVFILE_IMPORT_OR_EXPORT void wavfile_read_close(FILE *fp);

/* open write file with channels */
WAVFILE_IMPORT_OR_EXPORT FILE *wavfile_write_open(const char *filename, int channels);

/* write audio data to file */
WAVFILE_IMPORT_OR_EXPORT int wavfile_write(FILE *fp, const char *data, size_t size);

/* close write file */
WAVFILE_IMPORT_OR_EXPORT void wavfile_write_close(FILE *fp);


#ifdef __cplusplus
}
#endif

#endif
