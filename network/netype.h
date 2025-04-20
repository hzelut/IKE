#ifndef __NETYPE_H__
#define __NETYPE_H__

#include <stdint.h>

#include "buffer.h"

typedef uint32_t ip4_addr;

typedef struct {
	ip4_addr	src;
	ip4_addr	dst;
	buffer_t* data;
}packet_t;

packet_t* pkt_create(ip4_addr src, ip4_addr dst, buffer_t* data);
bool			pkt_free(packet_t* self);

ip4_addr	net_stoa(const char* ipstr);
char*			net_atos(ip4_addr addr);

#endif //__NETYPE_H__
