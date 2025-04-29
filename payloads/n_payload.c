#include "n_payload.h"
#include "log.h"

static const char* MM="NPLD";

payload_rule_t n_payload_rule[] = {
	{offsetof(n_payload_t, protocol), 1, false},
	{offsetof(n_payload_t, spi_size), 1, false},
	{offsetof(n_payload_t, message_type), 2, true}
};

payload_t* pld_n_create() {
	payload_t* pld = calloc(1, sizeof(payload_t));
	pld->body = calloc(1, sizeof(n_payload_t));
	pld->type = PT_N;
	pld->rule = n_payload_rule;
	pld->rule_count = 3;

	return pld;
}

void pld_n_unpack(payload_t* pld, buffer_t* src) {
	n_payload_t* self = pld->body;
	if(self->spi_size > 0) {
		self->spi = calloc(1, self->spi_size);
		_buf_read(src, self->spi, self->spi_size, false);
		logging_hex(LL_DBG, MM, self->spi, self->spi_size);
	}

	int data_len = pld->length - 8 - self->spi_size;
	if(data_len > 0) {
		self->data = calloc(1, data_len);
		_buf_read(src, self->data, data_len, false);
		logging_hex(LL_DBG, MM, self->data, data_len);
	}
}
