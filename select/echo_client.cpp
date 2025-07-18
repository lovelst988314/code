#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

#define BUFFER_SIZE 1024
int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUFFER_SIZE];
    int str_len;

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

    while(1) {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if(!strcmp(message, "Q\n") || !strcmp(message, "q\n")) {
            break;
        }
        str_len = write(sock, message, strlen(message));

         int recv_len = 0;
        while(recv_len < str_len) {
            int recv_cnt = read(sock, &message[recv_len], BUFFER_SIZE-1);
            if(recv_cnt == -1) {
                std::cout << "read() error" << std::endl;
                exit(1);
            }
            recv_len += recv_cnt;
        }
        // write(sock, message, strlen(message));
        // str_len = read(sock, message, sizeof(message) - 1);  //message不会被清空但是会被覆盖 
        message[str_len] = 0;   //确保字符串以null结尾 不会发生输出失败
        std::cout << "Message from server : " << message << std::endl;  
    }
    close(sock);
    return 0; 
}