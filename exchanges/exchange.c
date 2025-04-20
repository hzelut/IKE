#include <stddef.h>
#include "exchange.h"
#include "payload.h"

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

	for(int i = 0; i < 8; i++) {
		payload_rule_t* rule = &header_rule[i];
		_buf_read(src, self + rule->offset, rule->size, rule->is_reverse);
	}

	return self;
}
