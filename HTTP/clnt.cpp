#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <IP> <Port>\n";
        return 1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);

    // 创建 socket
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    // 设置地址信息
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serv_addr.sin_addr);

    // 连接服务器
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        std::cerr << "Connection failed\n";
        close(sock);
        return 1;
    }

    // 构造 HTTP GET 请求
    const char* request = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
    write(sock, request, strlen(request));

    // 接收响应
    char buffer[BUF_SIZE];
    int bytes_read;
    while ((bytes_read = read(sock, buffer, BUF_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0';
        std::cout << buffer;
    }

    close(sock);
    return 0;
}