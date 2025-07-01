#include<iostream>
#include<arpa/inet.h>
#include<string>

using namespace std;

int main() {
    string ip_address1 = "1.5.3.4";
    string ip_address2 = "192.168.1.777";

    unsigned long addr1 = inet_addr(ip_address1.c_str());
    if(addr1 == INADDR_NONE) {
        cerr << "Invalid IP address: " << ip_address1 << endl;
        return 1;
    } else 
        cout << "IP address: " << ip_address1 << " is " << hex << addr1 << endl;

    unsigned long addr2 = inet_addr(ip_address2.c_str());
    if(addr2 == INADDR_NONE) {
        cerr << "Invalid IP address: " <<  ip_address2 << endl;
        return 1;
    } else 
        cout << "IP address: " << ip_address2 << " is " << hex << addr2 << endl;

    return 0;
}