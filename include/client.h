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
	GET,		// HTTP GET request type
	POST,		// HTTP POST request type
	DELETE	// HTTP DELETE request type
} RequestType;

/**
 * @brief Initializes the client system.
 *
 * This function sets up necessary client-side resources and configurations,
 * preparing for communication with the server. It should be called before
 * any requests are made.
 */
void InitClient();

/**
 * @brief Selects the request type for the HTTP request.
 *
 * This function prompts the user to select one of the supported HTTP request
 * types (GET, POST, DELETE) for communication with the server.
 *
 * @return The selected RequestType (GET, POST, DELETE).
 */
RequestType SelectRequestType();

/**
 * @brief Inputs fields for the selected HTTP request type.
 *
 * This function collects the necessary fields (e.g., key and value for POST)
 * based on the selected request type and prepares the data for sending.
 *
 * @param request The selected HTTP request type.
 */
void InputFields(RequestType request);

/**
 * @brief Sends the HTTP request to the server.
 *
 * This function constructs and sends the HTTP request to the server based on
 * the selected request type. It will handle connection, formatting, and
 * transmission.
 *
 * @param request The selected HTTP request type (GET, POST, DELETE).
 * @return A pointer to an HTTPResponse structure containing the server's
 * response.
 */
HTTPResponse* SendRequest(RequestType request);

/**
 * @brief Parses the server response.
 *
 * This function processes the HTTP response from the server, extracting
 * relevant information such as status codes, error messages, or data, depending
 * on the request.
 */
void ParseResponse();

/**
 * @brief Runs the client application.
 *
 * This function orchestrates the sequence of actions for the client: selecting
 * the request type, inputting fields, sending the request, and parsing the
 * response. It should be called to start the client operation.
 */
void Run();

/**
 * @brief Cleans up client-side resources.
 *
 * This function frees any dynamically allocated memory and resets
 * client-specific configurations to prepare for termination or future use.
 */
void Cleanup();

#endif	// CLIENT_H
