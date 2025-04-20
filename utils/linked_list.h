#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include <stdbool.h>

typedef struct {
	int		length;
	void*	first;
	void* last;
	void* current; // for travel
}linked_list_t;

linked_list_t*	llt_create();
bool						llt_free(linked_list_t* self);

bool llt_insert(linked_list_t* self, void* data, int position);
#define llt_insert_at_first(self, data) llt_insert(self, data, 1)
#define llt_insert_at_last(self, data) llt_insert(self, data, 0)
#define llt_insert_at_position(self, data, position) llt_insert(self, data, position)

void* llt_delete(linked_list_t* self, int position);
#define llt_delete_at_first(self) llt_delete(self, 1)
#define llt_delete_at_last(self) llt_delete(self, 0)
#define llt_delete_at_position(self, position) llt_delete(self, position)

void* llt_travel(linked_list_t* self);
bool	llt_travel_reset(linked_list_t* self);
bool	llt_is_empty(linked_list_t* self);

#endif //__LINKED_LIST_H__
