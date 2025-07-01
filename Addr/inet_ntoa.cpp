#include<iostream>
#include<string>
#include<arpa/inet.h>
#include<cstring>

using namespace std;

int main (int argc, char *argv[]) {
    struct sockaddr_in addr1, addr2;

    char *str_ptr;
    char str_arr[20];

    addr1.sin_addr.s_addr = htonl(0x1020304);
    addr2.sin_addr.s_addr = htonl(0x1010101); 

    str_ptr = inet_ntoa(addr1.sin_addr);
    strcpy(str_arr, str_ptr);

    inet_ntoa(addr2.sin_addr);
    cout << str_ptr << endl;
    cout << str_arr << endl;    

    return 0;
}

// 网络地址的初始化
//  struct sockaddr_in addr;
//  char * server_ip = "192.168.1.1";
//  char * server_port = "9190";
//  meset(&addr, 0, sizeof(addr));
//  addr.sin_family = AF_INET;
//  addr.sin_port = htons(atoi(server_port));
//  addr.sin_addr.s_addr = inet_addr(server_ip);