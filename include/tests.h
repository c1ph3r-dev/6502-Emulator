#ifndef EM6502_TESTS_H_
#define EM6502_TESTS_H_

#include "cpu.h"

#include <string>
#include <vector>
#include <map>

namespace EM6502
{
    typedef bool(*TEST)(CPU, MEM);

    struct TESTS
    {
        std::vector<TEST> tests;

        void InitializeTests();
        void RunTests(CPU& cpu, MEM& memory) const;

        private:
        bool RunTest(TEST test, const CPU& cpu, const MEM& memory) const
        {
            return test(cpu, memory);
        }
    };
    
}
#endif // EM6502_TESTS_H_