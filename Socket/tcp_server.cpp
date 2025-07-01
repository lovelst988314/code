#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

int main(int argc, char *argv[]) {
    int server_socket;
    int client_socket;

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;

    char message[1024] = "Hello, Client!";

    if(argc != 2) { 
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        exit(1);
    }

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        exit(1);
    }

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        std::cerr << "Error listening on socket" << std::endl;
        exit(1);
    }

    client_addr_size = sizeof(client_addr);

    // Accept a client connection
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
    if (client_socket < 0) {
        std::cerr << "Error accepting connection" << std::endl;
        exit(1);
    }

    write(client_socket, message, strlen(message));
    close(client_socket);
    close(server_socket);
    return 0;
}