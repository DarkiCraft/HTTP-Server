#include "parser.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

HTTPRequest* ParseHTTPRequest(const char* raw_request) {
	if (raw_request == NULL) {
		perror("Error: In ParseHTTPRequest(): raw_request is NULL");
		return NULL;
	}

	HTTPRequest* request = calloc(1, sizeof(HTTPRequest));
	if (request == NULL) {
		return NULL;
	}

	const char* header_end = strstr(raw_request, "\r\n\r\n");
	size_t header_len =
			(header_end) ? (size_t)(header_end - raw_request) : strlen(raw_request);

	char* raw_request_copy = my_strndup(raw_request, header_len);
	if (raw_request_copy == NULL) {
		perror("Error: In ParseHTTPRequest(): my_strndup() failed");
		free(request);
		return NULL;
	}

	char* line = strtok(raw_request_copy, "\r\n");
	if (line == NULL) {
		perror("Error: In ParseHTTPRequest(): strtok() failed");
		free(raw_request_copy);
		free(request);
		return NULL;
	}

	char* method = strtok(line, " ");
	if (method == NULL) {
		perror("Error: In ParseHTTPRequest(): strtok() failed");
		free(raw_request_copy);
		free(request);
		return NULL;
	}

	char* path = strtok(NULL, " ");
	if (path == NULL) {
		perror("Error: In ParseHTTPRequest(): strtok() failed");
		free(raw_request_copy);
		free(request);
		return NULL;
	}

	if (method != NULL) {
		request->method = my_strdup(method);
		if (request->method == NULL) {
			perror("Error: In ParseHTTPRequest(): my_strdup() failed");
			free(raw_request_copy);
			free(request);
			return NULL;
		}
	}

	request->path = my_strdup(path);
	if (request->path == NULL) {
		perror("Error: In ParseHTTPRequest(): my_strdup() failed");
		free(raw_request_copy);
		free(request);
		return NULL;
	}

	request->headers =
			my_strndup(raw_request + strlen(line) + 2, header_len - strlen(line) - 2);

	if (header_end != NULL) {
		request->body = my_strdup(header_end + 4);
	}

	free(raw_request_copy);
	return request;
}

void FreeHTTPRequest(HTTPRequest* request) {
	if (request == NULL) {
		return;
	}

	free(request->method);
	free(request->path);
	free(request->headers);
	free(request->body);
	free(request);
}

HTTPResponse* CreateHTTPResponse(int status_code,
																 const char* headers,
																 const char* body) {
	HTTPResponse* response = (HTTPResponse*)malloc(sizeof(HTTPResponse));
	if (response == NULL) {
		perror("Error: In CreateHTTPResponse(): malloc() failed");
		return NULL;
	}

	response->status_code = status_code;

	if (headers != NULL) {
		response->headers = my_strdup(headers);
		if (response->headers == NULL) {
			perror("Error: In CreateHTTPResponse(): my_strdup() failed");
			free(response);
			return NULL;
		}
	} else {
		response->headers = NULL;
	}

	if (body != NULL) {
		response->body = my_strdup(body);
		if (response->body == NULL) {
			perror("Error: In CreateHTTPResponse(): my_strdup() failed");
			free(response->headers);
			free(response);
			return NULL;
		}
	} else {
		response->body = NULL;
	}

	return response;
}

void FreeHTTPResponse(HTTPResponse* response) {
	if (response != NULL) {
		if (response->headers != NULL) {
			free(response->headers);
		}
		if (response->body != NULL) {
			free(response->body);
		}

		free(response);
	}
}

// src/parser.c