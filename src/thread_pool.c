#include "thread_pool.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "parser.h"
#include "queue.h"
#include "signal_handler.h"
#include "transaction_handler.h"

void InitThreadPool(ThreadPool* pool, int num_thread, Queue* queue) {
	if (num_thread <= 0) {
		(void)fprintf(stderr,
									"Error: In InitThreadPool(): num_thread must be > 0\n");
		exit(EXIT_FAILURE);
	}

	if (queue == NULL) {
		(void)fprintf(stderr, "Error: In InitThreadPool(): queue is NULL\n");
		exit(EXIT_FAILURE);
	}

	pool->num_threads = num_thread;
	pool->queue = queue;

	pool->threads = (pthread_t*)malloc(num_thread * sizeof(pthread_t));
	if (pool->threads == NULL) {
		perror("Error: In InitThreadPool(): malloc() failed");
		exit(EXIT_FAILURE);
	}

	if (pthread_mutex_init(&pool->lock, NULL) != 0) {
		perror("Error: In InitThreadPool(): pthread_mutex_init() failed");
		free(pool->threads);
		exit(EXIT_FAILURE);
	}

	if (pthread_cond_init(&pool->cond, NULL) != 0) {
		perror("Error: In InitThreadPool(): pthread_cond_init() failed");
		free(pool->threads);
		if (pthread_mutex_destroy(&pool->lock) != 0) {
			perror("Error: In InitThreadPool(): pthread_mutex_destroy() failed");
		}
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < num_thread; i++) {
		if (pthread_create(&pool->threads[i], NULL, WorkerThread, (void*)pool) !=
				0) {
			perror("Error: In InitThreadPool(): pthread_create() failed");

			for (int j = 0; j < i; j++) {
				if (pthread_cancel(pool->threads[j]) != 0) {
					perror("Error: In InitThreadPool(): pthread_cancel() failed");
				}
			}

			free(pool->threads);

			if (pthread_mutex_destroy(&pool->lock) != 0) {
				perror("Error: In InitThreadPool(): pthread_mutex_destroy() failed");
			}

			if (pthread_cond_destroy(&pool->cond) != 0) {
				perror("Error: In InitThreadPool(): pthread_cond_destroy() failed");
			}

			exit(EXIT_FAILURE);
		}
	}
}

void* WorkerThread(void* arg) {
	DisableSignalsInThread();

	while (is_server_running) {
		int client_socket = Dequeue(((ThreadPool*)arg)->queue);
		if (client_socket < 0) {
			continue;
		}

		HandleTransaction(client_socket);

		if (close(client_socket) < 0) {
			perror("Error: In WorkerThread(): close() failed");
		}
	}

	return NULL;
}

void CleanupThreadPool(ThreadPool* pool) {
	if (pool == NULL) {
		return;
	}

	for (int i = 0; i < pool->num_threads; i++) {
		if (pthread_join(pool->threads[i], NULL) != 0) {
			perror("Error: In CleanupThreadPool(): pthread_join() failed");
		}
	}

	free(pool->threads);
	pool->threads = NULL;

	if (pthread_mutex_destroy(&pool->lock) != 0) {
		perror("Error: In CleanupThreadPool(): pthread_mutex_destroy() failed");
	}

	if (pthread_cond_destroy(&pool->cond) != 0) {
		perror("Error: In CleanupThreadPool(): pthread_cond_destroy() failed");
	}
}

// src/thread_pool.c