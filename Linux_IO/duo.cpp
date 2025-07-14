#include <iostream>
#include <unistd.h>  // 仍然需要这个头文件来使用 dup, dup2, close

int main(int argc, char *argv[])
{
    int cfd1, cfd2;
    const char str1[] = "Hi~ \n";
    const char str2[] = "It's nice day~ \n";

    cfd1 = dup(1);
    cfd2 = dup2(cfd1, 7);

    std::cout << "fd1=" << cfd1 << ", fd2=" << cfd2 << std::endl;

    // 使用 write 函数（保持与原 C 代码一致）
    write(cfd1, str1, sizeof(str1));
    write(cfd2, str2, sizeof(str2));

    close(cfd1);
    close(cfd2);

    // 使用 write 函数（保持与原 C 代码一致）
    write(1, str1, sizeof(str1));
    close(1);
    write(1, str2, sizeof(str2));

    return 0;
}