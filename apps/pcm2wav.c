/*************************************************************************
	> File Name: pcm2wav.c
	> Author: answerywj
	> Mail: yuanweijie1993@gmail.com
	> Created Time: 2021年12月26日 星期日 14时10分36秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "wav.h"

#define BUFFSIZE 3200

int main(int argc, const char *argv[]) {
	
	printf("Version: %s\n", wavfile_version_str());

	FILE *input = NULL;
	FILE *output = NULL;
	int ret = -1;
	unsigned int channels = 0;  
	unsigned int bits = 0;  
	unsigned int samplerate = 0;  

	if (argc < 3) {
        printf("Usage: %s xxx.pcm xxx.wav channels bits samplerate\n", argv[0]);
        goto end;
    }

	if (argv[3]) channels = atoi(argv[3]);
	if (argv[4]) bits = atoi(argv[4]);
	if (argv[5]) samplerate = atoi(argv[5]);
	
	printf("channels: %u, bits: %u, samplerate: %u\n", channels, bits, samplerate);

	input = fopen(argv[1], "rb+");
    if (!input) {
        printf("read input file %s failed!\n", argv[1]);        
        goto end;
    }

	output = wavfile_write_open(argv[2]);
	if (!output) {
        printf("read output file %s failed!\n", argv[2]);
        goto end;
    }

	int len = 0;
	char buffer[BUFFSIZE];
	while (!feof(input)) {
		len = fread(buffer, 1, BUFFSIZE, input);
		if (len > 0) {
			if (len != wavfile_write(output, buffer, len)) {
        		printf("write output file %s failed!\n", argv[2]);
				goto end;
			}
		} else if (len < 0) {
        	printf("read input file %s failed!\n", argv[1]);
			goto end;
		} else {
			break;
		}
	}

	ret = 0;

end:
	if (0 == ret) printf("process successfully!\n");
	else printf("process failed!\n");

	if (input) {
		fclose(input);
		input = NULL;
	}
	
	if (output) {
		wavfile_write_close(output, channels, bits, samplerate);
		output = NULL;
	}

	return ret;
}