#include "ke_payload.h"
#include "log.h"

static const char* MM="KEPLD";

payload_rule_t ke_payload_rule[] = {
	{offsetof(ke_payload_t, dh_group), 2, true},
	{offsetof(ke_payload_t, reserved), 2, false},
	{offsetof(ke_payload_t, data), -1, false}
};

payload_t* pld_ke_create() {
	payload_t* pld = calloc(1, sizeof(payload_t));
	pld->body = calloc(1, sizeof(ke_payload_t));
	pld->type = PT_KE;
	pld->rule = ke_payload_rule;
	pld->rule_count = 3;

	return pld;
}
