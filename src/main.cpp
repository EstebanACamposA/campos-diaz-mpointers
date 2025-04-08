#include <iostream>
#include "../include/memory_manager.h"
#include "../include/dereferenced_m_pointer.h"

#include <iomanip>  // for printing hex.


// #include "memory_manager_server.cpp"
// #include <grpcpp/grpcpp.h>
// #include "grpc_test.grpc.pb.h"
// #include "greeter.grpc.pb.h"
// #include "greeter.pb.h"


int main() {
    std::cout << "Hello from main.cpp!" << std::endl;
    Heap heap(16);


    int idecito = heap.Create(4, 1);
    std::vector<uint8_t> bytecito = {100,112,113,114};
    heap.Set(idecito, 4, bytecito);

    // std::vector<uint8_t> bytecito_retornado;
    // bytecito_retornado = heap.Get(idecito);
    // // heap.ModifyCount(idecito, 1); // Should do this until creating a new pointer for the same block.

    // for (size_t i = 0; i < 4; i++)
    // {
    //     std::cout << bytecito_retornado[i];
    // }


    int idecito2 = heap.Create(sizeof(long), 1);
    std::vector<uint8_t> bytecito2 = {88,77,66,55,44,33,22,11};
    heap.Set(idecito2, sizeof(long), bytecito2);

    int idecito3 = heap.Create(4, 1);
    std::vector<uint8_t> bytecito3 = {80,70,60,50};
    heap.Set(idecito3, 4, bytecito3);



    heap.Show();
    heap.ShowHex();

    Mpointer<int> mp(idecito);  // Instantiate Mpointer with int
    mp.printId();

    Mpointer<long> mp2(idecito2);  // Instantiate Mpointer with long.
    mp2.printId();

    Mpointer<int> mp3(idecito3);  // Instantiate Mpointer with int
    mp3.printId();

    mp.SetMemoryManager(&heap);
    mp2.SetMemoryManager(&heap);
    mp3.SetMemoryManager(&heap);

    /////////// mp using dptr manually.
    // DereferencedMpointer<int> dptr = *mp;

    // std::cout << "GOT HERE IN MAIN" << std::endl;

    
    // int a = dptr.original_m_pointer->GetDereferencedValue();
    // // int a = *mp;

    // std::cout << "int a = "<< a << std::endl;
    // std::cout << "data, " << std::hex << std::setw(2) << std::setfill('0') << a << std::endl;
    // std::cout << std::dec << std::endl;

    // int b = 2021558288;
    // dptr = b;


    // heap.ShowHex("SEGUNDO SHOW HEX. EL VALOR DE INDICE CERO DEBERIA DE SER EM.");

    // int c = dptr;
    // std::cout << "data c, " << std::hex << std::setw(2) << std::setfill('0') << c << std::endl;
    // std::cout << std::dec << std::endl;
    /////////// mp
    int barro = *mp;
    std::cout << "data a, " << std::hex << std::setw(2) << std::setfill('0') << barro << std::endl;
    std::cout << std::dec << std::endl;

    return 0;
}



