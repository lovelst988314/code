#include<iostream>
#include<unistd.h>
#include<signal.h>

void timeout(int sig) {
    if(sig == SIGALRM) {
        puts("Timeout!");
    }
    alarm(2);
}

void keycontrol(int sig) {
    if(sig == SIGINT) {
        puts("Ctrl+C pressed");
    }
}

int main(int argc, char *argv[]) {
    int i;
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(2);

    for(int i = 0; i < 3; i++) {
        puts("wait...");
        sleep(100);
        std:: cout << i << std::endl;
    }

    return 0;
}
