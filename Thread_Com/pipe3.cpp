#include<iostream>
#include<unistd.h>

#define BUFFER_SIZE 30

int main (int argc, char *argv[]) {
    int fds1[2], fds2[2];
    char str1[] = "who are you?";
    char str2[] = "Thansk you!";
    char buf[BUFFER_SIZE];

    pid_t pid;
    pipe(fds1), pipe(fds2);
    pid = fork();

    if(pid == 0) {
        write(fds1[1], str1, sizeof(str1));  //fd[1]是管道的写端
        read(fds2[0], buf, BUFFER_SIZE);
        std::cout << "child proc output: " << buf << std::endl;
    } else {
        read(fds1[0], buf, BUFFER_SIZE);   //fd[0]是管道的读端
        std:: cout << "parent proc output: " << buf << std::endl;
        write(fds2[1], str2, sizeof(str2));
        sleep(3);
    }
    return 0;
}

// 使用两个管道来完成两个进程间的通信