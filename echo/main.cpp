#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>

using namespace std;
//Определение размера буфера с помощью макроса
#define BUFSZ 128
int main(int argc, char *argv[])
{
    int sock; //Сокет
    char buffer[BUFSZ]; //Буфер для приема ответа сервера
    struct sockaddr_in server; //Содержит информацию о сервере
// Создание сокета
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cout << "Failed to create socket" << endl;
        exit(1);
    }
// Настройка адреса сервера
    server.sin_family = AF_INET;
    server.sin_port = htons(7);
    server.sin_addr.s_addr = inet_addr("82.179.90.12");//inet_addr("127.0.0.1");
	
// Подключение к серверу
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        cout << "Error connecting to the server" << endl;
        exit(1);
    }
//Отправка запроса серверу
	string message;
	cout << "Enter the message to send: ";
	getline(cin, message);
	
    if(send(sock, message.c_str(), message.size(), 0) < 0) {
        cout<< "Error sending the request" << endl;
        exit(1);
    }
    
// Получение ответа от сервера
    int num_bytes = recv(sock, buffer, sizeof(buffer), 0);
    if(num_bytes < 0){
        cout << "Error when receiving a response from the server" << endl;
        exit(1);
    }
// Вывод результата
    cout << "Message: " << endl;
    buffer[num_bytes] = '\0';
    cout << buffer << endl;
// Закрытие сокета
    close(sock);
}
