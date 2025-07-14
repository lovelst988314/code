#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/epoll.h>

#define BUFFER_SIZE 100
#define EPOLL_SIZE 50

int main(int argc, char *argv[]) {

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    int str_len, i;
    char buf[BUFFER_SIZE];

    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd, event_cnt;

    if(argc != 2) { 
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        exit(1);
    }

    // Create socket333
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

    epfd = epoll_create(EPOLL_SIZE);
    ep_events = (struct epoll_event *)malloc(EPOLL_SIZE * sizeof(struct epoll_event));

    event.events = EPOLLIN;
    event.data.fd = server_socket;
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_socket, &event);  //检测server_socket的可读事件

    // Accept a client connection
    while(1) {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if(event_cnt == -1) {
            std::cerr << "Error in epoll_wait" << std::endl;
            break;
        }
        for(i = 0; i < event_cnt; i++) { 
            if(ep_events[i].data.fd == server_socket) { 
                client_addr_size = sizeof(client_addr);
                client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
                event.events = EPOLLIN;
                event.data.fd = client_socket;
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_socket, &event);
                std::cout << "Connected client: " << client_socket << std::endl;
            } else {
                str_len = read(ep_events[i].data.fd, buf, BUFFER_SIZE);
                if(str_len == 0) {   //文件结束（EOF）
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(ep_events[i].data.fd);
                    std::cout << "Closed client: " << ep_events[i].data.fd << std::endl;
                } else {
                    write(ep_events[i].data.fd, buf, str_len);
                }
            }
        }
    }
    close(server_socket);
    close(epfd);
    free(ep_events);
    return 0;
}