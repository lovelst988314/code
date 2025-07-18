#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void timeout(int sig) {
    if(sig == SIGALRM) {
        puts("Time out!");
    }
    alarm(2);
}

int main (int argc, char *argv[]) {
    int i;
    struct sigaction act;
    act.sa_handler = timeout;

    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, NULL);

    alarm(2);

    for(i = 0; i < 3; i++) {    
        puts("wait...");
        sleep(100);
    }

    return 0;
}