#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_PENDING 5

void handle_client(int client_socket)
{
	char buffer[1024];
	int read_size;

	// Read the HTTP request
	read_size = read(client_socket, buffer, sizeof(buffer) - 1);
	if (read_size < 0)
	{
		perror("Error reading from socket");
		close(client_socket);
		return;
	}

	buffer[read_size] = '\0'; // Null-terminate the buffer

	// Print the HTTP request (optional for debugging)
	printf("Received request:\n%s\n", buffer);

	// Create a simple HTTP response
	char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, world!";

	// Send the HTTP response
	write(client_socket, response, strlen(response));

	// Close the connection
	close(client_socket);
}

int main()
{
	int server_socket, client_socket;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len = sizeof(client_addr);

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		perror("Error creating socket");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Error binding socket");
		close(server_socket);
		exit(EXIT_FAILURE);
	}

	if (listen(server_socket, MAX_PENDING) < 0)
	{
		perror("Error listening on socket");
		close(server_socket);
		exit(EXIT_FAILURE);
	}

	printf("Server is listening on port %d...\n", PORT);

	while (1)
	{
		client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
		if (client_socket < 0)
		{
			perror("Error accepting connection");
			continue;
		}
		handle_client(client_socket);
	}

	close(server_socket);
}
