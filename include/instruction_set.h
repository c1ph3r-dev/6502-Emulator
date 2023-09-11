#ifndef EM6502_INSTRUCTION_SET_H_
#define EM6502_INSTRUCTION_SET_H_

#include "utils.h"
#include <map>

namespace EM6502
{
  class CPU;
  class MEM;

  enum class opcodes : Byte
    {
        INS_NOP = 0xEA,
        INS_LDA_IM = 0xA9,
        INS_LDA_ZP = 0xA5,
        INS_LDA_ZPX = 0xB5,
        INS_LDA_ABS = 0xAD,
        INS_LDA_ABSX = 0xBD,
        INS_LDA_ABSY = 0xB9,
        INS_LDA_INDX = 0xA1,
        INS_LDA_INDY = 0xB1,
        INS_JSR = 0x20
    };

  typedef void(*INSTRUCTION)(CPU*, s32&, MEM*);

#define INSTRUCTION_PARAMS CPU*, s32&, MEM*

  void NOP(INSTRUCTION_PARAMS);
  void LDA_IM(INSTRUCTION_PARAMS);
  void LDA_ZP(INSTRUCTION_PARAMS);
  void LDA_ZPX(INSTRUCTION_PARAMS);
  void LDA_ABS(INSTRUCTION_PARAMS);
  void LDA_ABSX(INSTRUCTION_PARAMS);
  void LDA_ABSY(INSTRUCTION_PARAMS);
  void LDA_INDX(INSTRUCTION_PARAMS);
  void LDA_INDY(INSTRUCTION_PARAMS);
  void JSR(INSTRUCTION_PARAMS);
}

#endif // EM6502_INSTRUCTION_SET_H_