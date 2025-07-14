#include<iostream>
#include<fcntl.h>

int main(void) {
    FILE *fp;
    int fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC);

    if(fd == -1) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::cout << "first file descriptor:" << fd << std::endl;

    fp = fdopen(fd, "w");

    fputs("Hello, World!\n", fp);

    std::cout << "second file descriptor: " << fileno(fp) << std::endl;
    fclose(fp);
    return 0;
}