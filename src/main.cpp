#include "../include/cpu.h"

// http://www.obelisk.me.uk/6502/

bool RunTest(bool (* test)(CPU, MEM), const CPU& cpu, const MEM& memory)
{
    return test(cpu, memory);
}

int main()
{
    MEM memory;
    CPU cpu;
    cpu.reset(memory);

    auto test1 = [](CPU cpu, MEM memory){
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

    if(RunTest(test1, cpu, memory))
    {
        printf("Test 1 passed\n");
    }

    return 0;
}