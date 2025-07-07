#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>

int main(int argc, char *argv[]) {
    int sock;
    int snd_buf = 1024 * 3, rcv_buf = 1024 * 3;
    int state;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, sizeof(snd_buf));

    if(state) {
        std::cout << "setsockopt() error" << std::endl;
        exit(1);
    }

    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, sizeof(rcv_buf));
    if(state) {
        std::cout << "setsockopt() error" << std::endl;
        exit(1);
    }   

    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
    if(state) {
        std::cout << "getsockopt() error" << std::endl;
        exit(1);
    }

    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
    if(state) {
        std::cout << "getsockopt() error" << std::endl;
        exit(1);
    }

    std::cout << "Output buffer size: " << snd_buf << std::endl;
    std::cout << "Input buffer size: " << rcv_buf << std::endl;
    return 0;
}