#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    char message[BUFFER_SIZE];
    int str_len;

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    FILE *readfp;
    FILE *writefp;

    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        exit(1);
    }

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(serv_sock < 0) {
        std::cerr << "Error creating socket" << std::endl;
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        exit(1);
    }

    if(listen(serv_sock, 5) == -1) {
        std::cerr << "Error listening on socket" << std::endl;
        exit(1);
    }

    for(int i = 0; i < 5; i++) {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

        if(clnt_sock == -1) {
            std::cerr << "Error accepting connection" << std::endl;
            exit(1);
        } else {
            std::cout << "Client connected: " << i + 1 << std::endl;
        }

        readfp = fdopen(clnt_sock, "r");
        writefp = fdopen(clnt_sock, "w");

        while(!feof(readfp)) {
            fgets(message, BUFFER_SIZE, readfp);  //检查是否到达文件末尾 传入参数是一个FILE 对象的指针  表示要检查的输入流
            fputs(message, writefp);
            fflush(writefp);
        }

        fclose(readfp);
        fclose(writefp);
    }
    close(serv_sock);
    return 0;
}