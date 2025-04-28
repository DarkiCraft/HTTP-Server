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

	char* raw_request_copy = strndup(raw_request, header_len);
	if (raw_request_copy == NULL) {
		perror("Error: In ParseHTTPRequest(): strndup() failed");
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
		request->method = strdup(method);
		if (request->method == NULL) {
			perror("Error: In ParseHTTPRequest(): strdup() failed");
			free(raw_request_copy);
			free(request);
			return NULL;
		}
	}

	request->path = strdup(path);
	if (request->path == NULL) {
		perror("Error: In ParseHTTPRequest(): strdup() failed");
		free(raw_request_copy);
		free(request);
		return NULL;
	}

	request->headers =
			strndup(raw_request + strlen(line) + 2, header_len - strlen(line) - 2);

	if (header_end != NULL) {
		request->body = strdup(header_end + 4);
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
		response->headers = strdup(headers);
		if (response->headers == NULL) {
			perror("Error: In CreateHTTPResponse(): strdup() failed");
			free(response);
			return NULL;
		}
	} else {
		response->headers = NULL;
	}

	if (body != NULL) {
		response->body = strdup(body);
		if (response->body == NULL) {
			perror("Error: In CreateHTTPResponse(): strdup() failed");
			free(response->headers);
			free(response);
			return NULL;
		}
	} else {
		response->body = NULL;
	}

	return response;
}

HTTPResponse* ParseHTTPResponse(const char* raw_response) {
	if (raw_response == NULL) {
		perror("Error: In ParseHTTPResponse(): raw_response is NULL");
		return NULL;
	}

	HTTPResponse* response = (HTTPResponse*)malloc(sizeof(HTTPResponse));
	if (response == NULL) {
		perror("Error: malloc() failed for HTTPResponse");
		return NULL;
	}
	memset(response, 0, sizeof(HTTPResponse));

	char* line_end = strstr(raw_response, "\r\n");
	if (line_end == NULL) {
		perror(
				"Error: In ParseHTTPResponse(): Invalid response format (no header "
				"line found)");
		free(response);
		return NULL;
	}

	size_t status_line_len = line_end - raw_response;
	char* status_line = strndup(raw_response, status_line_len);
	if (status_line == NULL) {
		perror("Error: In ParseHTTPResponse(): strndup() failed for status line");
		free(response);
		return NULL;
	}

	int status_code = 0;
	if (sscanf(status_line, "HTTP/1.1 %d", &status_code) != 1) {
		perror("Error: In ParseHTTPResponse(): Failed to extract status code");
		free(status_line);
		free(response);
		return NULL;
	}
	response->status_code = status_code;
	free(status_line);

	char* header_end = strstr(raw_response, "\r\n\r\n");
	if (header_end == NULL) {
		perror("Error: In ParseHTTPResponse(): No headers end found");
		free(response);
		return NULL;
	}

	size_t header_len = header_end - raw_response;
	response->headers = strndup(raw_response + status_line_len + 2,
															header_len - status_line_len - 2);
	if (response->headers == NULL) {
		perror("Error: In ParseHTTPResponse(): strndup() failed for headers");
		free(response);
		return NULL;
	}

	response->body = strdup(header_end + 4);
	if (response->body == NULL) {
		perror("Error: In ParseHTTPResponse(): strdup() failed for body");
		free(response->headers);
		free(response);
		return NULL;
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