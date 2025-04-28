#include <stdlib.h>
#include <string.h>

#include "manager.h"
#include "config.h"
#include "sa.h"
#include "log.h"
#include "daemon.h"

static const char* MM="SAM";

manager_t* sam_create() {
	manager_t* self =	calloc(1, sizeof(manager_t));
	self->sas =	llt_create();

	return self;
}

void* sam_init_sas(manager_t* self, char* conf_filename) {
	int conf_count = 0;
	configure_t* confs = cnf_parse(conf_filename, &conf_count);

	for(int i = 0; i < conf_count; i++) {
		sa_t* sa = sa_create();
		sa->local.addr = confs[i].local.addr;
		sa->remote.addr = confs[i].remote.addr;
		sa->secret = calloc(strlen(confs[i].secret), sizeof(char));
		memcpy(sa->secret, confs[i].secret, strlen(confs[i].secret));

		llt_insert_at_last(self->sas, sa);
		logging(LL_DBG, MM, "Added new SA[%d]", i);
	}

	push_job(sam_running, self);
	return NULL;
}

void* sam_running(void* arg) {
	manager_t* self = arg;

	ip4_addr src, dst;
	buffer_t* buf;

	logging(LL_DBG, MM, "Started running loop");
	while(DAEMON.is_running) {
		buf = net_recv(&src, &dst);
		if(buf) {
			llt_travel_reset(self->sas);
			for(sa_t* sa = llt_travel(self->sas); sa != NULL; sa = llt_travel(self->sas)) {
				if(sa->local.addr == dst && sa->remote.addr == src) {
					logging(LL_DBG, MM, "[%s] -> [%s]", net_atos(src), net_atos(dst));
					exchange_t* exg = exg_unpack(buf);
					if(sa->last_exchange_type == 0 && exg->header.exchange_type == IKE_SA_INIT) {
					}
					break;
				}
			}

			buf_free(buf);
		}

		buf = NULL;
	}

	logging(LL_DBG, MM, "Finished running loop");
	return NULL;
}
