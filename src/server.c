#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "network.h"
#include "queue.h"
#include "thread_pool.h"

const size_t PORT = 8080;
const size_t MAX_PENDING_CONNECTIONS = 10;
const size_t BUFFER_SIZE = 1024;

static Queue queue;
static ThreadPool thread_pool;

static void HandleClient(int client_socket) {
	char buffer[BUFFER_SIZE];
	int read_size = 0;

	read_size = (int)read(client_socket, buffer, sizeof(buffer) - 1);
	if (read_size < 0) {
		perror("Error: In HandleClient(): read() failed");
		close(client_socket);
		return;
	}

	buffer[read_size] = '\0';

	HTTPRequest* request = ParseHTTPRequest(buffer);

	if (request == NULL) {
		perror("Error: In HandleClient(): ParseHTTPRequest() failed");
		return;
	}

	printf(
			"Received request:\n"
			"Method: %s\n"
			"Path: %s\n"
			"Body:\n%s\n",
			request->method,
			request->path,
			request->body ? request->body : "No body\n");

	FreeHTTPRequest(request);

	(void)fflush(stdout);

	char* response =
			"HTTP/1.1 200 OK\r\n"						// HTTP response status line
			"Content-Type: text/plain\r\n"	// HTTP response header
			"\r\n"													// End of headers
			"Hello, world!\r\n";						// HTTP response body

	write(client_socket, response, strlen(response));
}

void InitServer() {
	server_socket = CreateServerSocket((int)PORT, (int)MAX_PENDING_CONNECTIONS);

	if (server_socket < 0) {
		perror("Error: In InitServer(): CreateServerSocket() failed");
		exit(EXIT_FAILURE);
	}

	InitQueue(&queue, (int)MAX_PENDING_CONNECTIONS);
	InitThreadPool(&thread_pool, (int)MAX_PENDING_CONNECTIONS, &queue);
}

void RunServer() {
	while (is_server_running) {
		int client_socket = AcceptClient(server_socket);
		if (client_socket < 0) {
			perror("Error: In RunServer(): AcceptClient() failed");
			continue;
		}

		Enqueue(&queue, client_socket);

		// will add worker part later
		int socket_to_handle = Dequeue(&queue);
		if (socket_to_handle >= 0) {
			HandleClient(socket_to_handle);
			close(socket_to_handle);
		}
	}
}

void ShutdownServer() {
	CloseServerSocket(server_socket);
	CleanupQueue(&queue);
	CleanupThreadPool(&thread_pool);
}

int main() {
	InitServer();
	RunServer();
	ShutdownServer();
}
