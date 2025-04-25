#include "signal_handler.h"

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "queue.h"

static pthread_mutex_t signal_lock;
static Queue* queue_ref = NULL;

void InitSignalHandlers(Queue* queue) {
	queue_ref = queue;

	pthread_mutex_init(&signal_lock, NULL);

	struct sigaction sa;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = HandleSignal;

	if (sigaction(SIGINT, &sa, NULL) == -1) {
		perror("Error: In InitSignalHandlers(): sigaction(SIGINT) failed");
		exit(EXIT_FAILURE);
	}

	if (sigaction(SIGTERM, &sa, NULL) == -1) {
		perror("Error: In InitSignalHandlers(): sigaction(SIGTERM) failed");
		exit(EXIT_FAILURE);
	}
}

void HandleSignal(int sig) {
	if (sig == SIGINT || sig == SIGTERM) {
		if (pthread_mutex_lock(&signal_lock) != 0) {
			perror("Error: In HandleSignal(): pthread_mutex_lock() failed");
			return;
		}

		is_server_running = 0;
		pthread_cond_broadcast(&queue_ref->cond);

		if (pthread_mutex_unlock(&signal_lock) != 0) {
			perror("Error: In HandleSignal(): pthread_mutex_unlock() failed");
			return;
		}
	}
}

void CleanupSignalHandlers() {
	if (pthread_mutex_destroy(&signal_lock) != 0) {
		perror("Error: In CleanupSignalHandlers(): pthread_mutex_destroy() failed");
	}
	
	queue_ref = NULL;
}

// src/signal_handler.c