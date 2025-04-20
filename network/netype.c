#include <stdlib.h>
#include <stdio.h>

#include "netype.h"

packet_t* pkt_create(ip4_addr src, ip4_addr dst, buffer_t* data) {
	packet_t* self = calloc(1, sizeof(packet_t));
	self->src = src;
	self->dst = dst;
	self->data = data;

	return self;
}

bool pkt_free(packet_t* self) {
	buf_free(self->data);
	free(self);

	return true;
}

ip4_addr net_stoa(const char* ipstr) {
	uint32_t a,b,c,d;

	if(sscanf(ipstr, "%u.%u.%u.%u", &a, &b, &c, &d) == 4) {
		ip4_addr result = a + (b<<8) + (c<<16) + (d<<24);
		return result;
	}

	return 0;
}

char* net_atos(ip4_addr addr) {
	int len = 4*3+4;
	char* ret = calloc(len, sizeof(char));

	snprintf(ret, len, "%u.%u.%u.%u",
			(addr) & 0xFF,
			(addr>>8) & 0xFF,
			(addr>>16) & 0xFF,
			(addr>>24) & 0xFF);

	return ret;
}
