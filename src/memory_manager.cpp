#include <iostream>
#include "memory_manager.h" // Include the header where the function is declared
#include <vector>
#include <cstring>
#include <cstdint>
#include <iomanip>

///???
// #include <grpcpp/grpcpp.h>
// #include "greeter.grpc.pb.h"
// #include "greeter.pb.h"
//???

/*
There are 4 byte (int) size words. The words also have some metadata. Word(int id, int data, int type)
A block is a variable size series of adjacent words that make up a variable. A variable can have unused space if specified in Create(size, type)
The Heap is a huge series of words. Metadata, also stored in the Heap class, is responsible for determining the blocks
*/

/////////////////
// Funcion para probar los headers.
void helloWorldHeaderExample()
{
    std::cout << "Esta funcion se declara en memory manager de source y lo del header hace que todo el mundo lo pueda ver." << std::endl;
}
/////////////////

//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
// Memory Manager
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
Word::Word(int id, uint8_t data, int type, int ref_count)
    : id(id), data(data), type(type), ref_count(ref_count) {}

//---------------------------------------------------------------------------------------------------------------------
// Heap
//---------------------------------------------------------------------------------------------------------------------
Heap::Heap(int size)
    : size(size), memory_space((Word*)malloc(size * sizeof(Word)))
{
    if (!memory_space)
    {
        std::cerr << "memory_space pointer is null at Heap initialization ):\n";
    }
    for (int i = 0; i < size; i++)
    {
        memory_space[i] = Word(i, 63, 0, 0);  // Initialize
    }
}

// Finds the index of the first consecutive size amount of Word's in memory_space. An available block.
// e.g.: [f,t,f,f,t,t,f,f,f,...] If size == 2, returns 4 (the ...,t,t,...'s index)
int Heap::FindAvailable(int size)
{
    int current_pos = 0;
    int found_pos = 0;
    int needed_space = size;
    return FindAvailableAux(size, current_pos, size, found_pos);
}

// Recursive. Actually finds the index in found_pos.
int Heap::FindAvailableAux(int& size, int& current_pos, int& needed_space, int& found_pos)
{
    if(needed_space == 0)
    {
        return found_pos;
    }
    if(memory_space[current_pos].ref_count == 0)
    {
        current_pos++;
        needed_space--;
        return FindAvailableAux(size, current_pos, needed_space, found_pos);
    }
    needed_space = size;
    current_pos++;
    found_pos = current_pos;
    return FindAvailableAux(size, current_pos, needed_space, found_pos);
}

// Checks if a needed_space amount of Words are free in memory_space. 
bool Heap::Available(int pos, int needed_space)
{
    if(needed_space == 0)
    {
        return true;
    }
    if(memory_space[pos].ref_count == 0)
    {
        return Available(pos+1, needed_space -1);
    }
    return false;
}

// Receives a block's id and returns its absolute id in memory_space. 
int Heap::FindId(int id)
{
    std::cout << "this pointer: " << this << std::endl;
    std::cout << "ENTERED FindId ^^^^^^^^^^; " << "id = " << id << std::endl;
    std::cout << "ENTERED FindId ^^^^^^^^^^; " << "this->size = " << this->size << std::endl;
    for (size_t i = 0; i < this->size; i++)
    {
        if (memory_space[i].id == id && memory_space[i].ref_count > 0)
        {
            return i;
        }
    }
    std::cout << "Failed to FindId. id for a memory block not found. Returns -1" << std::endl;
    return -1;
}

// Returns the size of a block given the absolute id of the first element.
int Heap::GetSizeOfBlock(int id)
{
    int block_id = memory_space[id].id;
    int size = 0;
    while (memory_space[id].ref_count > 0 && memory_space[id].id == block_id)
    {
        size++;
        id++;
    }
    return size;
}

// Readies a block in memory. Increments ref_count and modifies the type.
// 0,0,0,0,4,5,6,7,8,9,
int Heap::Create(int size, int type)
{
    int id = FindAvailable(size);
    for (size_t i = 0; i < size; i++)
    {
        memory_space[i+id].id = id;
        memory_space[i+id].ref_count ++; // This can be done in another function as requirement.
        memory_space[i+id].type = type;
    }
    // phase -= size;
    return id;
}

// Recieves an id as the memory address to modify and an int[] value.
// Fills the memory block with the contents of value. If the sizes don't match, the smaller one is taken.
void Heap::Set(int id, int value_size, const std::vector<uint8_t>& value)
{
    // Finds whether the value or the memory block is the shorter array and gets the absolute id of the first position in the block.
    int smaller_size = value_size;
    int absolute_id = FindId(id);
    int block_size = GetSizeOfBlock(absolute_id);
    if (block_size < smaller_size) smaller_size = block_size;
    // fills the block without going out of limit in either array.
    for (size_t i = 0; i < smaller_size; i++)
    {
        memory_space[absolute_id + i].data = value[i];
    }
}

// Returns a std::vector<int> with the int values of the block at id.
std::vector<uint8_t> Heap::Get(int id)
{
    std::cout << "ENTERED Get() !!!!!!!!!!!!!!!!2" << std::endl;
    std::cout << "0 id = " << id << std::endl;
    int absolute_id = FindId(id);
    std::cout << "1 id = " << id << std::endl;
    std::cout << "1 absolute_id = " << absolute_id << std::endl;
    int block_size = GetSizeOfBlock(absolute_id);
    std::cout << "2 id = " << id << std::endl;
    std::cout << "2 absolute_id = " << absolute_id << std::endl;
    std::vector<uint8_t> block_data(block_size);
    for (size_t i = 0; i < block_size; i++)
    {
        block_data[i] = memory_space[i+absolute_id].data;
    }
    std::cout << "EXITED Get() !!!!!!!!!!!!!!!!2??" << std::endl;
    return block_data;  // Returning a vector
}

// // Adds value to the ref_count of each word of the block at id.
// int ModifyCount(int id, int value)
// {
//     int absolute_id = FindId(id);
//     int block_size = GetSizeOfBlock(absolute_id);
//     for (size_t i = 0; i < block_size; i++)
//     {
//         memory_space[i+absolute_id].ref_count += value;
//     }
// }
// Adds value to the ref_count of the first word of the block at id. Returns the new value.
int Heap::ModifyCount(int id, int value)
{
    int absolute_id = FindId(id);
    return memory_space[absolute_id].ref_count += value;
}

// prints it.
void Heap::Show()
{
    for (size_t i = 0; i < size; i++)
    {
        std::cout << "id, " << memory_space[i].id << ";\tdata, " << memory_space[i].data << ";\ttype, " << memory_space[i].type << ";\tref_count, " << memory_space[i].ref_count << std::endl;
        // std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(memory_space[i].data) << " ";
    }   
}

void Heap::ShowHex()
{
    for (size_t i = 0; i < size; i++)
    {
        std::cout << "id, " << memory_space[i].id << ";\tdata, " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(memory_space[i].data) << ";\ttype, " << memory_space[i].type << ";\tref_count, " << memory_space[i].ref_count << std::endl;
        // std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(memory_space[i].data) << " ";
    }
    std::cout << std::dec << std::endl;
}
void Heap::ShowHex(std::string message)
{
    std::cout << message << std::endl;
    for (size_t i = 0; i < size; i++)
    {
        std::cout << "id, " << memory_space[i].id << ";\tdata, " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(memory_space[i].data) << ";\ttype, " << memory_space[i].type << ";\tref_count, " << memory_space[i].ref_count << std::endl;
        // std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(memory_space[i].data) << " ";
    }
    std::cout << std::dec << std::endl;
}



//---------------------------------------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------------------------------------




