#ifndef PARSER_H
#define PARSER_H

/**
 * @struct HTTPRequest
 * @brief Represents a parsed HTTP request.
 *
 * This structure breaks down the HTTP request into its method, path, headers,
 * and body.
 */
typedef struct {
	char* method;		// HTTP method (e.g., "GET", "POST")
	char* path;			// Request path (e.g., "/api/data")
	char* headers;	// Full headers as a string
	char* body;			// HTTP request body
} HTTPRequest;

/**
 * @struct HTTPResponse
 * @brief Represents an HTTP response to be sent to the client.
 *
 * This structure encapsulates the essential components of an HTTP response:
 * status code, headers, and body. It is dynamically created and should be freed
 * after being sent to avoid memory leaks.
 */
typedef struct {
	int status_code;	// The HTTP status code (e.g., 200, 404, 405).
	char* headers;		// The headers section of the response (NULL-able)
	char* body;				// The body of the response (NULL-able).
} HTTPResponse;

/**
 * @brief Parses the raw HTTP request string into a structured HTTPRequest
 * object.
 *
 * This function tokenizes the HTTP request and extracts the method, path,
 * headers, and body.
 *
 * @param raw_request The raw HTTP request string.
 * @return A dynamically allocated HTTPRequest structure. Must be freed after
 * use. NULL on error
 */
HTTPRequest* ParseHTTPRequest(const char* raw_request);

/**
 * @brief Frees the memory associated with an HTTPRequest structure.
 *
 * This includes all dynamically allocated strings inside the structure.
 *
 * @param request A pointer to the HTTPRequest structure to be freed.
 */
void FreeHTTPRequest(HTTPRequest* request);

/**
 * @brief Constructs an HTTPResponse object.
 *
 * Helper to easily create an HTTP response with status, headers, and body.
 *
 * @param status_code HTTP status code.
 * @param headers HTTP headers string (NULL-able).
 * @param body HTTP body string (NULL-able).
 * @return A dynamically allocated HTTPResponse. Must be freed after use.
 */
HTTPResponse* CreateHTTPResponse(int status_code,
																 const char* headers,
																 const char* body);

/**
 * @brief Parses a raw HTTP response into an HTTPResponse structure.
 *
 * This function tokenizes the HTTP response and extracts the status code,
 *
 * @param raw_response A string containing the raw HTTP response to parse.
 * @return A pointer to an HTTPResponse structure containing the parsed data, or
 * NULL on error.
 */
HTTPResponse* ParseHTTPResponse(const char* raw_response);

/**
 * @brief Frees the memory associated with an HTTPResponse structure.
 *
 * @param response A pointer to the HTTPResponse structure to be freed.
 */
void FreeHTTPResponse(HTTPResponse* response);

#endif	// PARSER_H

// include/parser.h