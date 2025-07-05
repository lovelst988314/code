#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

#define BUF_SIZE 30
int main(int argc, char *argv[]) {
    int sock;
    FILE *fp;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    int read_cnt;

    if(argc != 3) {
        std::cout << "Usage : " << argv[0] << " <IP> <port>" << std::endl;
        exit(1);
    }

    fp = fopen("receive.dat", "wb");
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

    while((read_cnt = read(sock, message, BUF_SIZE)) != 0) {
        fwrite(message, 1, read_cnt, fp);
    }

    puts("Received file data");
    write(sock,"Thank you", 10);
    fclose(fp);
    close(sock);

    return 0; 
}