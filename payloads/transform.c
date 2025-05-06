#include "transform.h"
#include "payload.h"
#include "log.h"

static const char* MM="TRN";

payload_rule_t transform_rule[] = {
	{offsetof(transform_t, last), 1, false},
	{offsetof(transform_t, reserved), 1, false},
	{offsetof(transform_t, length), 2, true},
	{offsetof(transform_t, type), 1, false},
	{offsetof(transform_t, reserved), 1, false},
	{offsetof(transform_t, id), 2, true},
};


uint8_t get_format(uint16_t value) {
	return (value >> 15) & 0x01;
}

uint16_t get_type(uint16_t value) {
	return value & 0x7FFF;
}

transform_t* trn_create(size_t count) {
	transform_t* self = calloc(count, sizeof(transform_t));

	return self;
}

transform_t* trn_unpack(buffer_t* src, size_t count) {
	transform_t* trns = trn_create(count);

	for(size_t i = 0; i < count; i++) {
		transform_t* cur = &trns[i];
		int len = pld_unpack_by_rule((void*)cur, src, 0, transform_rule, 6);
		if(len < cur->length) {
			buf_rread(src, &cur->attribute.type, 2);
			logging_hex(LL_DBG, MM, &cur->attribute.type, 2);
			buf_rread(src, &cur->attribute.length, 2);
			logging_hex(LL_DBG, MM, &cur->attribute.length, 2);
			if(get_format(cur->attribute.type) == 0) {
				cur->attribute.variable_value = calloc(1, cur->attribute.length);
				buf_read(src, cur->attribute.variable_value, cur->attribute.length);
				logging_hex(LL_DBG, MM, cur->attribute.variable_value, cur->attribute.length);
			}
		}
	}

	return trns;
}
