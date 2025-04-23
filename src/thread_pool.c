#include "thread_pool.h"

#include <stdio.h>
#include <stdlib.h>

void InitThreadPool(ThreadPool* pool, int num_thread, Queue* queue) {
	if (num_thread <= 0) {
		(void)fprintf(stderr, "Error: In InitThreadPool(): num_thread must be > 0");
		exit(EXIT_FAILURE);
	}

	if (queue == NULL) {
		(void)fprintf(stderr, "Error: In InitThreadPool(): queue is NULL");
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
		(void)pthread_mutex_destroy(&pool->lock);
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < num_thread; i++) {
		if (pthread_create(&pool->threads[i], NULL, WorkerThread, (void*)pool) !=
				0) {
			perror("Error: In InitThreadPool(): pthread_create() failed");

			for (int j = 0; j < i; j++) {
				pthread_cancel(pool->threads[j]);
			}

			free(pool->threads);
			(void)pthread_mutex_destroy(&pool->lock);
			(void)pthread_cond_destroy(&pool->cond);

			exit(EXIT_FAILURE);
		}
	}
}

void* WorkerThread(void* arg) {
	while (1);
	return NULL;
}

void CleanupThreadPool(ThreadPool* pool) {
	if (pool == NULL) {
		return;
	}

	for (int i = 0; i < pool->num_threads; i++) {
		pthread_cancel(pool->threads[i]);
	}

	free(pool->threads);
	pool->threads = NULL;

	(void)pthread_mutex_destroy(&pool->lock);
	(void)pthread_cond_destroy(&pool->cond);
}
