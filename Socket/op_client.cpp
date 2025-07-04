#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

#define BUFFER_SIZE 1024
#define OPSZ 4
#define RLT_SIZE 4

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


    std::cout << "Operand count:";
    int opnd_cnt;
    std::cin >> opnd_cnt;
    message[0] = (char)opnd_cnt; 
    for(int i = 0; i < opnd_cnt; i++) {
        std::cout << "Operand" << i+1 << ": ";
        int opnd;
        std::cin >> opnd;
        memcpy(&message[1 + i * OPSZ], &opnd, OPSZ); // 按4字节存储到message
    }
    std::cout << "Operator:";
    char op;
    std::cin >> op;
    message[1 + opnd_cnt * OPSZ] = op;
       
    write(sock, message, 2 + opnd_cnt * OPSZ);

    int result;
    read(sock, &result, RLT_SIZE);

    std::cout << "Operation's result: " << result << std::endl;

    close(sock);
    return 0; 
}