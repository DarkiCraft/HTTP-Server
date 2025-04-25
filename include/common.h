#ifndef COMMON_H
#define COMMON_H

#include <signal.h>
#include <stddef.h>

/**
 * @brief The port number on which the server listens for incoming connections.
 *
 * This constant defines the network port used by the server to accept
 * client connections. It should be set to a valid, unused port number.
 *
 * @note Must be initialized in the implementation file before use.
 */
extern const size_t PORT;

/**
 * @brief The maximum number of pending client connections in the server queue.
 *
 * This constant specifies the limit for how many client connection requests
 * can be queued while waiting to be accepted. Beyond this limit, additional
 * connection attempts may be refused by the operating system.
 *
 * @note Must be initialized in the implementation file before use.
 */
extern const size_t MAX_PENDING_CONNECTIONS;

/**
 * @brief The size of the buffer used for reading data from client sockets.
 *
 * This constant determines the number of bytes allocated for input/output
 * operations involving client communication. It affects memory usage and
 * performance depending on workload.
 *
 * @note Must be initialized in the implementation file before use.
 */
extern const size_t BUFFER_SIZE;

/**
 * @brief Flag indicating whether the server should continue running.
 *
 * This variable is modified by the signal handler when a termination signal is
 * received. It should be checked periodically in long-running loops to
 * determine if shutdown is requested.
 */
extern volatile sig_atomic_t is_server_running;

/**
 * @brief Sets a block of memory to a specified value.
 *
 * This function fills the memory region starting at `dest` with the specified
 * value for `n` bytes. It ensures that the operation cannot be optimized away
 * by the compiler by using volatile pointers. The function prevents unsafe
 * memory manipulations by checking for `NULL` pointers.
 *
 * @param dest The destination buffer to be filled with the specified value.
 * @param value The value to set each byte of the memory block to.
 * @param n The number of bytes to set.
 * @return A pointer to the destination buffer (`dest`).
 * @note If `dest` is `NULL`, the function will return `NULL`.
 */
void* my_memset(void* dest, int value, size_t n);

/**
 * @brief Duplicates a string by allocating memory and copying the string.
 *
 * This function allocates memory for a new string and copies the entire content
 * of the given string into the new memory. The string is null-terminated.
 * If memory allocation fails, the function returns `NULL`.
 *
 * @param s The source string to duplicate.
 * @return A pointer to the newly allocated string, or `NULL` if memory
 * allocation fails.
 * @note If `s` is `NULL`, the function will return `NULL`.
 */
char* my_strdup(const char* s);

/**
 * @brief Copies memory from source to destination.
 *
 * This function copies `n` bytes of data from the `src` buffer to the `dest`
 * buffer. It ensures that overlapping regions are handled safely by copying
 * backwards when necessary. The function also prevents optimization by using
 * volatile pointers.
 *
 * @param dest The destination buffer where the data will be copied to.
 * @param src The source buffer from which the data will be copied.
 * @param n The number of bytes to copy.
 * @return A pointer to the destination buffer (`dest`).
 * @note If either `dest` or `src` is `NULL`, the function will return `NULL`.
 */
void* my_memcpy(void* dest, const void* src, size_t n);

/**
 * @brief Duplicates a substring of a given length.
 *
 * This function allocates memory for a new string and copies up to `n` bytes
 * from the source string `s` into it. It ensures that the string is
 * null-terminated and the memory is properly initialized. If the memory
 * allocation fails, `NULL` is returned.
 *
 * @param s The source string to duplicate.
 * @param n The maximum number of characters to copy from the source string.
 * @return A pointer to the newly allocated string, or `NULL` if memory
 * allocation fails.
 * @note If `s` is `NULL` or `n` is 0, the function will return `NULL`.
 */
char* my_strndup(const char* s, size_t n);

#endif	// COMMON_H

// include/common.h