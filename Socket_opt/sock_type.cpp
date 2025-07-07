#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>

int main(int argc,char **argv) {
    int tcp_sock, udp_sock;
    int sock_type;  //用一个整型来用保存套接字类型
    socklen_t optlen;
    int state;

    optlen = sizeof(sock_type);
    tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
    std::cout << "SOCK_STREAM: " << SOCK_STREAM << std::endl;
    std::cout << "SOCK_DGRAM: " << SOCK_DGRAM << std::endl;

    state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if(state) {
        std::cout << "get socket type error" << std::endl;
        exit(1);
    }
    std::cout << "tcp socket type: " << sock_type << std::endl;

    state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if(state) {
        std::cout << "get socket type error" << std::endl;
        exit(1);
    }
    std::cout << "udp socket type: " << sock_type << std::endl;

    return 0;
}