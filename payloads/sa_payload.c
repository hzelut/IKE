#include "sa_payload.h"
#include "log.h"

static const char* MM="SAPLD";

payload_rule_t sa_payload_rule[] = {
};

payload_t* pld_sa_create() {
	payload_t* pld = calloc(1, sizeof(payload_t));
	pld->body = calloc(1, sizeof(sa_payload_t));
	pld->type = PT_SA;
	pld->rule = sa_payload_rule;
	pld->rule_count = 0;

	return pld;
}

void pld_sa_unpack(payload_t* pld, buffer_t* src) {
	_buf_read(src, NULL, pld->length-4, false);
}
