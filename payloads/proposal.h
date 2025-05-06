#ifndef __PROPOSAL_H__
#define __PROPOSAL_H__

#include "buffer.h"
#include "transform.h"

/* RFC7296 - 3.3.1
                        1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | Last Substruc |   RESERVED    |         Proposal Length       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | Proposal Num  |  Protocol ID  |    SPI Size   |Num  Transforms|
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   ~                        SPI (variable)                         ~
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                                                               |
   ~                        <Transforms>                           ~
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

                      Figure 7: Proposal Substructure
 */
typedef struct {
	uint8_t last;
	uint8_t reserved;
	uint16_t length;
	uint8_t number;
	uint8_t protocol;
	uint8_t spi_size;
	uint8_t num_of_transforms;
	void* spi;
	transform_t* transforms;
}proposal_t;

proposal_t* pps_create();
proposal_t* pps_unpack(buffer_t* src);

#define pps_is_last(last) (last == 0)

#endif //__PROPOSAL_H__
