#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    char message[1024];
    int str_len;
    int idx = 0, read_len = 0;

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
    }

    while(read_len = read(sock, &message[idx++], 1)) {
        if(read_len == -1) {
            std::cout << "read() error" << std::endl;
            exit(1);
        }
        str_len += read_len;
    }

    std::cout << "Message from server : " << message << std::endl;
    std::cout << "Read " << str_len << " bytes" << std::endl;

    close(sock);
    return 0; 
}