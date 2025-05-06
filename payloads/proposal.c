#include "proposal.h"
#include "payload.h"
#include "log.h"

static const char* MM="PPS";

payload_rule_t proposal_rule[] = {
	{offsetof(proposal_t, last), 1, false},
	{offsetof(proposal_t, reserved), 1, false},
	{offsetof(proposal_t, length), 2, true},
	{offsetof(proposal_t, number), 1, false},
	{offsetof(proposal_t, protocol), 1, false},
	{offsetof(proposal_t, spi_size), 1, false},
	{offsetof(proposal_t, num_of_transforms), 1, false},
};

proposal_t* pps_create() {
	proposal_t* self = calloc(1, sizeof(proposal_t));

	return self;
}

proposal_t* pps_unpack(buffer_t* src) {
	proposal_t* self = pps_create();

	pld_unpack_by_rule((void*)self, src, 0, proposal_rule, 7);

	if(self->spi_size > 0) {
		self->spi = calloc(1, self->spi_size);
		buf_read(src, self->spi, self->spi_size);
		logging_hex(LL_DBG, MM, self->spi, self->spi_size);
	}
	if(self->num_of_transforms > 0) {
		logging(LL_DBG, MM, "Found %d transforms", self->num_of_transforms);
		self->transforms = trn_unpack(src, self->num_of_transforms);
	}

	return self;

}
