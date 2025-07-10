#include<iostream>
#include<sys/uio.h>

int main(int argc, char **argv) {
    struct iovec iov[2];
    char buf1[] = "12345";
    char buf2[] = "67890";

    int str_len;
    iov[0].iov_base = buf1;
    iov[0].iov_len = 3;  // Length of buf1
    iov[1].iov_base = buf2;
    iov[1].iov_len = 2;  // Length of buf2

    str_len = writev(1, iov, 2);   //1表示向控制台输出

    std::cout << std::endl;

    std::cout << "Total bytes written: " << str_len << std::endl;
    
    return 0;
}