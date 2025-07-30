#ifndef TRANSACTION_HANDLER_H
#define TRANSACTION_HANDLER_H

#include "parser.h"

/**
 * @brief Handles an incoming HTTP request and generates an appropriate
 * response.
 *
 * This function is responsible for processing an incoming HTTP request, which
 * may be a GET, POST, or DELETE request. It processes the request, interacts
 * with the database as necessary, and constructs an HTTP response to be sent
 * back to the client. If the method is not supported, a 405 Method Not Allowed
 * response is returned.
 *
 * @param client_socket The socket descriptor for the client connection. This is
 * used to read the incoming request and send the response back to the client.
 */
void HandleTransaction(int client_socket);

#endif	// TRANSACTION_HANDLER_H

// include/transaction_handler.h