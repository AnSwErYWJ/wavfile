/*************************************************************************
	> File Name: log.h
	> Author: answerywj
	> Mail: yuanweijie1993@gmail.com
	> Created Time: 2021年12月26日 星期日 13时51分41秒
 ************************************************************************/

#ifndef __LOG__H
#define __LOG__H

#ifdef __cplusplus
extern "C" {
#endif

#include "import/version.h"

#ifdef __unix__
#define Log printf
#define LogV printf
#define LogD printf
#define LogI printf
#define LogW printf
#define LogE printf

#elif defined(__ANDROID__)
#include <android/log.h>
#define Log(f, ...)   __android_log_print(ANDROID_LOG_VERBOSE, TAG, f, ##__VA_ARGS__)
#define LogV(f, ...)  __android_log_print(ANDROID_LOG_VERBOSE, TAG, f, ##__VA_ARGS__)
#define LogD(f, ...)  __android_log_print(ANDROID_LOG_DEBUG, TAG, f, ##__VA_ARGS__)
#define LogI(f, ...)  __android_log_print(ANDROID_LOG_INFO, TAG, f, ##__VA_ARGS__)
#define LogW(f, ...)  __android_log_print(ANDROID_LOG_WARN, TAG, f, ##__VA_ARGS__)
#define LogE(f, ...)  __android_log_print(ANDROID_LOG_ERROR, TAG, f, ##__VA_ARGS__)

#elif defined(__APPLE__)
#define Log printf
#define LogV printf
#define LogD printf
#define LogI printf
#define LogW printf
#define LogE printf

#elif defined(_WIN32)
#define Log printf
#define LogV printf
#define LogD printf
#define LogI printf
#define LogW printf
#define LogE printf

#else
#define Log(...) 
#define LogV(...)
#define LogD(...)
#define LogI(...)
#define LogW(...)
#define LogE(...)

#endif


#ifdef __cplusplus
}
#endif

#endif