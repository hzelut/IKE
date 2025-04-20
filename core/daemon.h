#ifndef __DAEMON_H__
#define __DAEMON_H__

#include "log.h"
#include "network.h"
#include "manager.h"

#define WORKER_MAX 10

typedef struct {
	pthread_t id;
	void*			(*func)(void *);
	void*			arg;
}job_t;

typedef struct {
	log_t*			LOG;
	network_t*	NET;
	manager_t*	SAM;

	bool			is_running;
	pthread_t worker[WORKER_MAX];
	queue_t*	job_que;
}daemon_t;

extern daemon_t DAEMON;

void daemon_create();
void daemon_free(daemon_t* self);
void push_job(void* (*func)(void*), void* arg);
void running();

#endif //__DAEMON_H__
