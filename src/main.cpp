#include "../include/cpu.h"
#include "../include/tests.h"
#include <iostream>

using namespace EM6502;

// https://web.archive.org/web/20210909190432/http://www.obelisk.me.uk/6502/

int main()
{
    MEM memory;
    CPU cpu;
    cpu.reset(memory);

    TESTS tests{};
    tests.InitializeTests();
    tests.RunTests(cpu, memory);

    return 0;
}