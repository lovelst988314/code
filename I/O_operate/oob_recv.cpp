#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<fcntl.h>


#define BUFFER_SIZE 30  

void urg_handler(int signo);

int accept_sock, recv_sock;
int main (int argc, char **argv) {
    struct sockaddr_in recv_adr, serv_adr;
    int str_len, state;
    socklen_t serv_adr_sz;
    struct sigaction act;
    char buffer[BUFFER_SIZE];

    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << "<Port>" << std::endl;
        exit(1);
    }

    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    accept_sock = socket(PF_INET, SOCK_STREAM, 0);   //accept_sock对应recv_adr
    if(accept_sock == -1) {
        std::cerr << "socket() error" << std::endl;
        exit(1);
    }
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(accept_sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1) {
        std::cerr << "bind() error" << std::endl;
        close(accept_sock);
        exit(1);
    }

    if(listen(accept_sock, 5) == -1) {
        std::cerr << "listen() error" << std::endl;
        close(accept_sock);
        exit(1);
    }

    serv_adr_sz = sizeof(serv_adr);
    recv_sock = accept(accept_sock, (struct sockaddr*)&serv_adr, &serv_adr_sz);

    fcntl(recv_sock, F_SETOWN, getpid());  // 表示将当前进程设为recv_sock上紧急数据到达时的通知目标。
    state = sigaction(SIGURG, &act, 0);

    while((str_len = recv(recv_sock, buffer, BUFFER_SIZE, 0)) != 0) {
        if(str_len == -1) 
            continue;
        buffer[str_len] = 0;
        std::cout << "Received: " << buffer << std::endl;
    }
    close(recv_sock);
    close(accept_sock);
    return 0;
}

void urg_handler(int signo) {
    int str_len;
    char buffer[BUFFER_SIZE];
    str_len = recv(recv_sock, buffer, sizeof(buffer)-1, MSG_OOB);
    buffer[str_len] = 0;
    std::cout << "Urgent message: " << buffer << std::endl;
}