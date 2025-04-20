#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "queue.h"
#include "netype.h"
#include "buffer.h"

typedef struct {
	int				sock;
	int				port;
	queue_t*	send_que;
	queue_t*	recv_que;
}network_t;

network_t*	net_create();
void				net_free(network_t* self);
void				net_exit(network_t* self);

bool				net_send(ip4_addr src, ip4_addr dst, buffer_t* data);
buffer_t*		net_recv(ip4_addr* src, ip4_addr* dst);

void*				net_receiving(void* arg);
void*				net_sending(void* arg);

#endif //__NETWORK_H__
