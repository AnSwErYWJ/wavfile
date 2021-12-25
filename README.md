# ARCHIVED
**This repo is archived, because this repo will update in https://github.com/AudioKits/exchange.**

# Name
wavfile - Read/Write wav file with header info API

# Compile
```
make clean all
```
# Methods
## wavfile_read_open
`FILE *wavfile_read_open(const char *filename, waveHeader_t *header);`

Open a file to read and return the wav header.

The following options are supported:

* `filename`

	file full path to read.
* `header`

	wav header struct defined in `wavfile.h`, will be filled when then function succeed.

Return value:
* success: file pointer

* failed: `NULL`

## wavfile_read
`int wavfile_read(FILE *fp, const char *data, size_t size);`

Read audio data from file.

The following options are supported:

* `fp`

	file pointer returned by `wavfile_read_open`.
* `data`

	buffer to save by read data.
* `size`

	size to read.

Return value:
* success: the size read 

* failed: `0`

## wavfile_read_close
`void wavfile_read_close(FILE *fp);`

Close th file.

The following options are supported:

* `fp`

	file pointer returned by `wavfile_read_open`.

Return value: `N/A`

## wavfile_write_open
`FILE *wavfile_write_open(const char *filename, int channels);`

Open a file to write and fill wav header to th file.

The following options are supported:

* `filename`

	file full path to write.
* `channels`

	channels of the audio data to write.

Return value:
* success: file pointer

* failed: `NULL`

## wavfile_write
`int wavfile_write(FILE *fp, const char *data, size_t size);`

Write audio data to file.

The following options are supported:

* `fp`

	file pointer returned by `wavfile_write_open`.
* `data`

	data to write.
* `size`

	size to write.

Return value:
* success: the size written 

* failed: `0`

## wavfile_write_close
`void wavfile_write_close(FILE *fp);`

Close th file.

The following options are supported:

* `fp`

	file pointer returned by `wavfile_write_open`.

Return value: `N/A`
