#include "../include/cpu.h"
#include "../include/tests.h"
#include <iostream>


// http://www.obelisk.me.uk/6502/

int main()
{
    MEM memory;
    CPU cpu;
    cpu.reset(memory);

    std::vector<TEST> tests;
    std::map<TEST, std::string> test_desc;

    tests.push_back(test1);
    test_desc[test1] = "LDA Immediate can load a value into the a register";

    tests.push_back(test2);
    test_desc[test2] = "LDA Zero Page can load a value into the a register";
    
    tests.push_back(test3);
    test_desc[test3] = "LDA Zero Page X can load a value into the a register";
    
    tests.push_back(test4);
    test_desc[test4] = "LDA Zero Page X can load a value into the a register when it wraps";

    for(auto& test : tests)
    {
        if(RunTest(test, cpu, memory))
        {
            std::cout << test_desc[test] << '\n';
        }
        else
        {
            std::cout << test_desc[test] << " - TEST FAILED\n";
        }
    }

    return 0;
}