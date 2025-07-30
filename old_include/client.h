#ifndef CLIENT_H
#define CLIENT_H

#include "parser.h"

/**
 * @enum RequestType
 * @brief Enumeration for HTTP request types.
 *
 * Defines the supported HTTP request types used for client-server
 * communication. The server processes these types to perform corresponding
 * operations.
 */
typedef enum {
	GET,		 // HTTP GET request type
	POST,		 // HTTP POST request type
	DELETE,	 // HTTP DELETE request type
	INVALID	 // Invalid or unrecognized request type
} RequestType;

/**
 * @brief Initializes the client interface.
 *
 * Prepares the client application for operation. Typically sets up any
 * necessary resources before user interaction begins.
 */
void InitClient();

/**
 * @brief Prompts the user to select an HTTP request type.
 *
 * Displays a menu or options to the user and returns the selected request type.
 *
 * @return RequestType Selected HTTP request type (GET, POST, DELETE, or
 * INVALID).
 */
RequestType SelectRequestType();

/**
 * @brief Accepts user input for fields required by the selected request type.
 *
 * Prompts the user to provide data based on the type of request (e.g.,
 * key-value pairs for POST, key for GET/DELETE).
 *
 * @param request The HTTP request type that determines the required input
 * fields.
 * @return int Returns 0 on success, or a non-zero value if input was invalid.
 */
int InputFields(RequestType request);

/**
 * @brief Sends the constructed request to the server and returns the response.
 *
 * Constructs and sends the HTTP request based on user input, then receives the
 * server’s response.
 *
 * @param request The type of HTTP request to send.
 * @return char* Pointer to a dynamically allocated string containing the
 * server’s response. The caller is responsible for freeing the memory.
 */
char* SendRequest(RequestType request);

/**
 * @brief Cleans up client resources.
 *
 * Frees any dynamically allocated memory or resources before the client exits.
 * Should be called before terminating the program.
 */
void Cleanup();

#endif	// CLIENT_H