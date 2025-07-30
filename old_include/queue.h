#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

/**
 * @struct Queue
 * @brief Represents a queue for storing client socket descriptors.
 *
 * This structure maintains the client socket descriptors in a circular queue.
 * The queue is thread-safe and uses a mutex for synchronization.
 */
typedef struct {
	int* queue;	 // Array to store client sockets
	int size;		 // Maximum size of the queue
	int front, rear, count;
	pthread_mutex_t lock;	 // Mutex to ensure thread safety
	pthread_cond_t cond;	 // Condition variable for threads to be notified
} Queue;

/**
 * @brief Initializes a client queue.
 *
 * This function initializes a queue to hold client socket descriptors,
 * allocates memory for the queue array, and sets up mutex and condition
 * variable for thread synchronization.
 *
 * @param queue Pointer to the client queue structure.
 */
void InitQueue(Queue* queue, int size);

/**
 * @brief Adds a client socket descriptor to the queue.
 *
 * This function adds a new client socket to the queue, and signals a Worker
 * thread to process the newly added connection. If the queue is full, the
 * connection is dropped.
 *
 * @param queue Pointer to the client queue structure.
 * @param client_socket The client socket descriptor to be added to the queue.
 */
void Enqueue(Queue* queue, int client_socket);

/**
 * @brief Retrieves a client socket descriptor from the queue.
 *
 * This function blocks and waits for a Worker thread to be notified when a
 * client connection is added to the queue. Once a client socket is available,
 * it returns the socket descriptor.
 *
 * @param queue Pointer to the client queue structure.
 * @return The client socket descriptor.
 */
int Dequeue(Queue* queue);

/**
 * @brief Cleans up and frees resources allocated for the queue.
 *
 * This function frees the memory for the queue array and destroys the mutex
 * and condition variable.
 *
 * @param queue Pointer to the client queue structure.
 */
void CleanupQueue(Queue* queue);

#endif	// QUEUE_H

// include/queue.h