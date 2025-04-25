#include "server.h"

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "database.h"
#include "network.h"
#include "queue.h"
#include "signal_handler.h"
#include "terminal.h"
#include "thread_pool.h"

static Queue queue;
static ThreadPool thread_pool;
static int server_socket;

void InitServer() {
	InitTerminalControlChars();
	InitSignalHandlers(&queue);

	server_socket = CreateServerSocket((int)PORT, (int)MAX_PENDING_CONNECTIONS);

	if (server_socket < 0) {
		(void)fprintf(stderr,
									"Error: In InitServer(): CreateServerSocket() failed\n");
		exit(EXIT_FAILURE);
	}

	InitDatabase();
	InitQueue(&queue, (int)MAX_PENDING_CONNECTIONS);
	InitThreadPool(&thread_pool, (int)MAX_PENDING_CONNECTIONS, &queue);
}

void RunServer() {
	is_server_running = 1;
	while (is_server_running) {
		int client_socket = AcceptClient(server_socket);

		if (!is_server_running) {
			break;
		}

		if (client_socket < 0) {
			(void)fprintf(stderr, "Error: In RunServer(): AcceptClient() failed\n");
			continue;
		}

		Enqueue(&queue, client_socket);
	}
}

void ShutdownServer() {
	CleanupThreadPool(&thread_pool);
	CleanupQueue(&queue);
	CleanupDatabase();
	CloseServerSocket(server_socket);
	CleanupSignalHandlers();
	CleanupTerminalControlChars();

	(void)putchar('\n');
}

int main() {
	InitServer();
	RunServer();
	ShutdownServer();
}

// src/server.c