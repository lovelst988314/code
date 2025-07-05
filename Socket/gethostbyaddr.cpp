#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<cstring>

int main (int argc, char *argv[]) {
    struct hostent *host;
    struct sockaddr_in addr;

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <IP>" << std::endl;
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);

    if(host == NULL) {
        exit(1);
    }

    std::cout << "Official name: " << host->h_name << std::endl;
    for(int i = 0; host->h_aliases[i] != NULL; i++) {
        std::cout << "Alias " << i+1 << ": " << host->h_aliases[i] << std::endl;
    }
    
    std::cout << "Address type: " << ((host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6") << std::endl;

    for(int i = 0; host->h_addr_list[i] != NULL; i++) {
        std::cout << "IP Address " << i+1 << ": " << inet_ntoa(*(struct in_addr*)host->h_addr_list[i]) << std::endl;
    }

    return 0;

}