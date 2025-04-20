#include <stdlib.h>
#include <string.h>

#include "manager.h"
#include "config.h"
#include "sa.h"
#include "log.h"

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

	return NULL;
}
