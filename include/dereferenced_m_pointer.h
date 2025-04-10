#ifndef DEREFERENCE_H
#define DEREFERENCE_H

#include <vector>
#include <string>
#include <cstring>
#include <cstdint>

// #include <grpcpp/grpcpp.h>
// #include "greeter.grpc.pb.h"
// #include "greeter.pb.h"



template <typename T>
class Mpointer;


///////////////////////////////////////////////
// DereferencedMpointer is used to define behaviour for both *mptr = var and var = *mptr
// as oposed to what it had for mptr = var and var = mptr // This is for when it has the asterisks.
template <typename T>
class DereferencedMpointer
{
    public:

        Mpointer<T>* original_m_pointer;

        DereferencedMpointer(Mpointer<T>* original_m_pointer)
        {
            // this->id=id;
            // this->target_memory_manager=target_memory_manager;
            this->original_m_pointer=original_m_pointer;
        }

        /* When var = this.
        i. e. a variable wants to take the value the pointer's pointing to.
        e. g.: int a = *m_ptr */
        operator T() const
        {
            return this->original_m_pointer->GetDereferencedValue();
        }

        /* When this = var.
        i. e. The pointer's value is being assigned that of another variable.
        e. g.: *m_ptr = int a*/
        DereferencedMpointer<T>& operator=(T right_hand_value)
        {
            this->original_m_pointer->Serialize(right_hand_value);
            return *this;
        }

};






#endif  // DEREFERENCE_H