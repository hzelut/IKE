#include <stdlib.h>

#include "sa.h"

sa_t* sa_create() {
	sa_t* self = calloc(1, sizeof(sa_t));

	return self;
}
