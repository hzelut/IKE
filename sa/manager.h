#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "linked_list.h"

typedef struct {
	linked_list_t* sas;
}manager_t;

manager_t* sam_create();

void*			sam_init_sas(manager_t* self, char* conf_filename);

#endif //__MANAGER_H__
