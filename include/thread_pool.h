#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>

#include "queue.h"

/**
 * @struct ThreadPool
 * @brief A structure representing a thread pool.
 *
 * This structure contains the necessary elements to manage a pool of threads
 * and distribute tasks to them. It handles the creation and management of
 * threads that will process client connections or other tasks concurrently.
 */
typedef struct {
	pthread_t* threads;	 // Array of thread IDs representing the thread pool
	int num_threads;		 // Number of threads in the pool */
	Queue* queue;	 // Pointer to the client queue, which holds client sockets
	pthread_mutex_t lock;	 // Mutex to synchronize access to shared resources
	pthread_cond_t
			cond;	 // Condition variable to signal Worker threads to process tasks
} ThreadPool;

/**
 * @brief Initializes the thread pool.
 *
 * This function initializes the thread pool by allocating memory for the thread
 * array, initializing the mutex and condition variable, and creating the
 * threads that will wait for tasks in the client queue.
 *
 * @param pool Pointer to the thread pool structure to be initialized.
 * @param num_threads The number of threads to create in the pool.
 * @param queue Pointer to the client queue which holds the tasks (client
 * sockets).
 */
void InitThreadPool(ThreadPool* pool, int num_threads, Queue* queue);

/**
 * @brief Worker function executed by each thread in the thread pool.
 *
 * This function represents the task that each thread in the pool will execute.
 * It continuously waits for tasks in the client queue and processes them when
 * available (e.g., handling client connections).
 *
 * @param arg Pointer to the thread pool structure passed to the Worker.
 * @return NULL.
 */
void* WorkerThread(void* arg);

/**
 * @brief Destroys the thread pool.
 *
 * This function cleans up resources used by the thread pool, including joining
 * all threads and freeing allocated memory.
 *
 * @param pool Pointer to the thread pool structure to be destroyed.
 */
void CleanupThreadPool(ThreadPool* pool);

#endif	// THREAD_POOL_H
