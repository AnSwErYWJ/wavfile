/*************************************************************************
	> File Name: wav.h
	> Author: answerywj
	> Mail: yuanweijie1993@gmail.com
	> Created Time: 2021年12月26日 星期日 15时25分11秒
 ************************************************************************/

#ifndef __WAV__H
#define __WAV__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#if (!(defined API_CALL) || !(defined API_IMPORT_OR_EXPORT))
	#if defined _WIN32
		#if defined _WIN64
			#define API_CALL __stdcall
		#else
			#define API_CALL
		#endif
		#define API_IMPORT_OR_EXPORT __declspec(dllexport)
	#elif defined __ANDROID__
		#define API_CALL
		#define API_IMPORT_OR_EXPORT __attribute ((visibility("default")))
	#elif defined __APPLE__
		#define API_CALL
		#define API_IMPORT_OR_EXPORT
	#elif defined __unix__
		#define API_CALL
		#define API_IMPORT_OR_EXPORT __attribute ((visibility("default")))
	#else
		#define API_CALL
		#define API_IMPORT_OR_EXPORT
	#endif
#endif


/* common */
API_IMPORT_OR_EXPORT const char * API_CALL wavfile_version_str(void);

/* wav read */
API_IMPORT_OR_EXPORT FILE * API_CALL wavfile_read_open(const char *filename, unsigned int *size);
API_IMPORT_OR_EXPORT int API_CALL wavfile_read(FILE *fp, void *data, size_t size);
API_IMPORT_OR_EXPORT int API_CALL wavfile_read_close(FILE *fp);

/* wav write */
API_IMPORT_OR_EXPORT FILE * API_CALL wavfile_write_open(const char *filename);
API_IMPORT_OR_EXPORT int API_CALL wavfile_write(FILE *fp, const void *data, size_t size);
API_IMPORT_OR_EXPORT int API_CALL wavfile_write_close(FILE *fp, int channels, int bits, int samplerate);

/* wav info */
typedef struct {
    unsigned   short  channels;
    unsigned   int    sampleRate;
    unsigned   int    byteRate;
    unsigned   short  blockSize;
    unsigned   short  bitsPerSample;
    unsigned   int    headerSize;
    unsigned   int    pcmSize;
} wavfile_header_t;

API_IMPORT_OR_EXPORT int API_CALL wavfile_info(FILE *fp, wavfile_header_t *header);


#ifdef __cplusplus
}
#endif

#endif
