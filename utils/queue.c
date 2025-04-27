#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

#include "log.h"

static const char *MM = "QUE";

// errors
bool que_is_null(queue_t* self) {
	if(self == NULL) {
		logging(LL_ERR, MM, "queue is NULL");
		return true;
	}

	return false;
}

queue_t* que_create(bool is_mutex) {
	queue_t* self = calloc(1, sizeof(queue_t));
	self->body = llt_create();

	self->is_mutex = is_mutex;
	if(is_mutex) {
		pthread_mutex_init(&self->mutex, NULL);
		pthread_cond_init(&self->cond, NULL);
	}

	return self;
}

bool que_free(queue_t* self) {
	if(que_is_null(self) || !llt_free(self->body))
		return false;

	free(self);
	return true;
}

bool que_enque(queue_t* self, void* data) {
	// errors
	if(que_is_null(self))
		return false;

	bool ret = false;

	if(self->is_mutex) {
		pthread_mutex_lock(&self->mutex);
	}

	ret = llt_insert_at_first(self->body, data);

	if(self->is_mutex) {
		if(ret) {
			pthread_cond_signal(&self->cond);
		}
		pthread_mutex_unlock(&self->mutex);
	}

	return ret;
}

void* que_deque(queue_t* self) {
	// errors
	if(que_is_null(self))
		return NULL;

	void* ret = NULL;

	if(self->is_mutex) {
		pthread_mutex_lock(&self->mutex);
		while(llt_is_empty(self->body)) {
			pthread_cond_wait(&self->cond, &self->mutex);
		}
	}

	ret = llt_delete_at_last(self->body);

	if(self->is_mutex) {
		pthread_mutex_unlock(&self->mutex);
	}

	return ret;
}
