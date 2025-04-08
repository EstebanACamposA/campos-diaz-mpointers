#ifndef MYHEADER_H
#define MYHEADER_H

#include <vector>
#include <string>
#include <cstring>
#include <cstdint>

//---------------------------------------------------------------------------------------------------------------------
// General functions
//---------------------------------------------------------------------------------------------------------------------

// Function declaration (prototype)
void helloWorldHeaderExample();  // Declare the function that will be defined in mysource.cpp

//---------------------------------------------------------------------------------------------------------------------
// GRPC
//---------------------------------------------------------------------------------------------------------------------
void RunServer(const std::string port);


//---------------------------------------------------------------------------------------------------------------------
// Memory Manager
//---------------------------------------------------------------------------------------------------------------------
class Word  // The name word is no longer correct since it was changed to store uint8_t
{
public:
    int id;
    uint8_t data;
    int type;
    int ref_count;
    Word(int id, uint8_t data, int type, int ref_count);
};

class Heap
{
    public:
        int size;
        Word* memory_space;
        Heap (int size);

        int FindAvailable(int size);
        
        int FindAvailableAux(int& size, int& current_pos, int& needed_space, int& found_pos);
        
        // Checks if a needed_space amount of Words are free in memory_space. 
        bool Available(int pos, int needed_space);
        // Receives a block's id and returns its absolute id in memory_space. 
        int FindId(int id);
        // Returns the size of a block given the absolute id of the first element.
        int GetSizeOfBlock(int id);

        // Readies a block in memory. Increments ref_count and modifies the type.
        // 0,0,0,0,4,5,6,7,8,9,
        int Create(int size, int type);
        
        // Recieves an id as the memory address to modify and an int[] value.
        // Fills the memory block with the contents of value. If the sizes don't match, the smaller one is taken.
        void Set(int id, int value_size, const std::vector<uint8_t>& value);
        // Returns a std::vector<int> with the int values of the block at id.
        std::vector<uint8_t> Get(int id);
        // // Adds value to the ref_count of each word of the block at id.

        // Adds value to the ref_count of the first word of the block at id. Returns the new value.
        int ModifyCount(int id, int value);

        // prints it. Data as char.
        void Show();

        // print it. Data as hex.
        void ShowHex();
        void ShowHex(std::string);

};
















template <typename T>
class DereferencedMpointer;

//---------------------------------------------------------------------------------------------------------------------
// Mpointer
//---------------------------------------------------------------------------------------------------------------------
template <typename T>
class Mpointer
{
    public:

        int id; // Analog of the memory address.
        Heap* target_memory_manager; // This has to be changed into server calls.

        Mpointer(int id)
        {
            this->id=id;
            this->target_memory_manager=nullptr;
        }

        void SetMemoryManager(Heap* target_memory_manager)
        {
            this->target_memory_manager = target_memory_manager;
        }

// template <typename T>
// T Mpointer<T>::Deserialize(std::vector<uint8_t> value)
// {
//     T recovered = default;
//     int smaller_size = sizeof(recovered);
//     if (smaller_size > sizeof)

//     std::memcpy(&recovered, value.data(), );
//     std::cout << "Recovered: " << recovered << std::endl;    
// }

        // Takes an std::vector<uint8_t>, converts it into a T type and returns it
        T Deserialize(std::vector<uint8_t> value)
        {
            // s s s s ;
            // a === 3td
            T recovered;
            int smaller_size = sizeof(T);
            int value_size = value.size();
            if (smaller_size > value_size) // Each byte is 1 unit of size. So the size of the vector is the same in element count and in memory.
                {smaller_size = value_size;}

            std::memcpy(&recovered, value.data(), smaller_size);
            std::cout << "Recovered: " << recovered << std::endl;    
            return recovered;
        }


        DereferencedMpointer<T> operator*()
        {
            DereferencedMpointer<T> dereferenced(this);
            return dereferenced;
        }


        // Takes a T value, converts it into an std::vector<uint8_t> and stores it in the target_memory_manager!!!
        void Serialize(T deserialized_value)
        {
            int size = sizeof(T);

            std::vector<uint8_t> data_for_block(size);
            std::memcpy(data_for_block.data(), &deserialized_value, size);

            this->target_memory_manager->Set(this->id, size, data_for_block);
        }

        // Serialize function already stores it (i. e. updated the block)
        // void UpdateBlock(T value)
        // {
            
        // }

        // returns: Mpointer->target_memory_manager->Get(Mpointer->id) (deserialized)
        T GetDereferencedValue()
        {
            std::cout << "ENTERED GetDereferencedValue() !!!!!!!!!!!!!!!!1" << std::endl;    
            
            
            std::vector<uint8_t> serialized_dereferenced_value = this->target_memory_manager->Get(this->id);

            for (size_t i = 0; i < serialized_dereferenced_value.size(); i++)
            {
                std::cout << "TRIES TO PRINT 111";    
                std::cout << serialized_dereferenced_value[i];
            }
            std::cout << "Size of serialized_dereferenced_value was" << serialized_dereferenced_value.size() << std::endl;    

            return Deserialize(serialized_dereferenced_value);
        }

        Mpointer& operator=(Mpointer right_hand_value)
        {
            this->id = right_hand_value.id;
            return *this;
        }
        

        void printId()
        {
            std::cout << "ID: " << id << std::endl;
        }
        
};







#endif  // MYHEADER_H