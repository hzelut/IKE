#include "payload.h"

payload_t* pld_create(payload_type type) {
	payload_t* self = calloc(1, sizeof(payload_t));
	self->type = type;

	switch(type) {
		case PT_NO: 
		case PT_SA:
		case PT_KE:
		case PT_IDi:
		case PT_IDr:
		case PT_CERT:
		case PT_CERTREQ:
		case PT_AUTH: 
		case PT_Nx: 
		case PT_N:
		case PT_D:
		case PT_V:
		case PT_TSi: 
		case PT_TSr:
		case PT_SK:
		case PT_CP:
		case PT_EAP:
		default:
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
