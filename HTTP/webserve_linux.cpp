#include <iostream>
#include <pthread.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>

#define BUF_SIZE 1024
#define SMALL_BUF_SIZE 100

void * handle_request(void * arg);
void send_data(FILE* fp, char* ct, char* file_name);
char* content_type(char* file);
void send_error(FILE *fp);


int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    char buf[BUF_SIZE];
    pthread_t t_id;
    if(argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }   
    
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) {
        std::cerr << "Socket creation failed\n";
        return 1;
    } 

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    if(listen(serv_sock, 5) == -1) {
        std::cerr << "Listen failed\n";
        return 1;
    }

    while(true) {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        std::cout <<"connection request: " << inet_ntoa(clnt_addr.sin_addr) << "  "  << ntohs(clnt_addr.sin_port) << std::endl;
        pthread_create(&t_id, NULL, handle_request, (void*)&clnt_sock);
        pthread_detach(t_id);
    }
    close(serv_sock);
    return 0;    
}

void * handle_request(void * arg) {
    int clnt_sock = *((int*)arg);
    char req_line[SMALL_BUF_SIZE];
    FILE *clnt_read;
    FILE *clnt_write;

    char method[10];
    char ct[15];
    char file_name[30];

    clnt_read = fdopen(clnt_sock, "r");
    clnt_write = fdopen(dup(clnt_sock), "w");
    fgets(req_line, SMALL_BUF_SIZE, clnt_read);
    if(strstr(req_line, "HTTP/") == NULL) {
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return NULL;
    }
    strcpy(method, strtok(req_line, " /"));
    strcpy(file_name, strtok(NULL, " /"));
    strcpy(ct, content_type(file_name));
    if(strcmp(method, "GET") != 0) {
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return NULL;
    }

    fclose(clnt_read);
    send_data(clnt_write, ct, file_name);
}

void send_data(FILE *clnt_write, char *ct, char *file_name) { 
    char protocol[] = "HTTP/1.0 200 OK\r\n";
    char server[] = "Server: Linux web server\r\n";
    char cnt_len[] = "Content-Length: 2048\r\n";
    char cnt_type[SMALL_BUF_SIZE];
    char buf[BUF_SIZE];
    FILE *send_file;

    sprintf(cnt_type, "Content-Type: %s\r\n\r\n", ct);
    send_file = fopen(file_name, "r");

    if(send_file == NULL) {
        send_error(clnt_write);
        return;
    }

    //传输头信息
    fputs(protocol, clnt_write);
    fputs(server, clnt_write);
    fputs(cnt_len, clnt_write);
    fputs(cnt_type, clnt_write);

    //传输请求数据
    while(fgets(buf, BUF_SIZE, send_file) != NULL) {
        fputs(buf, clnt_write);
        fflush(clnt_write);
    }
    fflush(clnt_write);
    fclose(clnt_write);
}

char* content_type(char* file) {
    char extension[SMALL_BUF_SIZE];
    char file_name[SMALL_BUF_SIZE];
    strcpy(file_name, file);

    strtok(file_name, ".");
    strcpy(extension, strtok(NULL, "."));

    if(!strcmp(extension, "html") || !strcmp(extension, "htm")) {
        return "text/html";
    }
    else {
        return "text/plain";
    }
}

void send_error(FILE *fp) {
    char protocol[] = "HTTP/1.0 404 NOT FOUND\r\n";
    char server[] = "Server: Linux web server\r\n";
    char cnt_len[] = "Content-Length: 2048\r\n";
    char cnt_type[] = "Content-Type: text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
                     "<body><font size=+5><br>发生错误！查看请求文件名"
                     "</font></body></html>";

    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fflush(fp);
}

