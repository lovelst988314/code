#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int sock;
    char message[BUFFER_SIZE];
    int str_len;
    struct sockaddr_in serv_addr;

    FILE *readfp;
    FILE *writefp;

    if(argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        std::cerr << "socket() error" << std::endl;
        exit(1);
    }   

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        std::cerr << "connect() error" << std::endl;
        exit(1);
    } else {
        std::cout << "Connected to server" << std::endl;
    }

    readfp = fdopen(sock, "r");
    writefp = fdopen(sock, "w");

    while(1) { 
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if(!strcmp(message, "Q\n") || !strcmp(message, "q\n"))  break;
        fputs(message, writefp);
        fflush(writefp);  
        fgets(message, sizeof(message), readfp);
        std::cout << "Message from server: " << message << std::endl;
    }
    fclose(writefp);
    fclose(readfp);
    return 0;
}
