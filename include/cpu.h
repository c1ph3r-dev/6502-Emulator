#ifndef EM6502_CPU_H_
#define EM6502_CPU_H_

#include "utils.h"
#include "mem.h"
#include "instruction_set.h"
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
        std::map<opcodes, INSTRUCTION> instructions;

        void set_instructions()
        {
            // ADD INSTRUCTIONS FUNCTION TO MAP USING OPCODE AS KEY TO ENABLE HANDLING OF INSTRUCTION
            instructions[opcodes::INS_NOP] = NOP;
            instructions[opcodes::INS_LDA_IM] = LDA_IM;
            instructions[opcodes::INS_LDA_ZP] = LDA_ZP;
            instructions[opcodes::INS_LDA_ZPX] = LDA_ZPX;
            instructions[opcodes::INS_LDA_ABS] = LDA_ABS;
            instructions[opcodes::INS_LDA_ABSX] = LDA_ABSX;
            instructions[opcodes::INS_LDA_ABSY] = LDA_ABSY;
            instructions[opcodes::INS_LDA_INDX] = LDA_INDX;
            instructions[opcodes::INS_LDA_INDY] = LDA_INDY;
            instructions[opcodes::INS_LDX_IM] = LDX_IM;
            instructions[opcodes::INS_LDX_ZP] = LDX_ZP;
            instructions[opcodes::INS_LDX_ZPY] = LDX_ZPY;
            instructions[opcodes::INS_LDX_ABS] = LDX_ABS;
            instructions[opcodes::INS_LDX_ABSY] = LDX_ABSY;
            instructions[opcodes::INS_LDY_IM] = LDY_IM;
            instructions[opcodes::INS_LDY_ZP] = LDY_ZP;
            instructions[opcodes::INS_LDY_ZPX] = LDY_ZPX;
            instructions[opcodes::INS_LDY_ABS] = LDY_ABS;
            instructions[opcodes::INS_LDY_ABSX] = LDY_ABSX;
            instructions[opcodes::INS_JSR] = JSR;
        }

        void reset(Word ResetVector, MEM& memory)
    	{
    		PC = ResetVector;
    		SP = 0x0100;
    		C = Z = I = D = B = V = N = 0;
    		A = X = Y = 0;
            set_instructions();
    		memory.initialize();
    	}
    public:

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

        Byte read_byte(s32& cycles, MEM& memory, Word address)
        {
            auto Data = memory[address];
            cycles--;
            return Data;
        }

        Word read_word(s32& cycles, MEM& memory, Word address)
        {
            Byte LoByte = read_byte(cycles, memory, address);
            Byte HiByte = read_byte(cycles, memory, address + 1);
            return LoByte | (HiByte << 8);
        }

        inline void ld_set_status(Byte& reg)
        {
            Z = (reg == 0);
            N = (reg & 0b10000000) > 0;
        }

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
                if (instructions.count((opcodes)Instruction))
                    instructions[(opcodes)Instruction](this, cycles, &memory);
                else 
                {
                    printf("Unhandled instruction: %02X\n", Instruction);
                    throw -1;
                }
            }
            const s32 NumCyclesUsed = CyclesRequested - cycles;
            return NumCyclesUsed;
        }
    };
}
#endif // EM6502_CPU_H_