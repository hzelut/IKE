#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * 0 1 2 3 4 5 6 7 8
 * +-+-+-+-+-+-+-+-+
 * |   | Filled  | |
 * +-+-+-+-+-+-+-+-+
 * offset = 2
 * size = 5
 * capacity = 8
 *
 * end of remains = 1
 * begin of remains = 2
 */
typedef struct {
	uint8_t*	data;
	size_t		offset;
	size_t		size;
	size_t		capacity;
}buffer_t;

buffer_t* buf_create(size_t capacity);
bool			buf_free(buffer_t* self);

bool			_buf_write(buffer_t* self, void* data, size_t size, bool reverse);
#define buf_write(self, data, size) _buf_write(self, data, size, false)
#define buf_rwrite(self, data, size) _buf_write(self, data, size, true)
bool			buf_bwrite(buffer_t* self, buffer_t* src);

bool			_buf_read(buffer_t* self, void* dest, size_t size, bool reverse);
#define buf_read(self, dest, size) _buf_read(self, dest, size, false)
#define buf_rread(self, dest, size) _buf_read(self, dest, size, true)
bool			buf_bread(buffer_t* self, buffer_t* dst, size_t size);

bool			buf_merge(buffer_t* self, buffer_t* src, bool is_src_free);

bool			buf_is_empty(buffer_t* self);

#endif //__BUFFER_H__
