#include<iostream>
#include<unistd.h>
#include<sys/time.h>
#include<sys/select.h>

#define BUf_SIZE 30

int main(int argc, char *argv[]) {
    fd_set reads, temps;
    int result, str_len;
    char buf[BUf_SIZE];
    struct timeval timeout;

    FD_ZERO(&reads);
    FD_SET(0, &reads);  //FD_SET 第一个参数是位置 都是设置为1


    // timeout.tv_sec = 5;
    // timeout.tv_usec = 5000;

    while(1) {
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        result = select(1, &temps, 0, 0, &timeout);
        if(result == -1) {
            puts("select() error");
            break;
        } else if(result == 0) {
            puts("Time-out");
            break;
        }  else {
            if(FD_ISSET(0, &temps)) {
                str_len = read(0, buf, BUf_SIZE);
                buf[str_len] = '\0';
                std::cout << "message from console: " << buf << std::endl;
            }
        }
    }
    return 0;
}