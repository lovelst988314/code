#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

#define BUFFER_SIZE 1024

int calculate(int opum, int* opnds, char op) {
    int result = opnds[0];
    switch(op) {
        case '+':
            for(int i = 1; i < opum; i++)
                result += opnds[i];
            break;
        case '-':
            for(int i = 1; i < opum; i++)
                result -= opnds[i];
            break;
        case '*':
            for(int i = 1; i < opum; i++)
                result *= opnds[i];
    }
    return result;
}

int main(int argc, char *argv[]) {

    int server_socket, client_socket;
    char message[BUFFER_SIZE];
    int str_len; // 接受信息的长度

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;

    if(argc != 2) { 
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        exit(1);
    }

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        std::cerr << "Error listening on socket" << std::endl;
        exit(1); 
    }

    client_addr_size = sizeof(client_addr);

    // Accept a client connection
    for(int i = 0; i < 5; i++) {

        int opnd_cnt = 0;
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);

        read(client_socket, &opnd_cnt, 1);

        int recv_len = 0, recv_cnt = 0;
        while (recv_len < opnd_cnt*4+1) { 
            recv_cnt += read(client_socket, &message[recv_len], BUFFER_SIZE-1);
            recv_len += recv_cnt;
        }

        int result = 0;
        result = calculate(opnd_cnt, (int*)message, message[recv_len-1]);
        
        std::cout << "result: " << result << std::endl;
        
        write(client_socket, &result, sizeof(result));  //写回的长度应该直接使用 read的返回值而不是 strlen(message)
    
        close(client_socket);
    }
    close(server_socket);
    return 0;
}