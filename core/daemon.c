#include <signal.h>
#include <unistd.h>

#include "daemon.h"

daemon_t DAEMON;

static const char* MM="DAEMON";

void* worker_loop(void* arg);
void signal_handler(int sig);

void daemon_create() {
  DAEMON.LOG = log_create();

	DAEMON.job_que = que_create(true);
	DAEMON.NET = net_create();
	DAEMON.SAM = sam_create();

}

void daemon_free(daemon_t* self) {
	if(!self->is_running) {
		que_free(self->job_que);
		net_free(DAEMON.NET);
	}
};

void push_job(void* (*func)(void*), void* arg) {
	job_t* job = calloc(1, sizeof(job_t));
	job->func = func;
	job->arg = arg;
	que_enque(DAEMON.job_que, job);
	logging(LL_DBG, MM, "Added a new job");
}

void* worker_loop(void* arg) {
	(void)arg;
	while(DAEMON.is_running) {
		job_t* job = que_deque(DAEMON.job_que);
		if(job == NULL) {
			break;
		}

		if(job->func) {
			job->func(job->arg);
			free(job);
		}
	}
	logging(LL_DBG, MM, "worker stop");
	return NULL;
}

void daemon_exit() {
	for(int i =0; i < WORKER_MAX; i++)
		que_enque(DAEMON.job_que, NULL);

	net_exit(DAEMON.NET);
}

void signal_handler(int sig) {
	logging(LL_DBG, MM, "Received signal %d", sig);

	DAEMON.is_running = false;
	daemon_exit();
}

void running() {
	DAEMON.is_running = true;

	signal(SIGINT, signal_handler);

	for(int i =0; i < WORKER_MAX; i++) {
		if(pthread_create(&DAEMON.worker[i], NULL, worker_loop, NULL) != 0)
			logging(LL_ERR, MM, "Failed create worker %d", i);
		else
			logging(LL_DBG, MM, "Started worker %d", i);
	}
	logging(LL_DBG, MM, "All worker started");

	for(int i =0; i < WORKER_MAX; i++) {
		pthread_join(DAEMON.worker[i], NULL);
	}
	logging(LL_DBG, MM, "All worker finished");
}
