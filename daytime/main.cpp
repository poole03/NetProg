#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <memory>
using namespace std;

int main(){
    unique_ptr< sockaddr_in > self_addr(new sockaddr_in);
    unique_ptr< sockaddr_in > remote_addr(new sockaddr_in);
    int sr_code;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if( sock < 0 ) {
        cerr << "Failed to create socket" << endl;
        return 1;
    }


    self_addr->sin_family = AF_INET;
    self_addr->sin_addr.s_addr = INADDR_ANY;
    self_addr->sin_port = htons(0);

    if( bind(sock, reinterpret_cast< const sockaddr* >(self_addr.get()), sizeof(sockaddr)) < 0 ) {
        cerr << "Failed to bind socket" << endl;
        return 1;
    }

    remote_addr->sin_family = AF_INET;
    remote_addr->sin_addr.s_addr = inet_addr("82.179.90.12");//inet_addr("172.16.40.1");
    remote_addr->sin_port = htons(13);

    char buf[256];

    if( sendto(sock, buf, 256, 0, reinterpret_cast< const sockaddr* >(remote_addr.get()), sizeof(sockaddr)) < 0 ) {
        cerr << "Failed to start exchange" << endl;
        return 1;
    }

    socklen_t clen = sizeof(sockaddr);

    sr_code = recvfrom(sock, buf, 256, 0, reinterpret_cast< sockaddr* >(remote_addr.get()), &clen);
    if( sr_code == -1 ) {
        cerr << "Failed to recieve message" << endl;
        return 1;
    }

    string daytime(buf, sr_code);

    cout << daytime << endl;
    
    return 0;
}
