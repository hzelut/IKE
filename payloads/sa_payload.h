#ifndef __SA_PAYLOAD_H__
#define __SA_PAYLOAD_H__

#include "payload.h"
#include "linked_list.h"

/* RFC7296 - 3.3
                        1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | Next Payload  |C|  RESERVED   |         Payload Length        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                                                               |
   ~                          <Proposals>                          ~
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

                  Figure 6: Security Association Payload
 */
typedef struct {
	linked_list_t* proposals;
}sa_payload_t;

payload_t*	pld_sa_create();

void				pld_sa_unpack(payload_t* pld, buffer_t* src);

#endif //__SA_PAYLOAD_H__
