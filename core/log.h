#ifndef __LOG_H__
#define __LOG_H__

#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>

#include "buffer.h"

typedef enum {
	LT_STD,
	LT_SYSLOG,
	LT_FILE
}LOG_TYPE;

typedef enum {
	LL_ALL,
	LL_DBG,
	LL_WARN,
	LL_INFO,
	LL_ERR
}LOG_LEVEL;

typedef struct {
	LOG_TYPE		type;
	LOG_LEVEL		level;
	FILE*				out;

  pthread_mutex_t   mutex;
}log_t;

log_t*	log_create();
void		logging(LOG_LEVEL level, const char* module, const char* fmt, ...);
void		logging_buf(LOG_LEVEL level, const char* module, buffer_t* buf);
void		logging_hex(LOG_LEVEL level, const char* module, void* data, int size);

#endif //__LOG_H__
