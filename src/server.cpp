
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "memory_manager.h"

void RunServer(Heap* heap)
{
    int server_fd, new_socket;
    sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    // const char* hello = "placeholder byte array. In this case the byte array is a char*. It needs a conversion form std::vector<uint8_t>";
    const char* hello = "This gets sent if the client called the server from a void type method.";

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


        // ssize_t bytes_read = read(new_socket, buffer, 1024);

        // if (bytes_read > 0) {


        // ADD A SWITCH CASE DEPENDING ON WHAT THE MESSAGE STARTS WITH. THAT DECIDES THE WANTED FUNCTION FROM THE SERVER.
        /* Server calls:
        1: CREATE {1, size, size, size, size}
        2: SET {2, id, id, id, id, size, size, size, size, value, value, ..., value} 
        3: GET {3, id, id, id, id}
        4: MODIFY COUNT {4, id, id, id, id, value, value, value, value}

        1: CREATE {1, int}
        2: SET {2, int, int, any} 
        3: GET {3, int}
        4: MODIFY COUNT {4, int, int}
        */
        std::vector<uint8_t> result_switch = {};
        switch (buffer[0]) {
            case 1:{
                std::cout << "Server received: Create request." << std::endl;
                int size = 0;
                std::memcpy(&size, &buffer[1], sizeof(int));

                // Returns de Id from Create
                int res_temp = heap->Create(size, 0);
                result_switch.resize(sizeof(int));
                memcpy(result_switch.data(), &res_temp, sizeof(int));
                break;}
            case 2:{
                std::cout << "Server received: Set request." << std::endl;
                int id = 0;
                std::memcpy(&id, &buffer[1], sizeof(int));
                int size = 0;
                std::memcpy(&size, &buffer[1+sizeof(int)], sizeof(int));
                std::vector<uint8_t> data(buffer + 1 + 2 * sizeof(int), buffer + 1 + 2 * sizeof(int) + size); // If size is too big, it may go outside of buffer's bounds (1024 bytes).

                heap->Set(id, size, data);
                break;}
            case 3:{
                std::cout << "Server received: Get request." << std::endl;
                int id = 0;
                std::memcpy(&id, &buffer[1], sizeof(int));
                std::vector<uint8_t> data = heap->Get(id);
                result_switch = data;
                break;}
            case 4:{
                std::cout << "Server received: ModifyCount request." << std::endl;
                int id = 0;
                std::memcpy(&id, &buffer[1], sizeof(int));
                int value = 0;
                std::memcpy(&value, &buffer[1+sizeof(int)], sizeof(int));

                heap->ModifyCount(id, value);
                break;}
        }

        // Send reply
        // send(new_socket, hello, strlen(hello), 0);
        if (!result_switch.empty()) {
            send(new_socket, result_switch.data(), result_switch.size(), 0);
            // std::cout << "result_switch" << result_switch[0] << std::endl;
            std::cout << "Reply from server: " << std::endl;
            for (uint8_t byte : result_switch) {
                std::cout << static_cast<int>(byte) << " ";
            }
            std::cout << " " << std::endl;
        } else {
            send(new_socket, hello, strlen(hello), 0);
            std::cout << "Hello message sent" << std::endl;
        }
        heap->ShowHex();
        

        close(new_socket);
    }

    close(server_fd);    
    
    // return 0;
}








