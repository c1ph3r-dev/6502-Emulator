#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// http://www.obelisk.me.uk/6502/

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;

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
};

enum class opcodes : Byte
{
    INS_LDA_IM = 0xA9
};

struct CPU
{
    Word PC;        // Program counter
    Word SP;        // Stack pointer // ToDo: switch to Byte

    Byte A, X, Y;   // Registers

    Byte C : 1; // Status flag {Carry}
    Byte Z : 1; // Status flag {Zero}
    Byte I : 1; // Status flag {Interrupt Disable}
    Byte D : 1; // Status flag {Decimal Mode}
    Byte B : 1; // Status flag {Break Command}
    Byte V : 1; // Status flag {Overflow}
    Byte N : 1; // Status flag {Negative}

private:
    Byte fetch_byte(u32& cycles, MEM& memory)
    {
        auto Data = memory[PC];
        PC++;
        cycles--;
        return Data;
    }

public:
    void reset(MEM& memory)
    {
        PC = 0xFFFC;
        SP = 0x0100;
        C = Z = I = D = B = V = N = 0;
        A = X = Y = 0;
        memory.initialize();
    }

    void exec(u32 cycles, MEM& memory)
    {
        while(cycles > 0)
        {
            Byte Instruction = fetch_byte(cycles, memory);
            switch((opcodes)Instruction)
            {
                case opcodes::INS_LDA_IM:
                    A = fetch_byte(cycles, memory);
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                    break;
                default:
                    printf("Unknown opcode: %02X\n", Instruction);
                    break;
            }
        }
    }
};

int main()
{
    MEM memory;
    CPU cpu;
    cpu.reset(memory);
    // start - inline a little program
    memory[0xFFFC] = (Byte)opcodes::INS_LDA_IM;
    memory[0xFFFD] = 0x42;
    // end - inline a little program
    cpu.exec(2, memory);
    return 0;
}