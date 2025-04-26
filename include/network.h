#ifndef NETWORK_H
#define NETWORK_H

#include "common.h"
#include "parser.h"

/**
 * @brief Initializes the server socket.
 *
 * This function creates a socket, binds it to the specified port, and prepares
 * it for listening to incoming client connections.
 *
 * @param port The port on which the server will listen.
 * @param max_connections The maximum number of pending connections.
 * @return The server socket file descriptor (fd), -1 on error.
 */
int CreateServerSocket(int port, int max_connections);

/**
 * @brief Accepts a client connection.
 *
 * This function accepts an incoming client connection and returns the client
 * socket descriptor, allowing communication with the client.
 *
 * @param server_socket The server socket file descriptor.
 * @return The client socket file descriptor, -1 on error.
 */
int AcceptClient(int server_socket);

/**
 * @brief Closes the server socket.
 *
 * This function closes the specified server socket file descriptor,
 * freeing the associated resources.
 *
 * @param server_fd The server socket file descriptor to close.
 * @return 0 on success, -1 on error.
 */
int CloseServerSocket(int server_fd);

#endif	// NETWORK_H

// include/network.h