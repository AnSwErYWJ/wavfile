/*************************************************************************
	> File Name: wav2pcm.c
	> Author: answerywj
	> Mail: yuanweijie1993@gmail.com
	> Created Time: 2021年12月26日 星期日 14时10分42秒
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

	if (argc != 3) {
        printf("Usage: %s xxx.wav xxx.pcm\n", argv[0]);
        goto end;
    }

	unsigned int pcmSize = 0;

	input = wavfile_read_open(argv[1], &pcmSize);
	if (!input) {
        printf("read input file %s failed!\n", argv[1]);
        goto end;
    }

	output = fopen(argv[2], "wb+");
    if (!output) {
        printf("read output file %s failed!\n", argv[2]);        
        goto end;
    }

	int len = 0;
	char buffer[BUFFSIZE];
	unsigned int remainSize = pcmSize;
	unsigned int lenPerW = 0;
	while (!feof(input)) {
		lenPerW = (remainSize >= BUFFSIZE) ? BUFFSIZE : remainSize;
		len = wavfile_read(input, buffer, lenPerW);
		if (len > 0) {
			remainSize -= len;
			if (len != fwrite(buffer, 1, len, output)) {
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
		wavfile_read_close(input);
		input = NULL;
	}

	if (output) {
		fclose(output);
		output = NULL;
	}
	
	return ret;
}