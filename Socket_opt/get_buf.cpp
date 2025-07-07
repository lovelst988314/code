#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>

int main (int argc, char *argv[]) {
    int sock;
    int snd_buf, rcv_buf, state;
    socklen_t len;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);

    if(state) {
        std::cout << "get socket type error" << std::endl;
        exit(1);
    }

    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);

    if(state) {
        std::cout << "get socket type error" << std::endl;
        exit(1);
    }

    std::cout << "snd_buf : " << snd_buf << " rcv_buf : " << rcv_buf << std::endl;
    
}