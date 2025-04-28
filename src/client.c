#include "client.h"

#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common.h"
#include "terminal.h"

const int ROLL_NUM_LENGTH = 8;
const int MAX_NAME_LENGTH = 128;

static int is_client_running = 0;

static char server_ip[32] = {0};

static char roll_num[ROLL_NUM_LENGTH + 1] = {0};
static char name[MAX_NAME_LENGTH + 1] = {0};

static void HandleSignal(int sig) {
	if (sig == SIGINT || sig == SIGTERM) {
		is_client_running = 0;
	}
}

static int ValidateServerIP(const char* server_ip) {
	if (strchr(server_ip, ':') == NULL) {
		return -1;
	}

	const char* p = server_ip;
	while (*p && *p != ':') {
		if (!isdigit(*p) && *p != '.') {
			return -1;
		}
		p++;
	}

	p++;	// skip ':'
	while (*p) {
		if (!isdigit(*p)) {
			return -1;
		}
		p++;
	}

	return 0;
}

static int CheckServerReachability(const char* ip, int port) {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		return -1;	// Not reachable
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
		close(sock);
		return -1;	// Not reachable
	}

	int result =
			connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
	close(sock);

	if (result == 0) {
		return 0;	 // Reachable
	} else {
		return -1;	// Not reachable
	}
}

void InitClient() {
	is_client_running = 1;

	InitTerminalConfig();

	struct sigaction sa;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = HandleSignal;

	if (sigaction(SIGINT, &sa, NULL) == -1) {
		perror("Error: In InitSignalHandlers(): sigaction(SIGINT) failed");
		exit(EXIT_FAILURE);
	}

	if (sigaction(SIGTERM, &sa, NULL) == -1) {
		perror("Error: In InitSignalHandlers(): sigaction(SIGTERM) failed");
		exit(EXIT_FAILURE);
	}

	char server_ip[32] = {0};
	(void)printf("Enter server IP: ");
	if (scanf("%s", server_ip) != 1) {
		perror("Error: In InitClient(): scanf() failed");
		exit(EXIT_FAILURE);
	}

	if (ValidateServerIP(server_ip) != 0) {
		perror("Error: In InitClient(): Invalid server IP format");
		exit(EXIT_FAILURE);
	}

	if (CheckServerReachability(server_ip, PORT) != 0) {
		perror("Error: In InitClient(): Server is not reachable");
		exit(EXIT_FAILURE);
	}
}

RequestType SelectRequestType() {
	int choice = 0;

	while (is_client_running) {
		(void)printf(
				"Select request type:\n"
				"1. GET\n"
				"2. POST\n"
				"3. DELETE\n"
				"Enter your choice: ");

		if (scanf("%d", &choice) != 1) {
			perror("Error: In SelectRequestType(): scanf() failed");
			exit(EXIT_FAILURE);
		}

		switch (choice) {
			case 1:
				return GET;
			case 2:
				return POST;
			case 3:
				return DELETE;
			default:
				(void)printf("Invalid choice. Please try again.\n");
				break;
		}
	}
}

void InputFields(RequestType request) {
	if (request == GET || request == DELETE) {
		(void)printf("Enter roll number: ");
		if (scanf("%s", roll_num) != 1) {
			perror("Error: In InputFields(): scanf() failed");
			exit(EXIT_FAILURE);
		}
	} else if (request == POST) {
		(void)printf("Enter roll number: ");
		if (scanf("%s", roll_num) != 1) {
			perror("Error: In InputFields(): scanf() failed");
			exit(EXIT_FAILURE);
		}

		(void)printf("Enter name: ");
		if (scanf("%s", name) != 1) {
			perror("Error: In InputFields(): scanf() failed");
			exit(EXIT_FAILURE);
		}
	}
}

HTTPResponse* SendRequest(RequestType request_type) {
	HTTPResponse* response = NULL;

	char request[BUFFER_SIZE];
	memset(request, 0, sizeof(request));

	switch (request_type) {
		case GET:
			snprintf(request,
							 sizeof(request),
							 "curl -s http://%s:%zu?roll_num=%s",
							 server_ip,
							 PORT,
							 roll_num);
			break;
		case POST:
			snprintf(request, sizeof(request), "curl")

			break;
		case DELETE:

			break;
		default:
			perror("Error: In SendRequest(): Invalid request type");
			exit(EXIT_FAILURE);
	}

	return response;
}

void Cleanup() {
	is_client_running = 0;
	RevertTerminalConfig();
}

int main() {
	InitClient();

	while (is_client_running) {
		RequestType request = SelectRequestType();
		InputFields(request);
		HTTPResponse* response = SendRequest(request);
		ParseResponse(response);
		FreeHTTPResponse(response);
	}

	Cleanup();
}

// src/client.c