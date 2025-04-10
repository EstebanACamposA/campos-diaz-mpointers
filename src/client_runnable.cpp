#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../include/memory_manager.h"
#include "../include/dereferenced_m_pointer.h"

#include <vector>

#include "client.h"



int main(int argc, char const *argv[])
{
    Mpointer<int> mp;

    // DereferencedMpointer<int> dmp = *mp;
    *mp = 810;
    std::cout << "asignacion" << std::endl;

    int a = 1023;

    Mpointer<int> mp2;
    *mp2 = 1024;

    a = *mp2;
    std::cout << "int a = " << a << std::endl;





    return 0;
}
