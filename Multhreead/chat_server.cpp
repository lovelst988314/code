#include <iostream>
#include <pthread.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

void * handle_clnt(void * arg);
void send_msg(char * msg, int len);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    pthread_t t_id;

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>\n";
        return 1;
    }

    pthread_mutex_init(&mutx, NULL);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        std::cerr << "Bind failed\n";
        close(serv_sock);
        return 1;
    }


    if(listen(serv_sock, 5)) {
        std::cerr << "Listen failed\n";
        close(serv_sock);
        return 1;
    }


    while (true) {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

        pthread_mutex_lock(&mutx);
        clnt_socks[clnt_cnt++] = clnt_sock;
        pthread_mutex_unlock(&mutx);

        pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
        pthread_detach(t_id);

        std::cout << "Connected client IP: " << inet_ntoa(clnt_addr.sin_addr) << "\n";
    }

    close(serv_sock);
    return 0;
}

void* handle_clnt(void* arg) {
    int clnt_sock = *((int*)arg);
    int str_len = 0;
    char msg[BUF_SIZE];

    while ((str_len = read(clnt_sock, msg, sizeof(msg))) != 0) 
        send_msg(msg, str_len);
        pthread_mutex_lock(&mutx);
        for (int i = 0; i < clnt_cnt; i++) {
            if (clnt_socks[i] == clnt_sock) {
                while(i++ < clnt_cnt-1) clnt_socks[i] = clnt_socks[i+1];
                break;
            }
        }
        clnt_cnt--;
        pthread_mutex_unlock(&mutx);
        close(clnt_sock);
        return NULL;
    
}

void send_msg(char * msg, int len) {
    pthread_mutex_lock(&mutx);
    for (int i = 0; i < clnt_cnt; i++) {
        write(clnt_socks[i], msg, len);
    }
    pthread_mutex_unlock(&mutx);
}