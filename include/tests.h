#ifndef EM_TESTS_H_
#define EM_TESTS_H_

#include "cpu.h"

#include <string>
#include <vector>
#include <map>

typedef bool(*TEST)(CPU, MEM);

static bool RunTest(TEST test, const CPU& cpu, const MEM& memory)
{
    return test(cpu, memory);
}

static bool VerfifyUnmodifiedFlagsFromLDA(const CPU& cpu, const CPU& cpu_copy)
{
    return cpu.C == cpu_copy.C && cpu.I == cpu_copy.I && cpu.D == cpu_copy.D && cpu.B == cpu_copy.B && cpu.V == cpu_copy.V;
}

// Test that determines if LDA Immediate sets Zero flag when 0 is loaded into the a register
static TEST test0 = [](CPU cpu, MEM memory){
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
static TEST test1 = [](CPU cpu, MEM memory){
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
static TEST test2 = [](CPU cpu, MEM memory){
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
TEST test3 = [](CPU cpu, MEM memory){
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
TEST test4 = [](CPU cpu, MEM memory){
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

// Test that determines if the CPU does nothing when we execute zero cycles
TEST test5 = [](CPU cpu, MEM memory){
    //given:
	constexpr s32 NUM_CYCLES = 0;

	//when:
	auto cycles_used = cpu.exec(NUM_CYCLES, memory);

	//then:
	return cycles_used == 0;
};

// Test that determines if the CPU can execute more cycles than requested if required by the instruction
TEST test6 = [](CPU cpu, MEM memory){
    // given:
	memory[0xFFFC] = (Byte)opcodes::INS_LDA_IM;
	memory[0xFFFD] = 0x84;
	constexpr s32 NUM_CYCLES = 1;

	// when:
	auto cycles_used = cpu.exec(NUM_CYCLES, memory);

    // then:
    return cycles_used == 2;
};

#endif // EM_TESTS_H_