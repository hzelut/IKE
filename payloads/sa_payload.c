#include "sa_payload.h"
#include "log.h"
#include "proposal.h"

static const char* MM="SAPLD";

payload_rule_t sa_payload_rule[] = {
};

payload_t* pld_sa_create() {
	payload_t* pld = calloc(1, sizeof(payload_t));
	pld->type = PT_SA;
	pld->rule = sa_payload_rule;
	pld->rule_count = 0;

	sa_payload_t* sa = calloc(1, sizeof(sa_payload_t));
	sa->proposals = llt_create();

	pld->body = sa;
	return pld;
}

void pld_sa_unpack(payload_t* pld, buffer_t* src) {
	sa_payload_t* self = pld->body;
	proposal_t* pro = NULL;

	do {
		pro = pps_unpack(src);
		llt_insert_at_last(self->proposals, pro);
		logging(LL_DBG, MM, "Unpacking proposal %d", pro->number);
	} while(!pps_is_last(pro->last));
}
