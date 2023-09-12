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
        INS_LDX_IM = 0xA2,
        INS_LDX_ZP = 0xA6,
        INS_LDX_ZPY = 0xB6,
        INS_LDX_ABS = 0xAE,
        INS_LDX_ABSY = 0xBE,
        INS_LDY_IM = 0xA0,
        INS_LDY_ZP = 0xA4,
        INS_LDY_ZPX = 0xB4,
        INS_LDY_ABS = 0xAC,
        INS_LDY_ABSX = 0xBC,
        INS_JSR = 0x20
    };

  typedef void(*INSTRUCTION)(CPU*, s32&, MEM*);

#define INSTRUCTION_PARAMS CPU*, s32&, MEM*

  // NOP
  void NOP(INSTRUCTION_PARAMS);
  // LDA
  void LDA_IM(INSTRUCTION_PARAMS);
  void LDA_ZP(INSTRUCTION_PARAMS);
  void LDA_ZPX(INSTRUCTION_PARAMS);
  void LDA_ABS(INSTRUCTION_PARAMS);
  void LDA_ABSX(INSTRUCTION_PARAMS);
  void LDA_ABSY(INSTRUCTION_PARAMS);
  void LDA_INDX(INSTRUCTION_PARAMS);
  void LDA_INDY(INSTRUCTION_PARAMS);
  // LDX
  void LDX_IM(INSTRUCTION_PARAMS);
  void LDX_ZP(INSTRUCTION_PARAMS);
  void LDX_ZPY(INSTRUCTION_PARAMS);
  void LDX_ABS(INSTRUCTION_PARAMS);
  void LDX_ABSY(INSTRUCTION_PARAMS);
  // LDY
  void LDY_IM(INSTRUCTION_PARAMS);
  void LDY_ZP(INSTRUCTION_PARAMS);
  void LDY_ZPX(INSTRUCTION_PARAMS);
  void LDY_ABS(INSTRUCTION_PARAMS);
  void LDY_ABSX(INSTRUCTION_PARAMS);
  // JSR
  void JSR(INSTRUCTION_PARAMS);
}

#endif // EM6502_INSTRUCTION_SET_H_