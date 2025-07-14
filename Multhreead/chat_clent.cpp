#include <iostream>
#include <pthread.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

void * send_msg(void * arg);
void * recv_msg(void * arg);

char msg[BUF_SIZE];
char name[NAME_SIZE] = "[DEFAULT]";

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    pthread_t snd_thread, rcv_thread;
    void *thread_return;
    if(argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <IP> <port> <nickname>\n";
        return 1;
    }

    sprintf(name ,"[%s]", argv[3]);

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        std::cerr << "Connection failed\n";
    }

    pthread_create(&snd_thread, NULL, send_msg, (void *)&sock);
    pthread_create(&rcv_thread, NULL, recv_msg, (void *)&sock);
    pthread_join(snd_thread, &thread_return);
    pthread_join(rcv_thread, &thread_return);
    close(sock);
    return 0;
}

void *send_msg(void *socket) { 
    int sock = *((int *)socket);
    char name_msg[BUF_SIZE+NAME_SIZE];
    while (true) {
        fgets(msg, BUF_SIZE, stdin);
        if (strcmp(msg, "q") == 0 || strcmp(msg, "Q") == 0) {
            close(sock);
            exit(0);
        }
        sprintf(name_msg ,"%s %s", name, msg);
        write(sock, name_msg, strlen(name_msg));
    }
    return NULL;
}

void *recv_msg(void *socket) {
    int sock = *((int *)socket);
    char name_msg[BUF_SIZE+NAME_SIZE];
    int str_len;
    while (true) {
        str_len = read(sock, name_msg, BUF_SIZE+NAME_SIZE-1);
        if (str_len == -1)
            return (void *)-1;
        name_msg[str_len] = '\0';
        fputs(name_msg, stdout);
    }
    return NULL;
}