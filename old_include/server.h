#ifndef SERVER_H
#define SERVER_H

/**
 * @brief Initializes all server subsystems
 */
void InitServer();

/**
 * @brief Starts the server loop and handles incoming connections.
 *
 * This function blocks and continuously accepts new client connections,
 * passing them to the thread pool for processing.
 */
void RunServer();

/**
 * @brief Shuts down the server and frees all resources.
 */
void ShutdownServer();

#endif	// SERVER_H

// include/server.h