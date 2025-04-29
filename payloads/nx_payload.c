#include "nx_payload.h"

payload_rule_t nx_payload_rule[] = {
	{offsetof(nx_payload_t, data), -1, false}
};

payload_t* pld_nx_create() {
	payload_t* pld = calloc(1, sizeof(payload_t));
	pld->body = calloc(1, sizeof(nx_payload_t));
	pld->type = PT_Nx;
	pld->rule = nx_payload_rule;
	pld->rule_count = 1;

	return pld;
}
