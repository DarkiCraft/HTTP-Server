#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

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

extern int server_socket;
extern int is_server_running;

#endif	// COMMON_H

// include/common.h