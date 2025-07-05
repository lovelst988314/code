#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netdb.h>

int main (int argc, char *argv[]) {
    struct hostent *host;
    if(argc != 2) {
        std::cout << "Usage: " << argv[0] << " <IP>" << std::endl;
        exit(1);
    }
    host = gethostbyname(argv[1]);  
    if(!host) {
        std::cout << "Host not found" << std::endl;
        exit(1);
    }

    std::cout << "Official name: " << host->h_name << std::endl;

    for(int i = 0; host->h_aliases[i]; i++) {
        std::cout << "Alias " << i+1 << ": " << host->h_aliases[i] << std::endl;
    }

    std::cout << "Address type: " << ((host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6") << std::endl;

    for(int i = 0; host->h_addr_list[i]; i++)  {
        std::cout << "IP Address " << i+1 << ": " << inet_ntoa(*(struct in_addr*)host->h_addr_list[i]) << std::endl;
    }
}
 
// h_addr_list 是一个指针数组，数组的元素是 h_addr 的指针。 
//   (struct in_addr*)host->h_addr_list[i]    in_addr中存储的是ip地址的二进制表示 然后解引用为ip地址 在转换成字符串
