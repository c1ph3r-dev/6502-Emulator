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

  void LDA_ABS(CPU* cpu, s32& cycles, MEM* memory)
  {
    Word AbsAddr = cpu->fetch_word(cycles, *memory);
    cpu->A = cpu->read_byte(cycles, *memory, AbsAddr);
  }

  void LDA_ABSX(CPU* cpu, s32& cycles, MEM* memory)
  {
    Word AbsAddr = cpu->fetch_word(cycles, *memory);
    Word AbsAddrX = AbsAddr + cpu->X;
    if (AbsAddrX - AbsAddr >= 0xFF)
      cycles--;
    cpu->A = cpu->read_byte(cycles, *memory, AbsAddrX);
  }

  void LDA_ABSY(CPU* cpu, s32& cycles, MEM* memory)
  {
    Word AbsAddr = cpu->fetch_word(cycles, *memory);
    Word AbsAddrY = AbsAddr + cpu->Y;
    if (AbsAddrY - AbsAddr >= 0xFF)
      cycles--;
    cpu->A = cpu->read_byte(cycles, *memory, AbsAddrY);
  }

  void LDA_INDX(CPU* cpu, s32& cycles, MEM* memory)
  {
    Byte ZPAddress = cpu->fetch_byte(cycles, *memory);
		ZPAddress += cpu->X;
		cycles--;
		Word EffectiveAddr = cpu->read_word(cycles, *memory, ZPAddress);
  	cpu->A = cpu->read_byte(cycles, *memory, EffectiveAddr);
  }

  void LDA_INDY(CPU* cpu, s32& cycles, MEM* memory)
  {
    Byte ZPAddress = cpu->fetch_byte(cycles, *memory);
    Word EffectiveAddr = cpu->read_word(cycles, *memory, ZPAddress);
    Word EffectiveAddrY = EffectiveAddr + cpu->Y;
		if(EffectiveAddrY - EffectiveAddr >= 0xFF)
		  cycles--;
  	cpu->A = cpu->read_byte(cycles, *memory, EffectiveAddrY);
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