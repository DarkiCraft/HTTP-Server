#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

void InitQueue(Queue* queue, int size) {
	if (size <= 0) {
		size = 1;
	}

	queue->queue = (int*)malloc(size * sizeof(int));
	if (queue->queue == NULL) {
		perror("Error: In InitQueue(): malloc() failed");
		exit(EXIT_FAILURE);
	}

	queue->size = size;
	queue->front = 0;
	queue->rear = 0;
	queue->count = 0;

	if (pthread_mutex_init(&queue->lock, NULL) != 0) {
		perror("Error: In InitQueue(): pthread_mutex_init() failed");
		free(queue->queue);
		exit(EXIT_FAILURE);
	}

	if (pthread_cond_init(&queue->cond, NULL) != 0) {
		perror("Error: In InitQueue(): pthread_cond_init() failed");
		if (pthread_mutex_destroy(&queue->lock) != 0) {
			perror("Error: In InitQueue(): pthread_mutex_destroy() failed");
		}
		free(queue->queue);
		exit(EXIT_FAILURE);
	}
}

void Enqueue(Queue* queue, int client_socket) {
	if (queue == NULL) {
		(void)fprintf(stderr, "Error: In Enqueue(): queue is NULL\n");
		return;
	}

	if (client_socket < 0) {
		(void)fprintf(stderr, "Error: In Enqueue(): client_socket is invalid\n");
		return;
	}

	if (queue->queue == NULL) {
		(void)fprintf(stderr, "Error: In Enqueue(): queue is not initialized\n");
		return;
	}

	if (pthread_mutex_lock(&queue->lock) != 0) {
		perror("Error: In Enqueue(): pthread_mutex_lock() failed");
		return;
	}

	if (queue->count == queue->size) {
		perror("Error: In Enqueue(): Queue is full, dropping connection");
		if (pthread_mutex_unlock(&queue->lock) != 0) {
			perror("Error: In Enqueue(): pthread_mutex_unlock() failed");
		}
		return;
	}

	queue->queue[queue->rear] = client_socket;
	queue->rear = (queue->rear + 1) % queue->size;
	queue->count++;

	if (pthread_cond_signal(&queue->cond) != 0) {
		perror("Error: In Enqueue(): pthread_cond_signal() failed");
	}

	if (pthread_mutex_unlock(&queue->lock) != 0) {
		perror("Error: In Enqueue(): pthread_mutex_unlock() failed");
		return;
	}
}

int Dequeue(Queue* queue) {
	if (queue == NULL) {
		(void)fprintf(stderr, "Error: In Dequeue(): queue is NULL\n");
		return -1;
	}

	if (!is_server_running) {
		return -1;
	}

	if (pthread_mutex_lock(&queue->lock) != 0) {
		perror("Error: In Dequeue(): pthread_mutex_lock() failed");
		return -1;
	}

	if (!is_server_running) {
		pthread_mutex_unlock(&queue->lock);
		return -1;
	}

	while (queue->count == 0 && is_server_running) {
		if (pthread_cond_wait(&queue->cond, &queue->lock) != 0) {
			pthread_mutex_unlock(&queue->lock);
			perror("Error: In Dequeue(): pthread_cond_wait() failed");
			return -1;
		}

		if (!is_server_running) {
			pthread_mutex_unlock(&queue->lock);
			return -1;
		}
	}

	if (queue->count == 0) {
		pthread_mutex_unlock(&queue->lock);
		return -1;
	}

	int client_socket = queue->queue[queue->front];
	queue->front = (queue->front + 1) % queue->size;
	queue->count--;

	pthread_mutex_unlock(&queue->lock);
	return client_socket;
}

void CleanupQueue(Queue* queue) {
	if (queue == NULL) {
		(void)fprintf(stderr, "Error: In CleanupQueue(): queue is NULL\n");
		return;
	}

	free(queue->queue);

	if (pthread_mutex_destroy(&queue->lock) != 0) {
		perror("Error: In CleanupQueue(): pthread_mutex_destroy() failed");
	}

	if (pthread_cond_destroy(&queue->cond) != 0) {
		perror("Error: In CleanupQueue(): pthread_cond_destroy() failed");
	}
}

// src/queue.c