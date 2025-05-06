#include <time.h>

#include "utils.h"

buffer_t* get_rand(int bytes) {
	buffer_t* dst = buf_create(bytes);
	unsigned char* tmp = malloc(bytes);

	srand(time(NULL)+(*tmp));
	for(int i = 0; i < bytes; i++)
		tmp[i] = rand();

	buf_write(dst, tmp, bytes);
	free(tmp);

	return dst;
}

uint64_t get_rand_spi() {
	uint64_t spi;
	srand(time(NULL)+(*((unsigned int*)&spi)));
	spi = rand() | ((uint64_t)rand())<<32;
	return spi;
}
