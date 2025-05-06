#include "payload.h"
#include "log.h"

#include "sa_payload.h"
#include "ke_payload.h"
#include "nx_payload.h"
#include "n_payload.h"

static const char* MM="PLD";

payload_t* pld_create(payload_type type) {
	payload_t* self = NULL;

	switch(type) {
		case PT_SA:
			self = pld_sa_create();
			break;
		case PT_KE:
			self = pld_ke_create();
			break;
		case PT_Nx: 
			self = pld_nx_create();
			break;
		case PT_N:
			self = pld_n_create();
			break;
		case PT_IDi:
		case PT_IDr:
		case PT_CERT:
		case PT_CERTREQ:
		case PT_AUTH: 
		case PT_D:
		case PT_V:
		case PT_TSi: 
		case PT_TSr:
		case PT_SK:
		case PT_CP:
		case PT_EAP:
		case PT_NO: 
		default:
			logging(LL_DBG, MM, "Not yet supported %s", pld_type_string(type));
			//func(self);
			break;
	}

	return self;
}

char* pld_type_string(payload_type type) {
	switch(type) {
		case PT_NO: return "No";
		case PT_SA: return "Security Association";
		case PT_KE: return "Key Exchange";
		case PT_IDi: return "Identification - Initiator";
		case PT_IDr: return "Identification - Responder";
		case PT_CERT: return "Certificate";
		case PT_CERTREQ: return "Certificate Request";
		case PT_AUTH: return "Authentication";
		case PT_Nx: return "Nonce";
		case PT_N: return "Notify";
		case PT_D: return "Delete";
		case PT_V: return "Vendor ID";
		case PT_TSi: return "Traffic Selector - Initiator";
		case PT_TSr: return "Traffic Selector - Responder";
		case PT_SK: return "Encrypted and Authenticated";
		case PT_CP: return "Configuration";
		case PT_EAP: return "Extensible Authentication";
		default: return "WTF!";
	}
}

int pld_unpack_by_rule(uint8_t* dst, buffer_t* src, int length, payload_rule_t* rule, size_t rule_count) {
	int l = 0;

	for(size_t i = 0; i < rule_count; i++) {
		payload_rule_t* r = &rule[i];
		int s = r->size;
		void* d = dst + r->offset;

		// Dynamic
		if(s == -1) {
			s = length - l;
			*(void **)(dst + r->offset) = calloc(1, s);
			d = *(void **)(dst + r->offset);
		}

		_buf_read(src, d, s, r->is_reverse);
		l += s;
		logging_hex(LL_DBG, MM, d, s);
	}

	return l;
}

payload_t* pld_unpack(buffer_t* src, payload_type type) {
	payload_t* self = pld_create(type);
	if(self == NULL) {
		return NULL;
	}

	logging(LL_DBG, MM, "Unpacking %s", pld_type_string(type));

	// RFC7296 - 3.2
	// Generic Payload Header
	_buf_read(src, &self->next_type, 1, false);
	logging_hex(LL_DBG, MM, &self->next_type, 1);
	_buf_read(src, &self->reserved, 1, false);
	logging_hex(LL_DBG, MM, &self->reserved, 1);
	_buf_read(src, &self->length, 2, true);
	logging_hex(LL_DBG, MM, &self->length, 2);

	if(type == PT_SA)
		pld_sa_unpack(self, src);
	else {
		int l = pld_unpack_by_rule(self->body, src, self->length-4, self->rule, self->rule_count);
		if(l+4 < self->length) {
			if(type == PT_N) {
				pld_n_unpack(self, src);
			}
			else {
				logging(LL_ERR, MM, "WTF!! where did the %d bytes go", self->length - l);
				return NULL;
			}
		}
	}
	logging(LL_DBG, MM, "Done unpacking %s", pld_type_string(type));

	return self;
}
