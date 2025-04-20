#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "netype.h"

typedef struct {
	struct {
		ip4_addr addr;
	}local, remote;
	char* secret;
}configure_t;


configure_t* cnf_parse(const char* filename, int* count);

#endif //__CONFIG_H__
