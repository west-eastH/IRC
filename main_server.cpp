#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CONNECTIONS 5
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Set up server address structure
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_socket, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CONNECTIONS) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    // Accept incoming connection
    socklen_t client_address_len = sizeof(client_address);
    if ((client_socket = accept(server_socket, reinterpret_cast<struct sockaddr*>(&client_address), &client_address_len)) == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Connection accepted from " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << "\n";

    // Receive data from client
    recv(client_socket, buffer, BUFFER_SIZE, 0);
    std::cout << "Received from client: " << buffer << "\n";

    // Send response to client
    const char *response = "Hello from server!";
    send(client_socket, response, strlen(response), 0);

    // Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}