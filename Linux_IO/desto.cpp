#include<iostream>
#include<fcntl.h>

int main(void) {
    FILE *fp1;
    int fd = open("data.dat", O_WRONLY| O_CREAT| O_TRUNC);

    if(fd == -1) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    fp1 = fdopen(fd, "w");
    fputs("Hello, World!\n", fp1);
    fclose(fp1);
    return 0;
}
