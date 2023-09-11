#ifndef EM6502_TESTS_H_
#define EM6502_TESTS_H_

#include "cpu.h"

#include <string>
#include <vector>
#include <map>

namespace EM6502
{
    typedef bool(*TEST)(CPU, MEM);

    static bool RunTest(TEST test, const CPU& cpu, const MEM& memory)
    {
        return test(cpu, memory);
    }

    static bool VerfifyUnmodifiedFlagsFromLDA(const CPU& cpu, const CPU& cpu_copy)
    {
        return 
        cpu.C == cpu_copy.C && 
        cpu.I == cpu_copy.I && 
        cpu.D == cpu_copy.D && 
        cpu.B == cpu_copy.B && 
        cpu.V == cpu_copy.V;
    }

    // Test that determines if the CPU does nothing when we execute zero cycles
    static TEST test0 = [](CPU cpu, MEM memory){
        //given:
    	constexpr s32 NUM_CYCLES = 0;

    	//when:
    	auto cycles_used = cpu.exec(NUM_CYCLES, memory);

    	//then:
    	return cycles_used == 0;
    };

    // Test that determines if the CPU can execute more cycles than requested if required by the instruction
    static TEST test1 = [](CPU cpu, MEM memory){
        // given:
    	memory[0xFFFC] = (Byte)opcodes::INS_LDA_IM;
    	memory[0xFFFD] = 0x84;
    	constexpr s32 NUM_CYCLES = 1;

    	// when:
    	auto cycles_used = cpu.exec(NUM_CYCLES, memory);

        // then:
        return cycles_used == 2;
    };

    // Test that determines if LDA Immediate sets Zero flag when 0 is loaded into the a register
    static TEST test2 = [](CPU cpu, MEM memory){
         // given:
        memory[0xFFFC] = (Byte)opcodes::INS_LDA_IM;
        memory[0xFFFD] = 0x0;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(2, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
        return cpu.A == 0x0 && cpu.Z && !cpu.N && flags && cycles_used == 2;
    };

    // Test that determines if LDA Immediate can load a value into the a register
    static TEST test3 = [](CPU cpu, MEM memory){
        // given:
        memory[0xFFFC] = (Byte)opcodes::INS_LDA_IM;
        memory[0xFFFD] = 0x84;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(2, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
        return cpu.A == 0x84 && !cpu.Z && cpu.N && flags && cycles_used == 2;
    };

    // Test that determines if LDA Zero Page can load a value into the a register
    static TEST test4 = [](CPU cpu, MEM memory){
        // given:
        memory[0xFFFC] = (Byte)opcodes::INS_LDA_ZP;
        memory[0xFFFD] = 0x42;
        memory[0x0042] = 0x37;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(3, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == 3;
    };

    // Test that determines if LDA Zero Page X can load a value into the a register
    static TEST test5 = [](CPU cpu, MEM memory){
        // given:
        cpu.X = 5;
        memory[0xFFFC] = (Byte)opcodes::INS_LDA_ZPX;
        memory[0xFFFD] = 0x42;
        memory[0x0047] = 0x37;

        // when:
        CPU cpu_copy = cpu;

        auto cycles_used = cpu.exec(4, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == 4;
    };

    // Test that determines if LDA Zero Page X can load a value into the a register when it wraps
    static TEST test6 = [](CPU cpu, MEM memory){
        // given:
        cpu.X = 0xFF;
        memory[0xFFFC] = (Byte)opcodes::INS_LDA_ZPX;
        memory[0xFFFD] = 0x80;
        memory[0x007F] = 0x37;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(4, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == 4;
    };

    // Test that determines if NOP instruction works correctly
    static TEST test7 = [](CPU cpu, MEM memory){
        // given:
        memory[0xFFFC] = (Byte)opcodes::INS_NOP;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(2, memory);

        // then:
        return cpu_copy.PC == cpu.PC - 1 && cycles_used == 2;
    };

    // Test that determines if LDA Absolute can load a value into the A register
    static TEST test8 = [](CPU cpu, MEM memory){
        // given:
        memory[0xFFFC] = (Byte)opcodes::INS_LDA_ABS;
        memory[0xFFFD] = 0x80;
        memory[0xFFFE] = 0x44; //0x4480
        memory[0x4480] = 0x37;
        constexpr s32 EXPECTED_CYCLES = 4;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(EXPECTED_CYCLES, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDA Absolute X can load a value into the A register
    static TEST test9 = [](CPU cpu, MEM memory){
        // given:
        cpu.X = 1;
        memory[0xFFFC] = (Byte)opcodes::INS_LDA_ABSX;
        memory[0xFFFD] = 0x80;
        memory[0xFFFE] = 0x44;  //0x4480 + 0x0001
        memory[0x4481] = 0x37;
        constexpr s32 EXPECTED_CYCLES = 4;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(EXPECTED_CYCLES, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDA Absolute X can load a value into the A register when it crosses a page boundary
    static TEST test10 = [](CPU cpu, MEM memory){
        // given:
        cpu.X = 0xFF;
        memory[0xFFFC] = (Byte)opcodes::INS_LDA_ABSX;
        memory[0xFFFD] = 0x02;
        memory[0xFFFE] = 0x44;  //0x4402
        memory[0x4501] = 0x37; //0x4402+0xFF crosses page boundary!
        constexpr s32 EXPECTED_CYCLES = 5;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(EXPECTED_CYCLES, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDA Absolute Y can load a value into the A register
    static TEST test11 = [](CPU cpu, MEM memory){
        // given:
        cpu.Y = 1;
        memory[0xFFFC] = (Byte)opcodes::INS_LDA_ABSY;
        memory[0xFFFD] = 0x80;
        memory[0xFFFE] = 0x44;  //0x4480 + 0x0001
        memory[0x4481] = 0x37;
        constexpr s32 EXPECTED_CYCLES = 4;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(EXPECTED_CYCLES, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDA Absolute Y can load a value into the A register when it crosses a page boundary
    static TEST test12 = [](CPU cpu, MEM memory){
        // given:
        cpu.Y = 0xFF;
        memory[0xFFFC] = (Byte)opcodes::INS_LDA_ABSY;
        memory[0xFFFD] = 0x02;
        memory[0xFFFE] = 0x44;  //0x4402
        memory[0x4501] = 0x37; //0x4402+0xFF crosses page boundary!
        constexpr s32 EXPECTED_CYCLES = 5;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(EXPECTED_CYCLES, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDA Indirect X can load a value into the A register
    static TEST test13 = [](CPU cpu, MEM memory){
        // given:
        cpu.X = 0x04;
        memory[0xFFFC] = (Byte)opcodes::INS_LDA_INDX;
        memory[0xFFFD] = 0x02;
        memory[0x0006] = 0x00;  //0x2 + 0x4
        memory[0x0007] = 0x80;
        memory[0x8000] = 0x37;
        constexpr s32 EXPECTED_CYCLES = 6;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(EXPECTED_CYCLES, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDA Indirect Y can load a value into the A register
    static TEST test14 = [](CPU cpu, MEM memory){
        // given:
        cpu.Y = 0x04;
        memory[0xFFFC] = (Byte)opcodes::INS_LDA_INDY;
        memory[0xFFFD] = 0x02;
        memory[0x0002] = 0x00;
        memory[0x0003] = 0x80;
        memory[0x8004] = 0x37;  //0x8000 + 0x4
        constexpr s32 EXPECTED_CYCLES = 5;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(EXPECTED_CYCLES, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDA Indirect Y can load a value into the A register when it crosses a page boundary
    static TEST test15 = [](CPU cpu, MEM memory){
        // given:
        cpu.Y = 0xFF;
        memory[0xFFFC] = (Byte)opcodes::INS_LDA_INDY;
        memory[0xFFFD] = 0x02;
        memory[0x0002] = 0x02;
        memory[0x0003] = 0x80;
        memory[0x8101] = 0x37;
        constexpr s32 EXPECTED_CYCLES = 6;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(EXPECTED_CYCLES, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };
}
#endif // EM6502_TESTS_H_