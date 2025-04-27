#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

#include "network.h"
#include "daemon.h"

static const char* MM="NET";

void net_exit(network_t* self) {
	que_enque(self->send_que, NULL);
	que_enque(self->recv_que, NULL);
	shutdown(self->sock, SHUT_RD);
}

void net_free(network_t* self) {
	que_free(self->recv_que);
	que_free(self->send_que);
	free(self);
}

network_t* net_create() {
	network_t* self = calloc(1, sizeof(network_t));
	self->recv_que = que_create(true);
	self->send_que = que_create(true);

	self->port = 500;
	self->sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(self->sock < 0) {
		logging(LL_ERR, MM, "Failed create socket");
		return NULL;
	}

	// for get IP_PKTINFO
	int opt = 1;
	setsockopt(self->sock, IPPROTO_IP, IP_PKTINFO, &opt, sizeof(opt));

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(self->port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(self->sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		logging(LL_ERR, MM, "Failed bind socket");
		return NULL;
	}

	logging(LL_INFO, MM, "Created socket");

	push_job(net_sending, self);
	push_job(net_receiving, self);

	return self;
}

bool net_send(ip4_addr src, ip4_addr dst, buffer_t* data) {
	network_t* self = DAEMON.NET;
	packet_t* packet = pkt_create(src, dst, data);
	que_enque(self->send_que, packet);

	return true;
}

buffer_t* net_recv(ip4_addr* src, ip4_addr* dst) {
	network_t* self = DAEMON.NET;
	packet_t* packet = que_deque(self->recv_que);
	if(packet) {
		if(src) *src = packet->src;
		if(dst) *dst = packet->dst;
		return packet->data;
	}

	return NULL;
}

void* net_receiving(void* arg) {
	network_t* self = arg;
	struct sockaddr_in client;
	int recv_len;
	char buf[1024];
	struct msghdr msg;
	struct iovec iov;
	char ancillary[64];
	struct cmsghdr *cm;
	struct in_pktinfo *pktinfo;


	logging(LL_DBG, MM, "Started receiving loop");
	while(DAEMON.is_running) {
		msg.msg_name = &client;
		msg.msg_namelen = sizeof(client);
		iov.iov_base = buf;
		iov.iov_len = sizeof(buf);
		msg.msg_iov = &iov;
		msg.msg_iovlen = 1;
		msg.msg_control = ancillary;
		msg.msg_controllen = sizeof(ancillary);
		msg.msg_flags = 0;

		recv_len = recvmsg(self->sock, &msg, 0);
		if(recv_len > 0) {
			for(cm = CMSG_FIRSTHDR(&msg); cm != NULL;
					cm = CMSG_NXTHDR(&msg, cm)) {
				if(cm->cmsg_level == IPPROTO_IP) {
					pktinfo = (struct in_pktinfo*)CMSG_DATA(cm);
					buffer_t* data = buf_create(recv_len);
					buf_write(data, buf, recv_len);

					packet_t* packet = pkt_create(
							client.sin_addr.s_addr,
							pktinfo->ipi_addr.s_addr,
							data
							);
					que_enque(self->recv_que, packet);
					break;
				}
			}
		}
		else if(recv_len == -1) {
			logging(LL_ERR, MM, "recvmsg error: %s", strerror(errno));
			break;
		}
	}

	logging(LL_DBG, MM, "Finished receiving loop");
	return NULL;
}

void* net_sending(void* arg) {
	network_t* self = arg;
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = self->port;

	logging(LL_DBG, MM, "Started sending loop");
	while(DAEMON.is_running) {
		packet_t* packet = que_deque(self->send_que);
		if(packet == NULL)
			break;

		addr.sin_addr.s_addr = packet->dst;
		sendto(self->sock, packet->data->data+packet->data->offset, packet->data->size, 0, (struct sockaddr*)&addr, sizeof(addr));
		pkt_free(packet);
	}

	logging(LL_DBG, MM, "Finished sending loop");
	return NULL;
}
