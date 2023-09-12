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

    std::vector<TEST> tests;
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

    return 0;
}