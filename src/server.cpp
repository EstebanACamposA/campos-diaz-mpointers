
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


void RunServer()
{
    int server_fd, new_socket;
    sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char* hello = "placeholder byte array. In this case the byte array is a char*. It needs a conversion form std::vector<uint8_t>";

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        // return 1;
    }

    // Allow reuse of address and port
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    // Set up address struct
    address.sin_family = AF_INET;         // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
    address.sin_port = htons(8080);       // Port 8080

    // Bind socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        // return 1;
    }

    // Listen
    listen(server_fd, 3);

    std::cout << "Server is waiting for a connection..." << std::endl;

    while (true)
    {
        // Accept connection
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0)
        {
            perror("accept");
            // return 1;
            continue;;
        }

        // Read from client
        read(new_socket, buffer, 1024);
        std::cout << "Message from client: " << buffer << std::endl;
        // ADD A SWITCH CASE DEPENDING ON WHAT THE MESSAGE STARTS WITH. THAT DECIDES THE WANTED FUNCTION FROM THE SERVER.

        // Send reply
        send(new_socket, hello, strlen(hello), 0);
        std::cout << "Hello message sent" << std::endl;

        close(new_socket);
    }

    close(server_fd);    
    
    // return 0;
}








