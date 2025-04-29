#include <stddef.h>
#include <string.h>

#include "exchange.h"
#include "payload.h"
#include "log.h"

static const char* MM="EXG";

payload_rule_t header_rule[] = {
	{offsetof(ike_header_t, SPIi), 8, false},
	{offsetof(ike_header_t, SPIr), 8, false},
	{offsetof(ike_header_t, next_payload_type), 1, false},
	{offsetof(ike_header_t, version), 1, false},
	{offsetof(ike_header_t, exchange_type), 1, false},
	{offsetof(ike_header_t, flags), 1, false},
	{offsetof(ike_header_t, message_id), 4, true},
	{offsetof(ike_header_t, length), 4, true}
};

exchange_t* exg_create() {
	exchange_t* self = calloc(1, sizeof(exchange_t));
	self->payloads = llt_create();

	return self;
}

exchange_t* exg_unpack(buffer_t* src) {
	exchange_t* self = exg_create();
	self->buf = src;

	logging(LL_DBG, MM, "Unpacking");
	for(int i = 0; i < 8; i++) {
		payload_rule_t* rule = &header_rule[i];
		_buf_read(src, (uint8_t*)&self->header + rule->offset, rule->size, rule->is_reverse);
		logging_hex(LL_DBG, MM, (uint8_t*)&self->header + rule->offset, rule->size);
	}
	logging(LL_DBG, MM, "Done unpacking");

	return self;
}

void exg_unpack_plds(exchange_t* self) {
	payload_type type = self->header.next_payload_type;

	logging(LL_DBG, MM, "Unpacking");
	int pld_count = 0;
	while(type != PT_NO) {
		payload_t* pld = pld_unpack(self->buf, type);
		if(pld == NULL)
			return;
		type = pld->next_type;
		llt_insert_at_last(self->payloads, pld);
		pld_count++;
	}
	logging(LL_DBG, MM, "Done unpacking - %d payloads", pld_count);
}

//#include <stdarg.h>
//#include <stdio.h>

bool _exg_has_plds(exchange_t* self, ...) {
  if(self == NULL)
    return false;

	va_list args;
	char map[PT_EAP+1] = {0,};
  va_start(args, self);
	memset(map, 0, PT_EAP+1);

	llt_travel_reset(self->payloads);
	for(payload_t* pld = llt_travel(self->payloads);
			pld != NULL;
			pld = llt_travel(self->payloads)) {
		map[pld->type]++;
	}

	for(payload_type type = va_arg(args, payload_type);
			type != PT_NO;
			type = va_arg(args, payload_type)) {
		if(map[type] == 0) {
			va_end(args);
			return false;
		}
	}

	va_end(args);
	return true;
}
