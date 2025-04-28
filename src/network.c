#include "network.h"

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"

static void PrintLocalIP() {
	struct ifaddrs* ifaddr = NULL;
	struct ifaddrs* ifa = NULL;
	char ip_str[INET_ADDRSTRLEN];

	if (getifaddrs(&ifaddr) == -1) {
		perror("Error: getifaddrs() failed");
		return;
	}

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr->sa_family == AF_INET) {
			void* addr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
			inet_ntop(AF_INET, addr, ip_str, INET_ADDRSTRLEN);

			if (ifa->ifa_name[0] != 'l') {
				printf("Server listening on: %s\n", ip_str);
				break;
			}
		}
	}

	freeifaddrs(ifaddr);
}

int CreateServerSocket(int port, int max_connections) {
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (server_socket < 0) {
		(void)fprintf(stderr, "Error: In CreateServerSocket(): socket() failed\n");
		return -1;
	}

	int opt = 1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) <
			0) {
		perror("Error: In CreateServerSocket(): setsockopt() failed");
		close(server_socket);
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

	PrintLocalIP();

	if (listen(server_socket, max_connections) < 0) {
		perror("Error: In CreateServerSocket(): listen() failed");
		close(server_socket);
		return -1;
	}

	return server_socket;
}

int AcceptClient(int server_socket) {
	int client_socket = accept(server_socket, NULL, NULL);

	if (!is_server_running) {
		return -1;
	}

	if (client_socket < 0) {
		(void)fprintf(stderr, "Error: In AcceptClient(): accept() failed\n");
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

// src/network.c