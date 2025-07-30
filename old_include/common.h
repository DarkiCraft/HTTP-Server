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
 * @brief HTTP status code indicating an invalid or unsupported HTTP method.
 *
 * This constant is used to represent an HTTP request with a method
 * that the server does not recognize or support. Typically results
 * in a response indicating client error.
 */
extern const size_t HTTP_INVALID_METHOD;

/**
 * @brief HTTP status code indicating a bad request.
 *
 * This constant represents a malformed HTTP request,
 * such as a missing header or improperly formatted request line.
 * It is used to signal that the server could not understand the request.
 */
extern const size_t HTTP_BAD_REQUEST;

/**
 * @brief HTTP status code indicating that the requested resource was not found.
 *
 * This constant is used when the server cannot locate the requested
 * resource (e.g., URL not mapped to any available handler or data).
 */
extern const size_t HTTP_NOT_FOUND;

/**
 * @brief HTTP status code indicating a successful request.
 *
 * This constant is used to represent a successful HTTP request
 * that has been correctly processed and responded to.
 */
extern const size_t HTTP_OK;

#endif	// COMMON_H

// include/common.h