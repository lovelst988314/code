#include<iostream>
#include<unistd.h>



int main (int argc, char* argv[]) {
    pid_t pid;
  

    pid = fork();

    if(pid == 0) {
        puts("Child Process");
    } else {
        std::cout <<  "child process id: " << pid << std::endl;
        sleep(10);
    }

    if(pid == 0) {
        std::cout << "end child process" << std::endl;
    } else {
        puts("end parent process");
    }

    return 0;
}