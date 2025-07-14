#include<iostream>
#include<pthread.h>
#include <unistd.h>

// 使用 sleep(1); 表示休眠 1 秒

void *thread_main(void *arg);

int main() {
    pthread_t t_id;
    int thread_param = 5;

    if(pthread_create(&t_id, NULL, thread_main ,(void*)&thread_param) != 0) {
        puts("Thread creation error");
        return -1;
    }
    sleep(10);
    puts("end of main");
    return 0;
}

void *thread_main(void *arg) { 
    int i;
    int cnt = *((int*)arg);
    for(i = 0; i < cnt; i++) {
        sleep(1);
        puts("running thread");
    }
    return NULL;
}





