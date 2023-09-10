#ifndef EM_CPU_H_
#define EM_CPU_H_

#include "utils.h"
#include "mem.h"
#include <stdio.h>
#include <stdlib.h>

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

#endif // EM_CPU_H_