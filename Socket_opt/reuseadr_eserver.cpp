#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<cstring>

#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[]) {
    int sev_sock, clnt_sock;
    char message[30];
    int option, str_len;
    socklen_t optlen, clnt_adr_sz;
    struct sockaddr_in serv_adr, clnt_adr;
    if(argc != 2) {
        std::cout << "Usage : <%s> <port>" << argv[0] << std::endl;
        exit(1);
    }

    sev_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sev_sock == -1) {
        std::cout << "socket() error" << std::endl;
        exit(1);
    }

    // optlen = sizeof(option);
    // option = TRUE;
    // setsockopt(sev_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(sev_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        std::cout << "bind() error" << std::endl;
        exit(1);
    }

    if(listen(sev_sock, 5) == -1) {
        std::cout << "listen() error" << std::endl;
        exit(1);
    }

    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(sev_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

    while((str_len = read(clnt_sock, message, sizeof(message))) != 0) {
        write(clnt_sock, message, str_len);
        write(1, message, str_len);
    }

    close(clnt_sock);
    close(sev_sock);
    return 0;
}