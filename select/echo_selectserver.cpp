#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/select.h>
#include<sys/time.h>

#define BUFFER_SIZE 100

int main(int argc, char *argv[]) {

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    struct timeval timeout;
    fd_set reads, cpy_reads;

    socklen_t client_addr_size;
    int fd_max, str_len, fd_num, i;
    char buf[BUFFER_SIZE];

    if(argc != 2) { 
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        exit(1);
    }

    // Create socket333
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        std::cerr << "Error listening on socket" << std::endl;
        exit(1);
    }

    FD_ZERO(&reads);
    FD_SET(server_socket, &reads);
    fd_max = server_socket;

    // Accept a client connection
    while(1) {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if((fd_num = select(fd_max + 1, &cpy_reads, NULL, NULL, &timeout)) == -1) break;
        if(fd_num == 0) continue;
        for(int i = 0; i < fd_max + 1; i++) {
            if(FD_ISSET(i, &cpy_reads)) {
                if(i  == server_socket) {
                    client_addr_size = sizeof(client_addr);
                    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
                    FD_SET(client_socket, &reads);
                    if(fd_max < client_socket) fd_max = client_socket;
                    std::cout << "connected client socket: " << client_socket << std::endl;
                } else {
                    str_len = read(i, buf, BUFFER_SIZE);
                    if(str_len == 0) {
                        FD_CLR(i, &reads);
                        close(i);
                        std::cout << "closed client socket: " << i << std::endl;
                    } else {
                        write(i, buf, str_len);
                    }
                }

            }
        }
    }
    close(server_socket);
    return 0;
}