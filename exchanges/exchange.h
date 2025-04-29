#ifndef __EXCHANGE_H__
#define __EXCHANGE_H__

#include <stdint.h>
#include "linked_list.h"
#include "buffer.h"

/* RFC7296 - 3.1
                        1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                       IKE SA Initiator's SPI                  |
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                       IKE SA Responder's SPI                  |
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  Next Payload | MjVer | MnVer | Exchange Type |     Flags     |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                          Message ID                           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                            Length                             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

                        Figure 4: IKE Header Format
 */

typedef enum {
	IKE_SA_INIT				= 34,
	IKE_AUTH					= 35,
	CREATE_CHILD_SA		= 36,
	INFORMATIONAL			= 37
} exchange_type;

typedef struct {
	uint64_t	SPIi;
	uint64_t	SPIr;
	uint8_t		next_payload_type;
	uint8_t		version;
	uint8_t		exchange_type;
	uint8_t		flags;
	uint32_t	message_id;
	uint32_t	length;
}ike_header_t;

typedef struct {
	ike_header_t header;
	linked_list_t* payloads;
	buffer_t* buf;
}exchange_t;

exchange_t* exg_create();

exchange_t* exg_unpack(buffer_t* src);
void				exg_unpack_plds(exchange_t* self);

bool				_exg_has_plds(exchange_t* self, ...);
#define exg_has_plds(self, ...) _exg_has_plds(self, __VA_ARGS__, 0)

#endif //__EXCHANGE_H__
