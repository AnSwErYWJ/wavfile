/*************************************************************************
	> File Name: wavinfo.c
	> Author: answerywj
	> Mail: yuanweijie1993@gmail.com
	> Created Time: 2021年12月26日 星期日 14时10分48秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "wav.h"

#define BUFFSIZE 3200

int main(int argc, const char *argv[]) {
	
	printf("Version: %s\n", wavfile_version_str());

	FILE *input = NULL;
	int ret = -1;

	if (argc != 2) {
        printf("Usage: %s xxx.wav\n", argv[0]);
        goto end;
    }

	input = fopen(argv[1], "rb+");
    if (!input) {
        printf("read input file %s failed!\n", argv[1]);        
        goto end;
    }

	wavfile_header_t header;
	if (0 != wavfile_info(input, &header)) {
		printf("%s wavfile info failed!\n", argv[1]);        
        goto end;
	}

	printf("%s header info: \n", argv[1]);
	printf("=============================\n");
    printf("channels: %u\n", header.channels);
    printf("sampleRate: %u\n", header.sampleRate);
    printf("byteRate: %u\n", header.byteRate);
    printf("blockSize: %u\n", header.blockSize);
    printf("bitsPerSample: %u\n", header.bitsPerSample);
    printf("headerSize: %u\n", header.headerSize);
    printf("pcmSize: %u\n", header.pcmSize);
	printf("=============================\n");

	ret = 0;

end:
	if (0 == ret) printf("process successfully!\n");
	else printf("process failed!\n");

	if (input) {
		fclose(input);
		input = NULL;
	}
	
	return ret;
}