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
    tests.push_back(test0);
    tests.push_back(test1);
    tests.push_back(test2);
    tests.push_back(test3);
    tests.push_back(test4);
    tests.push_back(test5);
    tests.push_back(test6);
    tests.push_back(test7);
    tests.push_back(test8);
    tests.push_back(test9);
    tests.push_back(test10);
    tests.push_back(test11);
    tests.push_back(test12);
    tests.push_back(test13);
    tests.push_back(test14);
    tests.push_back(test15);

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