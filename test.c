/*************************************************************************
	> File Name: test.c
	> Author: answer
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Tue 04 Jun 2019 10:48:47 AM CST
 ************************************************************************/

#include <stdio.h>
#include "wavfile.h"

#define READFILE "./test.wav"
#define WRITEFILE "./test_out.wav"
#define CHANNELS 2

int main() {
	FILE *r_fp = NULL;
	FILE *w_fp = NULL;
	waveHeader_t header;

	r_fp = wavfile_read_open(READFILE, &header);
	if (!r_fp) {
		fprintf(stderr, "wavfile_read_open failed\n");
		return -1;
	}

	w_fp = wavfile_write_open(WRITEFILE, CHANNELS);
	if (!w_fp) {
		fprintf(stderr, "wavfile_write_open failed\n");
		return -1;
	}

	int len = 0;
	char data[CHANNELS * 3200];
	while (1) {
		len = wavfile_read(r_fp, data, CHANNELS * 3200);
		if (len <= 0) break;
		len = wavfile_write(w_fp, data, len);
		if (len <= 0) break;
	}

	if (r_fp) {
		wavfile_read_close(r_fp);
	}
	
	if (w_fp) {
		wavfile_write_close(w_fp);
	}

	return 0;
}
