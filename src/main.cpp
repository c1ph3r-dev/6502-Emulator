#include "../include/cpu.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

// http://www.obelisk.me.uk/6502/

typedef bool(*TEST)(CPU, MEM);

bool RunTest(TEST test, const CPU& cpu, const MEM& memory)
{
    return test(cpu, memory);
}

int main()
{
    MEM memory;
    CPU cpu;
    cpu.reset(memory);

    std::vector<TEST> tests;
    std::map<TEST, std::string> test_desc;

    // Test that determines if LDA Immediate can load a value into the a register
    TEST test1 = [](CPU cpu, MEM memory){
        // start - inline a little program
        memory[0xFFFC] = (Byte)opcodes::INS_JSR;
        memory[0xFFFD] = 0x42;
        memory[0xFFFE] = 0x42;
        memory[0x4242] = (Byte)opcodes::INS_LDA_IM;
        memory[0x4243] = 0x84;
        // end - inline a little program
        cpu.exec(8, memory);

        return cpu.A == 0x84;
    };

    tests.push_back(test1);
    test_desc[test1] = "LDA Immediate can load a value into the a register\n";

    for(auto& test : tests)
    {
        if(RunTest(test, cpu, memory))
        {
            std::cout << test_desc[test];
        }
    }

    return 0;
}