#ifndef EM_MEMORY_H_
#define EM_MEMORY_H_

#include "utils.h"
#include <assert.h>

struct MEM
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void initialize()
    {
        for (auto& data : Data)
            data = 0;
    }

    /** Read 1 byte */
    Byte operator[](u32 address) const
    { 
        assert(address < MAX_MEM);
        return Data[address]; 
    } 

    /** Write 1 byte */
    Byte& operator[](u32 address) 
    { 
        assert(address < MAX_MEM);
        return Data[address]; 
    } 

    /** Write 2 bytes */
    void write_word(s32& cycles, Word data, u32 address)
    {
        Data[address] = data & 0xFF;
        Data[address + 1] = (data >> 8);
        cycles -= 2;
    }
};

#endif // EM_MEMORY_H_