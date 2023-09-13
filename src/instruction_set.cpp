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
    cpu->ld_set_status(cpu->A);
  };

  void LDA_ZP(CPU* cpu, s32& cycles, MEM* memory)
  {
    Byte ZeroPageAddr = cpu->fetch_byte(cycles, *memory);
    cpu->load_register(cycles, *memory, ZeroPageAddr, cpu->A);
  }

  void LDA_ZPX(CPU* cpu, s32& cycles, MEM* memory)
  {
    Byte ZeroPageAddr = cpu->fetch_byte(cycles, *memory);
    ZeroPageAddr += cpu->X;
    cycles--;
    cpu->load_register(cycles, *memory, ZeroPageAddr, cpu->A);
  }

  void LDA_ABS(CPU* cpu, s32& cycles, MEM* memory)
  {
    Word AbsAddr = cpu->fetch_word(cycles, *memory);
    cpu->load_register(cycles, *memory, AbsAddr, cpu->A);
  }

  void LDA_ABSX(CPU* cpu, s32& cycles, MEM* memory)
  {
    Word AbsAddr = cpu->fetch_word(cycles, *memory);
    Word AbsAddrX = AbsAddr + cpu->X;
    if (AbsAddrX - AbsAddr >= 0xFF)
      cycles--;
    cpu->load_register(cycles, *memory, AbsAddrX, cpu->A);
  }

  void LDA_ABSY(CPU* cpu, s32& cycles, MEM* memory)
  {
    Word AbsAddr = cpu->fetch_word(cycles, *memory);
    Word AbsAddrY = AbsAddr + cpu->Y;
    if (AbsAddrY - AbsAddr >= 0xFF)
      cycles--;
    cpu->load_register(cycles, *memory, AbsAddrY, cpu->A);
  }

  void LDA_INDX(CPU* cpu, s32& cycles, MEM* memory)
  {
    Byte ZPAddress = cpu->fetch_byte(cycles, *memory);
		ZPAddress += cpu->X;
		cycles--;
		Word EffectiveAddr = cpu->read_word(cycles, *memory, ZPAddress);
  	cpu->load_register(cycles, *memory, EffectiveAddr, cpu->A);
  }

  void LDA_INDY(CPU* cpu, s32& cycles, MEM* memory)
  {
    Byte ZPAddress = cpu->fetch_byte(cycles, *memory);
    Word EffectiveAddr = cpu->read_word(cycles, *memory, ZPAddress);
    Word EffectiveAddrY = EffectiveAddr + cpu->Y;
		if(EffectiveAddrY - EffectiveAddr >= 0xFF)
		  cycles--;
  	cpu->load_register(cycles, *memory, EffectiveAddrY, cpu->A);
  }

  void LDX_IM(CPU* cpu, s32& cycles, MEM* memory)
  {
    cpu->X = cpu->fetch_byte(cycles, *memory);
    cpu->ld_set_status(cpu->X);
  }

  void LDX_ZP(CPU* cpu, s32& cycles, MEM* memory)
  {
    Byte ZeroPageAddr = cpu->fetch_byte(cycles, *memory);
    cpu->load_register(cycles, *memory, ZeroPageAddr, cpu->X);
  }

  void LDX_ZPY(CPU* cpu, s32& cycles, MEM* memory)
  {
    Byte ZeroPageAddr = cpu->fetch_byte(cycles, *memory);
    ZeroPageAddr += cpu->Y;
    cycles--;
    cpu->load_register(cycles, *memory, ZeroPageAddr, cpu->X);
  }

  void LDX_ABS(CPU* cpu, s32& cycles, MEM* memory)
  {
    Word AbsAddr = cpu->fetch_word(cycles, *memory);
    cpu->load_register(cycles, *memory, AbsAddr, cpu->X);
  }

  void LDX_ABSY(CPU* cpu, s32& cycles, MEM* memory)
  {
    Word AbsAddr = cpu->fetch_word(cycles, *memory);
    Word AbsAddrY = AbsAddr + cpu->Y;
    if (AbsAddrY - AbsAddr >= 0xFF)
      cycles--;
    cpu->load_register(cycles, *memory, AbsAddrY, cpu->X);
  }

  void LDY_IM(CPU* cpu, s32& cycles, MEM* memory)
  {
    cpu->Y = cpu->fetch_byte(cycles, *memory);
    cpu->ld_set_status(cpu->Y);
  }

  void LDY_ZP(CPU* cpu, s32& cycles, MEM* memory)
  {
    Byte ZeroPageAddr = cpu->fetch_byte(cycles, *memory);
    cpu->load_register(cycles, *memory, ZeroPageAddr, cpu->Y);
  }

  void LDY_ZPX(CPU* cpu, s32& cycles, MEM* memory)
  {
    Byte ZeroPageAddr = cpu->fetch_byte(cycles, *memory);
    ZeroPageAddr += cpu->X;
    cycles--;
    cpu->load_register(cycles, *memory, ZeroPageAddr, cpu->Y);
  }

  void LDY_ABS(CPU* cpu, s32& cycles, MEM* memory)
  {
    Word AbsAddr = cpu->fetch_word(cycles, *memory);
    cpu->load_register(cycles, *memory, AbsAddr, cpu->Y);
  }

  void LDY_ABSX(CPU* cpu, s32& cycles, MEM* memory)
  {
    Word AbsAddr = cpu->fetch_word(cycles, *memory);
    Word AbsAddrX = AbsAddr + cpu->X;
    if (AbsAddrX - AbsAddr >= 0xFF)
      cycles--;
    cpu->load_register(cycles, *memory, AbsAddrX, cpu->Y);
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