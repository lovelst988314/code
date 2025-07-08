#include<iostream>
#include<unistd.h>

#define BUFFER_SIZE 30

int main (int argc, char *argv[]) {
    int fds[2];
    char str[] = "who are you?";
    char buf[BUFFER_SIZE];

    pid_t pid;
    pipe(fds);
    pid = fork();

    if(pid == 0) {
        write(fds[1], str, sizeof(str));  //fd[1]是管道的写端
    } else {
        read(fds[0], buf, BUFFER_SIZE);   //fd[0]是管道的读端
        puts(buf);
    }
    return 0;
}