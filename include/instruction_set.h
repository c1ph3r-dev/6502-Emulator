#ifndef EM6502_INSTRUCTION_SET_H_
#define EM6502_INSTRUCTION_SET_H_

#include "utils.h"
#include <map>

namespace EM6502
{
  class CPU;
  class MEM;

  typedef void(*INSTRUCTION)(CPU*, s32&, MEM*);

  void NOP(CPU* cpu, s32& cycles, MEM* memory);
  void LDA_IM(CPU* cpu, s32& cycles, MEM* memory);
  void LDA_ZP(CPU* cpu, s32& cycles, MEM* memory);
  void LDA_ZPX(CPU* cpu, s32& cycles, MEM* memory);
  void JSR(CPU* cpu, s32& cycles, MEM* memory);
}

#endif // EM6502_INSTRUCTION_SET_H_