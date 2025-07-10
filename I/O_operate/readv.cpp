#include<iostream>
#include<sys/uio.h>

#define BUF_SIZE 100 
int main(int argc, char **argv) {
    struct iovec iov[2];
    char buf1[BUF_SIZE] = "0,";
    char buf2[BUF_SIZE] = "0,";

    int str_len;
    iov[0].iov_base = buf1;
    iov[0].iov_len = 5;  // Length of buf1
    iov[1].iov_base = buf2;
    iov[1].iov_len = BUF_SIZE;  // Length of buf2

    str_len = readv(0, iov, 2);   //0表示向控制台输入

    std::cout << "Total bytes written: " << str_len << std::endl;
    std::cout << "First buffer: " << buf1 << std::endl;
    std::cout << "Second buffer: " << buf2 << std::endl;
    
    return 0;
}