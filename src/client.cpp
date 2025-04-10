// #include "memory_manager.h"

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


int SendMessage(char* message)
{

    int sock = 0;
    sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Convert IPv4 address from text to binary
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        return 1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    const char* hello = "Hello from client";
    send(sock, hello, strlen(hello), 0);
    std::cout << "Message sent to server\n";

    read(sock, buffer, 1024);
    std::cout << "Reply from server: " << buffer << std::endl;

    close(sock);
    return 0;



}


int main()
{
    // Implementacion de la lista enlazada


}
