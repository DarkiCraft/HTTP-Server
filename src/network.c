#include "network.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int CreateServerSocket(int port, int max_connections) {
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (server_socket < 0) {
		perror("Error: In CreateServerSocket(): socket() failed");
		return -1;
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);

	if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) <
			0) {
		perror("Error: In CreateServerSocket(): bind() failed");
		close(server_socket);
		return -1;
	}

	if (listen(server_socket, max_connections) < 0) {
		perror("Error: In CreateServerSocket(): listen() failed");
		close(server_socket);
		return -1;
	}

	return server_socket;
}

int AcceptClient(int server_socket) {
	int client_socket = accept(server_socket, NULL, NULL);
	if (client_socket < 0) {
		perror("Error: In AcceptClient(): accept() failed");
		return -1;
	}

	return client_socket;
}

int CloseServerSocket(int server_fd) {
	if (close(server_fd) < 0) {
		perror("Error: In CloseServerSocket(): close() failed");
		return -1;
	}

	return 0;
}
