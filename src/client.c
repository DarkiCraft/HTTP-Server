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

const int MAX_SERVER_IP_LENGTH = 32;

const int ROLL_NUM_LENGTH = 8;
const int MAX_NAME_LENGTH = 128;

static int is_client_running = 0;

static char* server_ip = NULL;
static char* roll_num = NULL;
static char* name = NULL;

static void HandleSignal(int sig) {
	if (sig == SIGINT || sig == SIGTERM) {
		is_client_running = 0;
	}
}

static int ValidateServerIP(const char* server_ip) {
	const char* p = server_ip;
	while (*p && *p != ':') {
		if (!isdigit(*p) && *p != '.') {
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

	server_ip = (char*)malloc(MAX_SERVER_IP_LENGTH + 1);
	roll_num = (char*)malloc(ROLL_NUM_LENGTH + 1);
	name = (char*)malloc(MAX_NAME_LENGTH + 1);

	if (roll_num == NULL || name == NULL || server_ip == NULL) {
		perror("Error: In InitClient(): malloc() failed");
		exit(EXIT_FAILURE);
	}

	(void)memset(server_ip, 0, MAX_SERVER_IP_LENGTH);
	(void)memset(roll_num, 0, ROLL_NUM_LENGTH);
	(void)memset(name, 0, MAX_NAME_LENGTH);

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

	(void)printf("Enter server IP: ");
	if (scanf("%s", server_ip) != 1) {
		perror("Error: In InitClient(): scanf() failed");
		exit(EXIT_FAILURE);
	}

	if (ValidateServerIP(server_ip) != 0) {
		(void)fprintf(stderr, "Error: In InitClient(): Invalid server IP format\n");
		exit(EXIT_FAILURE);
	}

	if (CheckServerReachability(server_ip, PORT) != 0) {
		(void)fprintf(stderr, "Error: In InitClient(): Server is not reachable\n");
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

	return INVALID;	 // unreachable
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
		if (fgets(name, MAX_NAME_LENGTH, stdin) == NULL) {
			perror("Error: In InputFields(): fgets() failed");
			exit(EXIT_FAILURE);
		}

		size_t len = strlen(name);
		if (len > 0 && name[len - 1] == '\n') {
			name[len - 1] = '\0';	 // Remove newline character
		}
	}
}

char* SendRequest(RequestType request_type) {
	char request[BUFFER_SIZE];
	memset(request, 0, sizeof(request));

	switch (request_type) {
		case GET:
			if (snprintf(request,
									 sizeof(request),
									 "curl -i -s http://%s:%zu?roll_num=%s",
									 server_ip,
									 PORT,
									 roll_num) < 0) {
				perror("Error: In SendRequest(): snprintf() failed");
				exit(EXIT_FAILURE);
			}
			break;
		case POST:
			if (snprintf(request,
									 sizeof(request),
									 "curl -i -s http://%s:%zu "
									 "-H \"Content-Type: application/json\" "
									 "-d '{\"roll_num\":\"%s\", \"name\":\"%s\"}'",
									 server_ip,
									 PORT,
									 roll_num,
									 name) < 0) {
				perror("Error: In SendRequest(): snprintf() failed");
				exit(EXIT_FAILURE);
			}
			break;
		case DELETE:
			if (snprintf(request,
									 sizeof(request),
									 "curl -i -s http://%s:%zu?roll_num=%s -X DELETE",
									 server_ip,
									 PORT,
									 roll_num) < 0) {
				perror("Error: In SendRequest(): snprintf() failed");
				exit(EXIT_FAILURE);
			}
			break;
		default:
			perror("Error: In SendRequest(): Invalid request type");
			exit(EXIT_FAILURE);
	}

	FILE* fp = popen(request, "r");
	if (fp == NULL) {
		perror("Error: In SendRequest(): popen() failed");
		return NULL;
	}

	char response[BUFFER_SIZE];
	memset(response, 0, sizeof(response));

	size_t total_read = fread(response, 1, sizeof(response) - 1, fp);
	response[total_read] = '\0';

	if (ferror(fp)) {
		perror("Error: In SendRequest(): fread() failed");
		pclose(fp);
		return NULL;
	}

	char* response_copy = (char*)malloc(strlen(response) + 1);
	if (response_copy == NULL) {
		perror("Error: In SendRequest(): malloc() failed");
		pclose(fp);
		return NULL;
	}

	strncpy(response_copy, response, total_read);
	response_copy[total_read] = '\0';

	if (pclose(fp) == -1) {
		perror("Error: In SendRequest(): pclose() failed");
		free(response_copy);
		return NULL;
	}

	return response_copy;
}

void Cleanup() {
	RevertTerminalConfig();

	free(server_ip);
	free(roll_num);
	free(name);
}

int main() {
	InitClient();

	while (is_client_running) {
		RequestType request = SelectRequestType();
		InputFields(request);
		char* raw_response = SendRequest(request);
		if (raw_response == NULL) {
			(void)fprintf(stderr, "Error: In main(): SendRequest() failed");
			continue;
		}
		HTTPResponse* response = ParseHTTPResponse(raw_response);
		// Print(response);
		FreeHTTPResponse(response);
	}

	Cleanup();
}

// src/client.c