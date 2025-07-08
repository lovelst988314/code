#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<signal.h>
#include<wait.h>

#define BUFFER_SIZE 30

void read_childproc(int sig);
int main(int argc, char *argv[]) {

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;

    pid_t pid;
    struct sigaction act;
    socklen_t client_addr_size;

    char message[BUFFER_SIZE];
    int str_len, state; // 接受信息的长度

    if(argc != 2) { 
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        exit(1);
    }

    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);

    // Create socket
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

    

    // Accept a client connection
    while(1) {
        client_addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);

        if (client_socket == -1) {
            continue;
        } else {
            std::cout << "Client connected：" << std::endl;
        }
        pid = fork();  
        if(pid == -1) {
            close(client_socket);
            continue;
        }

        if(pid == 0) {
            close(server_socket);
            while((str_len = read(client_socket, message, BUFFER_SIZE-1)) != 0) {
                write(client_socket, message, str_len);  //写回的长度应该直接使用 read的返回值而不是 strlen(message)
            }
            close(client_socket);
            puts("client disconnected..  ");
            return 0; 
        } else {
            close(client_socket);
        }
    }
    close(server_socket);
    return 0;
}

void read_childproc(int sig) {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    std::cout << "removed proc id: " << pid << std::endl;
}