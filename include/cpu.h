#ifndef EM6502_CPU_H_
#define EM6502_CPU_H_

#include "utils.h"
#include "mem.h"
#include <stdio.h>
#include <stdlib.h>

namespace EM6502
{
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
        Byte fetch_byte(s32& cycles, MEM& memory)
        {
            auto Data = memory[PC];
            PC++;
            cycles--;
            return Data;
        }
    
        Word fetch_word(s32& cycles, MEM& memory)
        {
            // 6502 is little endian
            Word Data = memory[PC];
            PC++;
    
            Data |= (memory[PC] << 8);
            PC++;
    
            cycles -= 2;
    
            return Data;
        }
    
        Byte read_byte(s32& cycles, MEM& memory, u32 address)
        {
            auto Data = memory[address];
            cycles--;
            return Data;
        }
    
        Word read_word(s32& cycles, MEM& memory, u32 address)
        {
            Byte LoByte = read_byte(cycles, memory, address);
            Byte HiByte = read_byte(cycles, memory, address + 1);
            return LoByte | (HiByte << 8);
        }
    
        inline void lda_set_status()
        {
            Z = (A == 0);
            N = (A & 0b10000000) > 0;
        }
    
        void reset(Word ResetVector, MEM& memory)
    	{
    		PC = ResetVector;
    		SP = 0x0100;
    		C = Z = I = D = B = V = N = 0;
    		A = X = Y = 0;
    		memory.initialize();
    	}
    
    public:
        /**
         * @brief resets the cpu's PC, SP and registers and also initializes the memory
         * 
         * @param memory MEM object to initialize
         */
        void reset(MEM& memory)
        {
            reset(0xFFFC, memory);
        }
    
        /**
         * @brief executes a program stored in a MEM object
         * 
         * @param cycles: number of cycles the program takes to execute e.g. LDA Immediate uses 2 cycles
         * @param memory: MEM object containing the program instructions and data to be executed
         * 
         * @return the number of cycles that were used */
        s32 exec(s32 cycles, MEM& memory)
        {
            const s32 CyclesRequested = cycles;
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
                        SP += 2;
                        PC = SubAddr;
                        cycles--;
                    }
                    break;
                    default:
                        printf("Unknown opcode: %02X\n", Instruction);
                        break;
                }
            }
            const s32 NumCyclesUsed = CyclesRequested - cycles;
            return NumCyclesUsed;
        }
    };
}
#endif // EM6502_CPU_H_