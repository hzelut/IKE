#ifndef __PAYLOAD_H__
#define __PAYLOAD_H__

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "buffer.h"

typedef struct {
	size_t	offset;
	int     size;
	bool		is_reverse;
}payload_rule_t;

typedef enum {
  PT_NO = 0,
  PT_SA = 33,
  PT_KE = 34,
  PT_IDi = 35,
  PT_IDr = 36,
  PT_CERT = 37,
  PT_CERTREQ = 38,
  PT_AUTH = 39,
  PT_Nx = 40,
  PT_N = 41,
  PT_D = 42,
  PT_V = 43,
  PT_TSi = 44,
  PT_TSr = 45,
  PT_SK = 46,
  PT_CP = 47,
  PT_EAP = 48
}payload_type;

typedef struct {
	payload_type type;
	void* body;

	// RFC7296 - 3.2
	uint8_t		next_type;
	uint8_t		reserved;
	uint16_t	length;

	payload_rule_t* rule;
	size_t					rule_count;
}payload_t;

payload_t*	pld_create(payload_type type);
void				pld_free(payload_t* self);

void				pld_pack(payload_t* self, buffer_t* dst);
payload_t*	pld_unpack(buffer_t* src, payload_type type);
int         pld_unpack_by_rule(uint8_t* dst, buffer_t* src, int length, payload_rule_t* rule, size_t rule_count);

char* pld_type_string(payload_type type);

#endif //__PAYLOAD_H__
