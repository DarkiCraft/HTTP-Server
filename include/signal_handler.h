#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <signal.h>

#include "queue.h"

/**
 * @brief Initializes signal handling for graceful server shutdown.
 *
 * Sets up handlers for signals like SIGINT using sigaction().
 * The handler updates the is_server_running flag to indicate termination
 * request.
 *
 * @param queue Pointer to the Queue structure used for thread communication.
 */
void InitSignalHandlers(Queue* queue);

/**
 * @brief Signal handler function for handling termination signals.
 *
 * This function is called when a registered signal is received (e.g., SIGINT).
 * It sets the is_server_running flag to 0, indicating the server should begin
 * shutdown.
 *
 * @param sig The signal number that triggered the handler.
 */
void HandleSignal(int sig);

/**
 * @brief Cleans up signal handlers and associated resources.
 *
 * This function should be called before the program exits to ensure proper
 * cleanup of signal handling resources.
 */
void CleanupSignalHandlers();

#endif	// SIGNAL_HANDLER_H

// include/signal_handler.h