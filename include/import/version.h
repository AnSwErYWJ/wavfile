/*************************************************************************
	> File Name: version.h
	> Author: answerywj
	> Mail: yuanweijie1993@gmail.com
	> Created Time: 2021年12月26日 星期日 13时48分17秒
 ************************************************************************/
#ifndef __VERSION__H
#define __VERSION__H

#ifdef __cplusplus
extern "C" {
#endif

#define TAG 	"WAVFILE"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#if defined _WIN32
#define MAJOR  	   1
#define MINOR      0
#define REVISION   0

#define VERSION_NUM  (((MAJOR)%100)*10000 + ((MINOR)%100)*100 + ((REVISION)%100))
#define VERSION_STRING TOSTRING(MAJOR) "." TOSTRING(MINOR) "." TOSTRING(REVISION)
#define VERSION_TEXT TAG " v" VERSION_STRING " " __DATE__ " " __TIME__

#else
#define VERSION_NUM  (((MAJOR)%100)*10000 + ((MINOR)%100)*100 + ((REVISION)%100))
#define VERSION_STRING TOSTRING(MAJOR) "." TOSTRING(MINOR) "." TOSTRING(REVISION)
#define VERSION_TEXT TAG " v" VERSION_STRING "." CID "(" BUILD_TIME ")"
#endif

#ifdef __cplusplus
}
#endif

#endif