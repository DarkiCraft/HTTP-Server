#include "client.h"

#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
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
const int MAX_STATUS_LENGTH = 32;
const int MAX_MESSAGE_LENGTH = 256;

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
	}

	return -1;	// Not reachable
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

	if (CheckServerReachability(server_ip, (int)PORT) != 0) {
		(void)fprintf(stderr, "Error: In InitClient(): Server is not reachable\n");
		exit(EXIT_FAILURE);
	}

	printf("Successfully connected.\n\n");
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

		errno = 0;
		if (scanf("%d", &choice) != 1) {
			if (errno != EINTR) {
				perror("Error: In SelectRequestType(): scanf() failed");
			}
			return INVALID;
		}
		(void)putchar('\n');

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

int InputFields(RequestType request) {
	if (request == GET || request == DELETE) {
		(void)printf("Enter roll number (YYA-DDDD): ");
		errno = 0;
		if (scanf("%8s", roll_num) != 1) {
			if (errno != EINTR) {
				perror("Error: In InputFields(): scanf() failed");
			}
			return -1;
		}

	} else if (request == POST) {
		(void)printf("Enter roll number: ");
		errno = 0;
		if (scanf("%8s", roll_num) != 1) {
			if (errno != EINTR) {
				perror("Error: In InputFields(): scanf() failed");
			}
			return -1;
		}

		(void)getchar();
		(void)printf("Enter name: ");
		errno = 0;
		if (fgets(name, MAX_NAME_LENGTH, stdin) == NULL) {
			if (errno != EINTR) {
				perror("Error: In InputFields(): fgets() failed");
			}
			return -1;
		}

		size_t len = strlen(name);
		if (len > 0 && name[len - 1] == '\n') {
			name[len - 1] = '\0';
		}
	}

	return 0;
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
				return NULL;
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
				return NULL;
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
				return NULL;
			}
			break;
		default:
			return NULL;
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

void ExtractField(const char* body,
									const char* field_name,
									char* output,
									size_t output_size) {
	char* pos = strstr(body, field_name);
	if (pos) {
		pos = strchr(pos, ':');
		if (pos) {
			pos += 1;
			while (*pos == ' ' || *pos == '\"') pos++;

			size_t i = 0;
			while (*pos && *pos != '\"' && *pos != ',' && *pos != '\n' &&
						 i < output_size - 1) {
				output[i++] = *pos++;
			}
			output[i] = '\0';
		}
	} else {
		output[0] = '\0';
	}
}

void PrintResponse(const HTTPResponse* response) {
	if (response == NULL) {
		printf("NULL response\n");
		return;
	}

	printf("Status Code: %d\n", response->status_code);

	if (response->body == NULL) {
		printf("Empty body.\n");
		return;
	}

	char status[MAX_STATUS_LENGTH];
	char message[MAX_MESSAGE_LENGTH];
	char roll_num[ROLL_NUM_LENGTH];
	char name[MAX_NAME_LENGTH];

	memset(status, 0, sizeof(status));
	memset(message, 0, sizeof(message));
	memset(roll_num, 0, sizeof(roll_num));
	memset(name, 0, sizeof(name));

	ExtractField(response->body, "status", status, sizeof(status));
	ExtractField(response->body, "message", message, sizeof(message));
	ExtractField(response->body, "roll_num", roll_num, sizeof(roll_num));
	ExtractField(response->body, "name", name, sizeof(name));

	printf("Status: %s\n", status);

	if (roll_num[0] != '\0' && name[0] != '\0') {
		printf("Roll Number: %s\n", roll_num);
		printf("Name: %s\n", name);
	} else if (message[0] != '\0') {
		printf("Message: %s\n", message);
	} else {
		printf("No additional information.\n");
	}
}

void Cleanup() {
	RevertTerminalConfig();

	free(server_ip);
	free(roll_num);
	free(name);

	(void)putchar('\n');
}

int main() {
	InitClient();

	while (is_client_running) {
		RequestType request = SelectRequestType();
		if (InputFields(request) != 0 && is_client_running) {
			(void)fprintf(stderr, "Error: In main(): InputFields() failed");
			continue;
		}

		char* raw_response = SendRequest(request);
		if (raw_response == NULL && is_client_running) {
			(void)fprintf(stderr, "Error: In main(): SendRequest() failed");
			free(raw_response);
			continue;
		}

		if (!is_client_running) {
			free(raw_response);
			break;
		}

		HTTPResponse* response = ParseHTTPResponse(raw_response);
		if (response == NULL && is_client_running) {
			(void)fprintf(stderr, "Error: In main(): ParseHTTPResponse() failed");
		} else {
			(void)printf("Server responded:\n\n");
			PrintResponse(response);
			(void)putchar('\n');
		}

		free(raw_response);
		FreeHTTPResponse(response);
	}

	Cleanup();
}

// src/client.c