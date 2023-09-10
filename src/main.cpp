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

    /** Write 2 bytes */
    void write_word(u32& cycles, Word data, u32 address)
    {
        Data[address] = data & 0xFF;
        Data[address + 1] = (data >> 8);
        cycles -= 2;
    }
};

enum class opcodes : Byte
{
    INS_LDA_IM = 0xA9,
    INS_LDA_ZP = 0xA5,
    INS_LDA_ZPX = 0xB5,
    INS_JSR = 0x20
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

    Word fetch_word(u32& cycles, MEM& memory)
    {
        // 6502 is little endian
        Word Data = memory[PC];
        PC++;

        Data |= (memory[PC] << 8);
        PC++;

        cycles -= 2;

        return Data;
    }

    Byte read_byte(u32& cycles, MEM& memory, u32 address)
    {
        auto Data = memory[address];
        cycles--;
        return Data;
    }

    inline void lda_set_status()
    {
        Z = (A == 0);
        N = (A & 0b10000000) > 0;
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
                    {
                        A = fetch_byte(cycles, memory);
                        lda_set_status();
                    }
                    break;
                case opcodes::INS_LDA_ZP:
                    {
                        Byte ZeroPageAddr = fetch_byte(cycles, memory);
                        A = read_byte(cycles, memory, ZeroPageAddr);
                        lda_set_status();
                    }
                    break;
                case opcodes::INS_LDA_ZPX:
                {
                    Byte ZeroPageAddr = fetch_byte(cycles, memory);
                    ZeroPageAddr += X;
                    cycles--;
                    A = read_byte(cycles, memory, ZeroPageAddr);
                    lda_set_status();
                }
                break;
                case opcodes::INS_JSR:
                {
                    Word SubAddr = fetch_word(cycles, memory);
                    memory.write_word(cycles, PC - 1, SP);
                    PC = SubAddr;
                    SP++;
                    cycles--;
                }
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
    memory[0xFFFC] = (Byte)opcodes::INS_JSR;
    memory[0xFFFD] = 0x42;
    memory[0xFFFE] = 0x42;
    memory[0x4242] = (Byte)opcodes::INS_LDA_IM;
    memory[0x4243] = 0x84;
    // end - inline a little program
    cpu.exec(9, memory);
    return 0;
}