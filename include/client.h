#include <vector>
#include <string>
#include <cstring>
#include <cstdint>

std::vector<uint8_t> SendMessage(std::vector<uint8_t> message, int port);

extern int global_port;
// int global_port = 8080;