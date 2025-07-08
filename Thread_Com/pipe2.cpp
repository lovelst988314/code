#include<iostream>
#include<unistd.h>

#define BUFFER_SIZE 30

int main (int argc, char *argv[]) {
    int fds[2];
    char str1[] = "who are you?";
    char str2[] = "Thansk you!";
    char buf[BUFFER_SIZE];

    pid_t pid;
    pipe(fds);
    pid = fork();

    if(pid == 0) {
        write(fds[1], str1, sizeof(str1));  //fd[1]是管道的写端
        sleep(2);
        read(fds[0], buf, BUFFER_SIZE);
        std::cout << "child proc output: " << buf << std::endl;
    } else {
        read(fds[0], buf, BUFFER_SIZE);   //fd[0]是管道的读端
        std:: cout << "parent proc output: " << buf << std::endl;
        write(fds[1], str2, sizeof(str2));
        sleep(3);
    }
    return 0;
}