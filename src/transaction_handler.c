#include "transaction_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "database.h"
#include "parser.h"

static HTTPResponse* HandleInvalidRequest() {
	const char* body =
			"{\r\n"
			"\t\"status\": \"error\",\r\n"
			"\t\"message\": \"Supported methods: GET, POST, PUT, DELETE.\"\r\n"
			"}\r\n";

	char header[BUFFER_SIZE];
	if (snprintf(header,
							 sizeof(header),
							 "Content-Type: application/json\r\n"
							 "Content-Length: %zu\r\n"
							 "Connection: close\r\n",
							 strlen(body)) < 0) {
		(void)fprintf(stderr,
									"Error: In HandleInvalidRequest(): snprintf() failed\n");
		return NULL;
	}

	return CreateHTTPResponse((int)HTTP_INVALID_METHOD, header, body);
}

static HTTPResponse* HandleBadRequest() {
	const char* body =
			"{\r\n"
			"\t\"status\": \"error\",\r\n"
			"\t\"message\": \"Bad request.\"\r\n"
			"}";

	char header[BUFFER_SIZE];
	if (snprintf(header,
							 sizeof(header),
							 "Content-Type: application/json\r\n"
							 "Content-Length: %zu\r\n"
							 "Connection: close\r\n",
							 strlen(body)) < 0) {
		(void)fprintf(stderr, "Error: In HandleBadRequest(): snprintf() failed\n");
		return NULL;
	}

	return CreateHTTPResponse((int)HTTP_BAD_REQUEST, header, body);
}

static HTTPResponse* HandleNotFound() {
	const char* body =
			"{\r\n"
			"\t\"status\": \"error\",\r\n"
			"\t\"message\": \"roll_num not found.\"\r\n"
			"}\r\n";

	char header[BUFFER_SIZE];
	if (snprintf(header,
							 sizeof(header),
							 "Content-Type: application/json\r\n"
							 "Content-Length: %zu\r\n"
							 "Connection: close\r\n",
							 strlen(body)) < 0) {
		(void)fprintf(stderr, "Error: In HandleNotFound(): snprintf() failed\n");
		return NULL;
	}

	return CreateHTTPResponse((int)HTTP_NOT_FOUND, header, body);
}

static HTTPResponse* HandleGET(HTTPRequest* request) {
	char* question_mark = strchr(request->path, '?');
	if (question_mark == NULL) {
		(void)fprintf(
				stderr,
				"Error: In HandleGET(): Invalid request path: '?' not present\n");
		return HandleBadRequest();
	}

	char* path = question_mark + 1;

	char* equal_to = strchr(path, '=');
	if (equal_to == NULL) {
		(void)fprintf(
				stderr,
				"Error: In HandleGET(): Invalid request path: '=' not present\n");
		return HandleBadRequest();
	}

	*equal_to = '\0';
	if (strcmp(path, "roll_num") != 0) {
		(void)fprintf(
				stderr, "Error: In HandleGET(): Invalid key: %s\n", request->path);
		return HandleBadRequest();
	}

	char* after_equal_to = equal_to + 1;

	char* name = DatabaseGet(after_equal_to);

	if (name == NULL) {
		(void)fprintf(stderr, "Error: In HandleGET(): DatabaseGet() failed\n");
		return HandleNotFound();
	}

	char body[BUFFER_SIZE];
	if (snprintf(body,
							 sizeof(body),
							 "{\r\n"
							 "\t\"status\": \"success\",\r\n"
							 "\t\"roll_num\": \"%s\",\r\n"
							 "\t\"name\": \"%s\"\r\n"
							 "}\r\n",
							 after_equal_to,
							 name) < 0) {
		(void)fprintf(stderr, "Error: In HandleGET(): snprintf() failed\n");
	}

	char header[BUFFER_SIZE];
	if (snprintf(header,
							 sizeof(header),
							 "Content-Type: application/json\r\n"
							 "Content-Length: %zu\r\n"
							 "Connection: close\r\n",
							 strlen(body)) < 0) {
		(void)fprintf(stderr, "Error: In HandleGET(): snprintf() failed\n");
	}

	free(name);

	return CreateHTTPResponse((int)HTTP_OK, header, body);
}

static HTTPResponse* HandlePOST(HTTPRequest* request) {
	if (strstr(request->headers, "Content-Type: application/json") == NULL) {
		(void)fprintf(
				stderr,
				"Error: In HandlePOST(): Content-Type is not application/json\n");
		return HandleBadRequest();
	}

	char* roll_num_key = strstr(request->body, "\"roll_num\"");
	char* name_key = strstr(request->body, "\"name\"");

	if (roll_num_key == NULL || name_key == NULL) {
		(void)fprintf(stderr,
									"Error: In HandlePOST(): Missing roll_num or name in body\n");
		return HandleBadRequest();
	}

	char* roll_num_start = strchr(roll_num_key, ':');
	if (roll_num_start == NULL) {
		(void)fprintf(stderr, "Error: In HandlePOST(): Invalid roll_num format\n");
		return HandleBadRequest();
	}
	roll_num_start++;
	while (*roll_num_start == ' ' || *roll_num_start == '\"') {
		roll_num_start++;
	}

	char* roll_num_end = strchr(roll_num_start, '\"');
	if (roll_num_end == NULL) {
		(void)fprintf(stderr, "Error: In HandlePOST(): Invalid roll_num value\n");
		return HandleBadRequest();
	}
	*roll_num_end = '\0';

	char* name_start = strchr(name_key, ':');
	if (name_start == NULL) {
		(void)fprintf(stderr, "Error: In HandlePOST(): Invalid name format\n");
		return HandleBadRequest();
	}
	name_start++;
	while (*name_start == ' ' || *name_start == '\"') {
		name_start++;
	}

	char* name_end = strchr(name_start, '\"');
	if (name_end == NULL) {
		(void)fprintf(stderr, "Error: In HandlePOST(): Invalid name value\n");
		return HandleBadRequest();
	}
	*name_end = '\0';

	if (DatabasePost(roll_num_start, name_start) == 0) {
		(void)fprintf(stderr, "Error: In HandlePOST(): DatabasePost() failed\n");
		return HandleBadRequest();
	}

	char body[BUFFER_SIZE];
	if (snprintf(body,
							 sizeof(body),
							 "{\r\n"
							 "\t\"status\": \"success\",\r\n"
							 "\t\"message\": \"Record added successfully.\"\r\n"
							 "}\r\n") < 0) {
		(void)fprintf(stderr, "Error: In HandlePOST(): snprintf() failed\n");
	}

	char header[BUFFER_SIZE];
	if (snprintf(header,
							 sizeof(header),
							 "Content-Type: application/json\r\n"
							 "Content-Length: %zu\r\n"
							 "Connection: close\r\n",
							 strlen(body)) < 0) {
		(void)fprintf(stderr, "Error: In HandlePOST(): snprintf() failed\n");
	}

	return CreateHTTPResponse((int)HTTP_OK, header, body);
}

static HTTPResponse* HandleDELETE(HTTPRequest* request) {
	char* question_mark = strchr(request->path, '?');
	if (question_mark == NULL) {
		(void)fprintf(
				stderr,
				"Error: In HandleDELETE(): Invalid request path: '?' not present\n");
		return HandleBadRequest();
	}

	char* path = question_mark + 1;

	char* equal_to = strchr(path, '=');
	if (equal_to == NULL) {
		(void)fprintf(
				stderr,
				"Error: In HandleDELETE(): Invalid request path: '=' not present\n");
		return HandleBadRequest();
	}

	*equal_to = '\0';
	if (strcmp(path, "roll_num") != 0) {
		(void)fprintf(
				stderr, "Error: In HandleDELETE(): Invalid key: %s\n", request->path);
		return HandleBadRequest();
	}

	char* after_equal_to = equal_to + 1;

	if (DatabaseDelete(after_equal_to) == 0) {
		(void)fprintf(stderr,
									"Error: In HandleDELETE(): DatabaseDelete() failed\n");
		return HandleNotFound();
	}

	char body[BUFFER_SIZE];
	if (snprintf(body,
							 sizeof(body),
							 "{\r\n"
							 "\t\"status\": \"success\",\r\n"
							 "\t\"message\": \"roll_num deleted successfully.\"\r\n"
							 "}\r\n") < 0) {
		(void)fprintf(stderr, "Error: In HandleDELETE(): snprintf() failed\n");
	}

	char header[BUFFER_SIZE];
	if (snprintf(header,
							 sizeof(header),
							 "Content-Type: application/json\r\n"
							 "Content-Length: %zu\r\n"
							 "Connection: close\r\n",
							 strlen(body)) < 0) {
		(void)fprintf(stderr, "Error: In HandleDELETE(): snprintf() failed\n");
	}

	return CreateHTTPResponse((int)HTTP_OK, header, body);
}

typedef enum { GET, POST, DELETE, INVALID } HTTPMethod;

static HTTPMethod GetHTTPMethod(const char* method) {
	if (strcmp(method, "GET") == 0) {
		return GET;
	}

	if (strcmp(method, "POST") == 0) {
		return POST;
	}

	if (strcmp(method, "DELETE") == 0) {
		return DELETE;
	}

	return INVALID;
}

void HandleTransaction(int client_socket) {
	char buffer[BUFFER_SIZE];
	int read_size = 0;

	read_size = (int)read(client_socket, buffer, sizeof(buffer) - 1);
	if (read_size < 0) {
		(void)fprintf(stderr, "Error: In HandleTransaction(): read() failed\n");
		return;
	}

	buffer[read_size] = '\0';

	HTTPRequest* request = ParseHTTPRequest(buffer);

	if (request == NULL) {
		(void)fprintf(stderr,
									"Error: In HandleTransaction(): ParseHTTPRequest() failed\n");
		return;
	}

	HTTPResponse* response = NULL;

	HTTPMethod method = GetHTTPMethod(request->method);

	switch (method) {
		case GET:
			response = HandleGET(request);
			break;
		case POST:
			response = HandlePOST(request);
			break;
		case DELETE:
			response = HandleDELETE(request);
			break;
		case INVALID:
			response = HandleInvalidRequest();
			break;
	}

	char response_buffer[BUFFER_SIZE];
	if (snprintf(response_buffer,
							 sizeof(response_buffer),
							 "HTTP/1.1 %d\r\n"
							 "%s\r\n"
							 "%s",
							 response->status_code,
							 response->headers,
							 response->body) < 0) {
		(void)fprintf(stderr, "Error: In HandleTransaction(): snprintf() failed\n");
	}

	if (write(client_socket, response_buffer, strlen(response_buffer)) < 0) {
		(void)fprintf(stderr, "Error: In HandleTransaction(): write() failed\n");
	}

	FreeHTTPRequest(request);
	FreeHTTPResponse(response);
}

// src/transaction_handler.c