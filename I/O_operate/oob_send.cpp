#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>

#define BUFFER_SIZE 30

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in recv_adr;
    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <IP> <Port>" << std::endl;
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock < 0) {
        std::cerr << "Error creating socket" << std::endl;
        exit(1);
    }

    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    recv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1) {
        std::cerr << "Error connecting to server" << std::endl;
        close(sock);
        exit(1);
    }

    write(sock, "Hello, World!", 13);
    send(sock, "message!", strlen("message!"), MSG_OOB);

    write(sock, "567", 3);
    send(sock, "890", 3, MSG_OOB);
    close(sock);
    return 0;
}