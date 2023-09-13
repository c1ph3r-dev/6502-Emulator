#include "../include/tests.h"
#include <iostream>

namespace EM6502
{
  void TESTS::RunTests(CPU& cpu, MEM& memory) const
  {
    int count{0};
    for(auto& test : tests)
    {
        if(RunTest(test, cpu, memory))
        {
            std::cout << count << '\n';
        }
        else
        {
            std::cout << count << " - TEST FAILED\n";
        }
        count++;
    }
  }

   static bool VerfifyUnmodifiedFlagsFromLD(const CPU& cpu, const CPU& cpu_copy)
    {
        return 
        cpu.C == cpu_copy.C && 
        cpu.I == cpu_copy.I && 
        cpu.D == cpu_copy.D && 
        cpu.B == cpu_copy.B && 
        cpu.V == cpu_copy.V;
    }

    // Test that determines if the CPU does nothing when we execute zero cycles
    static TEST CPU_ZERO_CYCLES_TEST = [](CPU cpu, MEM memory){
        //given:
    	constexpr s32 NUM_CYCLES = 0;

    	//when:
    	auto cycles_used = cpu.exec(NUM_CYCLES, memory);

    	//then:
    	return cycles_used == 0;
    };

    // Test that determines if the CPU can execute more cycles than requested if required by the instruction
    static TEST CPU_FULLY_COMPLETE_INSTRUCTION_WITH_LESS_CYCLES_TEST = [](CPU cpu, MEM memory){
        // given:
    	memory[0xFFFC] = (Byte)opcodes::INS_LDA_IM;
    	memory[0xFFFD] = 0x84;
    	constexpr s32 NUM_CYCLES = 1;

    	// when:
    	auto cycles_used = cpu.exec(NUM_CYCLES, memory);

        // then:
        return cycles_used == 2;
    };

    // Test that determines if LDA Immediate sets Zero flag when 0 is loaded into the A register
    static TEST LDA_IM_ZERO_TEST = [](CPU cpu, MEM memory){
         // given:
        memory[0xFFFC] = (Byte)opcodes::INS_LDA_IM;
        memory[0xFFFD] = 0x0;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(2, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.A == 0x0 && cpu.Z && !cpu.N && flags && cycles_used == 2;
    };

    // Test that determines if LDA Immediate can load a value into the A register
    static TEST LDA_IM_TEST = [](CPU cpu, MEM memory){
        // given:
        memory[0xFFFC] = (Byte)opcodes::INS_LDA_IM;
        memory[0xFFFD] = 0x84;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(2, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.A == 0x84 && !cpu.Z && cpu.N && flags && cycles_used == 2;
    };

    // Test that determines if LDA Zero Page can load a value into the A register
    static TEST LDA_ZP_TEST = [](CPU cpu, MEM memory){
        // given:
        memory[0xFFFC] = (Byte)opcodes::INS_LDA_ZP;
        memory[0xFFFD] = 0x42;
        memory[0x0042] = 0x37;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(3, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == 3;
    };

    // Test that determines if LDA Zero Page X can load a value into the A register
    static TEST LDA_ZPX_TEST = [](CPU cpu, MEM memory){
        // given:
        cpu.X = 5;
        memory[0xFFFC] = (Byte)opcodes::INS_LDA_ZPX;
        memory[0xFFFD] = 0x42;
        memory[0x0047] = 0x37;

        // when:
        CPU cpu_copy = cpu;

        auto cycles_used = cpu.exec(4, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == 4;
    };

    // Test that determines if LDA Zero Page X can load a value into the A register when it wraps
    static TEST LDA_ZPX_WRAP_TEST = [](CPU cpu, MEM memory){
        // given:
        cpu.X = 0xFF;
        memory[0xFFFC] = (Byte)opcodes::INS_LDA_ZPX;
        memory[0xFFFD] = 0x80;
        memory[0x007F] = 0x37;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(4, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == 4;
    };

    // Test that determines if NOP instruction works correctly
    static TEST NOP_TEST = [](CPU cpu, MEM memory){
        // given:
        memory[0xFFFC] = (Byte)opcodes::INS_NOP;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(2, memory);

        // then:
        return cpu_copy.PC == cpu.PC - 1 && cycles_used == 2;
    };

    // Test that determines if LDA Absolute can load a value into the A register
    static TEST LDA_ABS_TEST = [](CPU cpu, MEM memory){
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
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDA Absolute X can load a value into the A register
    static TEST LDA_ABSX_TEST = [](CPU cpu, MEM memory){
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
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDA Absolute X can load a value into the A register when it crosses a page boundary
    static TEST LDA_ABSX_CROSS_TEST = [](CPU cpu, MEM memory){
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
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDA Absolute Y can load a value into the A register
    static TEST LDA_ABSY_TEST = [](CPU cpu, MEM memory){
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
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDA Absolute Y can load a value into the A register when it crosses a page boundary
    static TEST LDA_ABSY_CROSS_TEST = [](CPU cpu, MEM memory){
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
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDA Indirect X can load a value into the A register
    static TEST LDA_INDX_TEST = [](CPU cpu, MEM memory){
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
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDA Indirect Y can load a value into the A register
    static TEST LDA_INDY_TEST = [](CPU cpu, MEM memory){
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
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDA Indirect Y can load a value into the A register when it crosses a page boundary
    static TEST LDA_INDY_CROSS_TEST = [](CPU cpu, MEM memory){
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
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.A == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDX Immediate sets Zero flag when 0 is loaded into the X register
    static TEST LDX_IM_ZERO_TEST = [](CPU cpu, MEM memory){
         // given:
        memory[0xFFFC] = (Byte)opcodes::INS_LDX_IM;
        memory[0xFFFD] = 0x0;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(2, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.X == 0x0 && cpu.Z && !cpu.N && flags && cycles_used == 2;
    };

    // Test that determines if LDX Immediate can load a value into the X register
    static TEST LDX_IM_TEST = [](CPU cpu, MEM memory){
        // given:
        memory[0xFFFC] = (Byte)opcodes::INS_LDX_IM;
        memory[0xFFFD] = 0x84;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(2, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.X == 0x84 && !cpu.Z && cpu.N && flags && cycles_used == 2;
    };

    // Test that determines if LDX Zero Page can load a value into the X register
    static TEST LDX_ZP_TEST = [](CPU cpu, MEM memory){
        // given:
        memory[0xFFFC] = (Byte)opcodes::INS_LDX_ZP;
        memory[0xFFFD] = 0x42;
        memory[0x0042] = 0x37;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(3, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.X == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == 3;
    }; 

    // Test that determines if LDX Zero Page Y can load a value into the X register when it wraps
    static TEST LDX_ZPY_WRAP_TEST = [](CPU cpu, MEM memory){
        // given:
        cpu.Y = 0xFF;
        memory[0xFFFC] = (Byte)opcodes::INS_LDX_ZPY;
        memory[0xFFFD] = 0x80;
        memory[0x007F] = 0x37;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(4, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.X == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == 4;
    };

    // Test that determines if LDX Absolute can load a value into the X register
    static TEST LDX_ABS_TEST = [](CPU cpu, MEM memory){
        // given:
        memory[0xFFFC] = (Byte)opcodes::INS_LDX_ABS;
        memory[0xFFFD] = 0x80;
        memory[0xFFFE] = 0x44; //0x4480
        memory[0x4480] = 0x37;
        constexpr s32 EXPECTED_CYCLES = 4;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(EXPECTED_CYCLES, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.X == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDX Absolute Y can load a value into the X register
    static TEST LDX_ABSY_TEST = [](CPU cpu, MEM memory){
        // given:
        cpu.Y = 1;
        memory[0xFFFC] = (Byte)opcodes::INS_LDX_ABSY;
        memory[0xFFFD] = 0x80;
        memory[0xFFFE] = 0x44;  //0x4480 + 0x0001
        memory[0x4481] = 0x37;
        constexpr s32 EXPECTED_CYCLES = 4;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(EXPECTED_CYCLES, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.X == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDX Absolute Y can load a value into the X register when it crosses a page boundary
    static TEST LDX_ABSY_CROSS_TEST = [](CPU cpu, MEM memory){
        // given:
        cpu.Y = 0xFF;
        memory[0xFFFC] = (Byte)opcodes::INS_LDX_ABSY;
        memory[0xFFFD] = 0x02;
        memory[0xFFFE] = 0x44;  //0x4402
        memory[0x4501] = 0x37; //0x4402+0xFF crosses page boundary!
        constexpr s32 EXPECTED_CYCLES = 5;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(EXPECTED_CYCLES, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.X == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDY Immediate sets Zero flag when 0 is loaded into the Y register
    static TEST LDY_IM_ZERO_TEST = [](CPU cpu, MEM memory){
         // given:
        memory[0xFFFC] = (Byte)opcodes::INS_LDY_IM;
        memory[0xFFFD] = 0x0;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(2, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.Y == 0x0 && cpu.Z && !cpu.N && flags && cycles_used == 2;
    };

    // Test that determines if LDY Immediate can load a value into the Y register
    static TEST LDY_IM_TEST = [](CPU cpu, MEM memory){
        // given:
        memory[0xFFFC] = (Byte)opcodes::INS_LDY_IM;
        memory[0xFFFD] = 0x84;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(2, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.Y == 0x84 && !cpu.Z && cpu.N && flags && cycles_used == 2;
    };

    // Test that determines if LDY Zero Page can load a value into the Y register
    static TEST LDY_ZP_TEST = [](CPU cpu, MEM memory){
        // given:
        memory[0xFFFC] = (Byte)opcodes::INS_LDY_ZP;
        memory[0xFFFD] = 0x42;
        memory[0x0042] = 0x37;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(3, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.Y == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == 3;
    }; 

    // Test that determines if LDY Zero Page X can load a value into the Y register when it wraps
    static TEST LDY_ZPX_WRAP_TEST = [](CPU cpu, MEM memory){
        // given:
        cpu.X = 0xFF;
        memory[0xFFFC] = (Byte)opcodes::INS_LDY_ZPX;
        memory[0xFFFD] = 0x80;
        memory[0x007F] = 0x37;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(4, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.Y == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == 4;
    };

    // Test that determines if LDY Absolute can load a value into the Y register
    static TEST LDY_ABS_TEST = [](CPU cpu, MEM memory){
        // given:
        memory[0xFFFC] = (Byte)opcodes::INS_LDY_ABS;
        memory[0xFFFD] = 0x80;
        memory[0xFFFE] = 0x44; //0x4480
        memory[0x4480] = 0x37;
        constexpr s32 EXPECTED_CYCLES = 4;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(EXPECTED_CYCLES, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.Y == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDY Absolute X can load a value into the Y register
    static TEST LDY_ABSX_TEST = [](CPU cpu, MEM memory){
        // given:
        cpu.X = 1;
        memory[0xFFFC] = (Byte)opcodes::INS_LDY_ABSX;
        memory[0xFFFD] = 0x80;
        memory[0xFFFE] = 0x44;  //0x4480 + 0x0001
        memory[0x4481] = 0x37;
        constexpr s32 EXPECTED_CYCLES = 4;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(EXPECTED_CYCLES, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.Y == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    // Test that determines if LDY Absolute X can load a value into the Y register when it crosses a page boundary
    static TEST LDY_ABSX_CROSS_TEST = [](CPU cpu, MEM memory){
        // given:
        cpu.X = 0xFF;
        memory[0xFFFC] = (Byte)opcodes::INS_LDY_ABSX;
        memory[0xFFFD] = 0x02;
        memory[0xFFFE] = 0x44;  //0x4402
        memory[0x4501] = 0x37; //0x4402+0xFF crosses page boundary!
        constexpr s32 EXPECTED_CYCLES = 5;

        // when:
        CPU cpu_copy = cpu;
        auto cycles_used = cpu.exec(EXPECTED_CYCLES, memory);

        // then:
        bool flags = VerfifyUnmodifiedFlagsFromLD(cpu, cpu_copy);
        return cpu.Y == 0x37 && !cpu.Z && !cpu.N && flags && cycles_used == EXPECTED_CYCLES;
    };

    void TESTS::InitializeTests()
  {
    tests.push_back(CPU_ZERO_CYCLES_TEST);
    tests.push_back(CPU_FULLY_COMPLETE_INSTRUCTION_WITH_LESS_CYCLES_TEST);
    tests.push_back(NOP_TEST);
    tests.push_back(LDA_IM_TEST);
    tests.push_back(LDA_ZP_TEST);
    tests.push_back(LDA_ZPX_TEST);
    tests.push_back(LDA_ZPX_WRAP_TEST);
    tests.push_back(LDA_ABS_TEST);
    tests.push_back(LDA_ABSX_TEST);
    tests.push_back(LDA_ABSX_CROSS_TEST);
    tests.push_back(LDA_ABSY_TEST);
    tests.push_back(LDA_ABSY_CROSS_TEST);
    tests.push_back(LDA_INDX_TEST);
    tests.push_back(LDA_INDY_TEST);
    tests.push_back(LDA_INDY_CROSS_TEST);
    tests.push_back(LDX_IM_TEST);
    tests.push_back(LDX_IM_ZERO_TEST);
    tests.push_back(LDX_ZP_TEST);
    tests.push_back(LDX_ZPY_WRAP_TEST);
    tests.push_back(LDX_ABS_TEST);
    tests.push_back(LDX_ABSY_TEST);
    tests.push_back(LDX_ABSY_CROSS_TEST);
    tests.push_back(LDY_IM_TEST);
    tests.push_back(LDY_IM_ZERO_TEST);
    tests.push_back(LDY_ZP_TEST);
    tests.push_back(LDY_ZPX_WRAP_TEST);
    tests.push_back(LDY_ABS_TEST);
    tests.push_back(LDY_ABSX_TEST);
    tests.push_back(LDY_ABSX_CROSS_TEST);
  }
}