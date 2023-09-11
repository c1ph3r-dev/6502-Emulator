#include "../include/instruction_set.h"
#include "../include/cpu.h"

namespace EM6502
{
  void NOP(CPU* cpu, s32& cycles, MEM* memory)
  {
    cycles--;
  };

  void LDA_IM(CPU* cpu, s32& cycles, MEM* memory)
  {
    cpu->A = cpu->fetch_byte(cycles, *memory);
    cpu->lda_set_status();
  };

  void LDA_ZP(CPU* cpu, s32& cycles, MEM* memory)
  {
    Byte ZeroPageAddr = cpu->fetch_byte(cycles, *memory);
    cpu->A = cpu->read_byte(cycles, *memory, ZeroPageAddr);
    cpu->lda_set_status();
  }

  void LDA_ZPX(CPU* cpu, s32& cycles, MEM* memory)
  {
    Byte ZeroPageAddr = cpu->fetch_byte(cycles, *memory);
    ZeroPageAddr += cpu->X;
    cycles--;
    cpu->A = cpu->read_byte(cycles, *memory, ZeroPageAddr);
    cpu->lda_set_status();
  }

  void JSR(CPU* cpu, s32& cycles, MEM* memory)
  {
    Word SubAddr = cpu->fetch_word(cycles, *memory);
    memory->write_word(cycles, cpu->PC - 1, cpu->SP);
    cpu->SP += 2;
    cpu->PC = SubAddr;
    cycles--;
  }
}