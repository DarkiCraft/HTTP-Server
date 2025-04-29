#ifndef CLIENT_H
#define CLIENT_H

#include "parser.h"

/**
 * @enum RequestType
 * @brief Enumeration for HTTP request types.
 *
 * This enumeration defines the supported HTTP request types for communication.
 * The server will handle these types of requests.
 */
typedef enum {
	GET,		 // HTTP GET request type
	POST,		 // HTTP POST request type
	DELETE,	 // HTTP DELETE request type
	INVALID	 // Invalid request type
} RequestType;

void InitClient();

RequestType SelectRequestType();

int InputFields(RequestType request);

char* SendRequest(RequestType request);

void Cleanup();

#endif	// CLIENT_H
