<p align="center">
    <a href="#">
        <img src="https://img.shields.io/badge/Author-AnSwErYWJ-blue" alt="Author">
    </a>
    <a href="#">
        <img src="https://img.shields.io/github/license/AnSwErYWJ/wavfile?color=red" alt="license">
    </a>
    <a href="#">
        <img src="https://img.shields.io/github/last-commit/AnSwErYWJ/wavfile?color=orange" alt="last-commit">
    </a>
    <a href="#">
        <img src="https://img.shields.io/github/languages/top/AnSwErYWJ/wavfile?color=ff69b4" alt="languages">
    </a>
    <a href="#">
        <img src="https://img.shields.io/github/repo-size/AnSwErYWJ/wavfile?color=gren" alt="repo-size">
    </a>
</p>

# Name
wavfile - dynamically parse and fill the wav header.

# Description
wavfile provides a set of apis for reading and writing wav header, and also provides three examples to show the application of these apis:

- wav2pcm: convert wav file to pcm file, remove wav header;
- pcm2wav: convert pcm file to wav file, add wav header;
- wavinfo: parse wav header of input;

# Compile
```
$ make clean all
```

# Usage
```
$ ./wav2pcm xxx.wav xxx.pcm
$ ./pcm2wav xxx.pcm xxx.wav channels bits samplerate
$ ./wavinfo xxx.wav
```

# Methods
## wavfile_read_open
### Syntax
`FILE *wavfile_read_open(const char *filename, unsigned int *size);`

### Synopsis
open wav file to read.

### Params
- `filename` : wav file path to read;
- `size`: return pcm data size of wav file;

### Return value:
- `Success` : wav file descriptor, skip this wav header;
- `Failed` : `NULL`;

## wavfile_read
### Syntax
`int wavfile_read(FILE *fp, void *data, size_t size);`

### Synopsis
read pcm data from wav file.

### Params
- `fp` : wav file descriptor to read;
- `data` : pcm data to read;
- `size`: pcm data size to read;

### Return value:
- `Success` : number of bytes read;
- `Failed` : `0` or less than `0`;

## wavfile_read_close
### Syntax
`int wavfile_read_close(FILE *fp);`

### Synopsis
close wav file.

### Params
- `fp` : wav file descriptor;

### Return value:
- `Success` : `0`;
- `Failed` : `-1`;

## wavfile_write_open
### Syntax
`FILE *wavfile_write_open(const char *filename);`

### Synopsis
open wav file to write.

### Params
- `filename` : wav file path to write;

### Return value:
- `Success` : wav file descriptor, skip 44 bytes of header.;
- `Failed` : `NULL`;

## wavfile_write
### Syntax
`int wavfile_write(FILE *fp, void *data, size_t size);`

### Synopsis
write pcm data to wav file.

### Params
- `fp` : wav file descriptor to write;
- `data` : pcm data to write;
- `size`: pcm data size to write;

### Return value:
- `Success` : number of bytes written;
- `Failed` : `0` or less than the size written;

## wavfile_write_close
### Syntax
`int wavfile_write_close(FILE *fp, int channels, int bits, int samplerate);`

### Synopsis
close wav file.

### Params
- `fp` : wav file descriptor;
- `channels` : channels of wav file;
- `bits` : bits of wav file;
- `samplerate` : samplerate of wav file;
 
### Return value:
- `Success` : `0`;
- `Failed` : `-1`;


## wavfile_info
### Syntax
`int wavfile_info(FILE *fp, wavfile_header_t *header);`

### Synopsis
get wav header info in struct `wavfile_header_t`.

### Params
- `fp` : wav file descriptor;
- `header` : struct `wavfile_header_t` for saving wav header info;
 
### Return value:
- `Success` : `0`;
- `Failed` : `-1`;