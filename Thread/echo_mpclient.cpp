#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

#define BUFFER_SIZE 30

void read_routine(int sock, char *message);
void write_routine(int sock, char *message); 
int main (int argc, char *argv[]) {
    int sock;
    pid_t pid;
    struct sockaddr_in serv_addr;
    char message[BUFFER_SIZE];

    if(argc != 3) {
        std::cout << "Usage : " << argv[0] << " <IP> <port>" << std::endl;
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        std::cout << "socket() error" << std::endl;
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        std::cout << "connect() error" << std::endl;
        exit(1);
    } else {
        std::cout << "Connected" << std::endl;
    }

    pid = fork();
    if(pid == 0) {
        write_routine(sock,message);
    } else {
        read_routine(sock,message);
    }
    close(sock);
    return 0; 
}

void read_routine(int sock, char *message) {
    while(1) {
        int str_len = read(sock, message, BUFFER_SIZE);
        if(str_len == 0) 
            return;
        message[str_len] = '\0';
        std::cout << "message form server: " << message << std::endl;
    }
}
void write_routine(int sock, char *message) {
    while(1) {
        fgets(message, BUFFER_SIZE, stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            shutdown(sock, SHUT_WR);
            return;
        }
        write(sock, message, strlen(message));
    }
}
