#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

// 定义信号量和共享变量
static sem_t sem_one;   // 0
static sem_t sem_two;   // 1
static int num;

// 线程函数：读取数字
void* read(void* arg) {
    for (int i = 0; i < 5; ++i) {
        cout << "Input num: ";
        sem_wait(&sem_two); // 等待 sem_two 信号量
        cin >> num;
        sem_post(&sem_one);  // 释放 sem_one 信号量  
    }
    return nullptr;
}

// 线程函数：累加数字
void* accu(void* arg) {
    int sum = 0;
    for (int i = 0; i < 5; ++i) {
        sem_wait(&sem_one);  // 等待 sem_one 信号量
        sum += num;
        sem_post(&sem_two);  // 释放 sem_two 信号量
    }
    cout << "Result: " << sum << endl;
    return nullptr;
}

int main(int argc, char *argv[]) {
    pthread_t id_t1, id_t2;

    // 初始化信号量
    sem_init(&sem_one, 0, 0);
    sem_init(&sem_two, 0, 1);

    // 创建两个线程
    pthread_create(&id_t1, NULL, read, NULL);
    pthread_create(&id_t2, NULL, accu, NULL);

    // 等待两个线程完成
    pthread_join(id_t1, NULL);
    pthread_join(id_t2, NULL);

    // 销毁信号量
    sem_destroy(&sem_one);
    sem_destroy(&sem_two);

    return 0;
}