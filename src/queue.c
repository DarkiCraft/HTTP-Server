#include "queue.h"

#include <stdio.h>
#include <stdlib.h>

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
		pthread_mutex_destroy(&queue->lock);
		free(queue->queue);
		exit(EXIT_FAILURE);
	}
}

void Enqueue(Queue* queue, int client_socket) {
	pthread_mutex_lock(&queue->lock);

	if (queue->count == queue->size) {
		perror("Error: In Enqueue(): Queue is full, dropping connection");
		pthread_mutex_unlock(&queue->lock);
		return;
	}

	queue->queue[queue->rear] = client_socket;
	queue->rear = (queue->rear + 1) % queue->size;
	queue->count++;

	pthread_cond_signal(&queue->cond);
	pthread_mutex_unlock(&queue->lock);
}

int Dequeue(Queue* queue) {
	pthread_mutex_lock(&queue->lock);

	while (queue->count == 0) {
		if (pthread_cond_wait(&queue->cond, &queue->lock) != 0) {
			perror("Error: In Dequeue(): pthread_cond_wait() failed");
			pthread_mutex_unlock(&queue->lock);
			return -1;
		}
	}

	int client_socket = queue->queue[queue->front];
	queue->front = (queue->front + 1) % queue->size;
	queue->count--;

	if (pthread_mutex_unlock(&queue->lock) != 0) {
		perror("Error: In Dequeue(): pthread_mutex_unlock() failed");
	}

	return client_socket;
}

void CleanupQueue(Queue* queue) {
	free(queue->queue);
	pthread_mutex_destroy(&queue->lock);
	pthread_cond_destroy(&queue->cond);
}