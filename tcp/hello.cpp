#include <iostream>
#include <string>
#include <cstdlib> 
#include <cstring> 
#include <unistd.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
using namespace std;
void Exception(const string & why, const int exitCode ) 
{
    cout << "ErrorCode:"<<exitCode <<endl<< why << endl;
    exit(exitCode);
}
int main()
{
    sockaddr_in * selfAddr = new (sockaddr_in);
    selfAddr->sin_family = AF_INET; 
    selfAddr->sin_port = 0;         
    selfAddr->sin_addr.s_addr = 0;  

    sockaddr_in * remoteAddr = new (sockaddr_in);
    remoteAddr->sin_family = AF_INET;     
    remoteAddr->sin_port = htons(44214); 
    remoteAddr->sin_addr.s_addr = inet_addr("127.0.0.1"); 
   
    char *buffer = new char[4096];
    strcpy(buffer,"Привет, как дела?");  
    int msgLen = strlen(buffer);           
   
    int mySocket = socket(AF_INET, SOCK_STREAM, 0); 
    if (mySocket == -1) {
        close(mySocket);
        Exception("Ошибка открытия сокета",11);
    }
    int rc = bind(mySocket,(const sockaddr *) selfAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        Exception("Ошибка привязки сокета с локальным адресом",12);
        }

    rc = connect(mySocket, (const sockaddr*) remoteAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        Exception("Ошибка подключения сокета к удаленному серверу.", 13);
    }

    rc = send(mySocket, buffer, msgLen, 0);
    if (rc == -1) {
        close(mySocket);
        Exception("Сообщение об ошибке отправки", 14);
    }
    cout << "Мы отправляем: " << buffer << endl; 
      
    rc = recv(mySocket, buffer, 4096, 0);
    if (rc == -1) {
        close(mySocket);
       Exception("Ошибка получения ответа.", 15);
    }
    buffer[rc] = '\0'; 
    cout << "Мы получаем: " << buffer << endl;
    close(mySocket);

    delete selfAddr;
    delete remoteAddr;
    delete[] buffer;
    return 0;
}
