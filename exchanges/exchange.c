#include <stddef.h>

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

	logging(LL_DBG, MM, "Unpacking");
	for(int i = 0; i < 8; i++) {
		payload_rule_t* rule = &header_rule[i];
		_buf_read(src, (uint8_t*)&self->header + rule->offset, rule->size, rule->is_reverse);
		logging_hex(LL_DBG, MM, &self->header + rule->offset, rule->size);
	}
	logging(LL_DBG, MM, "Done unpacking");

	return self;
}
