#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

int main (int argc, char *argv[]) {
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        return 3;
    } else {
        std::cout << "Child PID: " << pid << std::endl;
        pid = fork();
        if(pid == 0) {
            exit(7);
        } else {
            std::cout << "Child PID: " << pid << std::endl;
            wait(&status);
            std:: cout << status << std::endl;
            if(WIFEXITED(status)) {
                std::cout << "Child exited with code one " << WEXITSTATUS(status) << std::endl;
            }
            wait(&status);
            if(WIFEXITED(status)) {
                std::cout << "Child exited with code two " << WEXITSTATUS(status) << std::endl;
            }
            sleep(30);
        }
    }
    return 0;
}