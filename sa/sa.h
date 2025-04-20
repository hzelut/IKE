#ifndef __SA_H__
#define __SA_H__

#include <stdint.h>
#include <stdbool.h>

#include "exchange.h"
#include "netype.h"

typedef struct {
	struct {
		ip4_addr addr;
	}local, remote;
	char* secret;

	bool						is_initiator;
	uint64_t				SPIi;
	uint64_t				SPIr;
	exchange_type		last_exchange_type;
	uint32_t				message_id;
}sa_t;

sa_t*		sa_create();

#endif //__SA_H__
