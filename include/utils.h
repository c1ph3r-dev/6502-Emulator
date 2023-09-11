#ifndef EM_UTILS_H_
#define EM_UTILS_H_

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;
using s32 = signed int;

static constexpr u32 MAX_MEM = 1024 * 64;

enum class opcodes : Byte
{
    INS_LDA_IM = 0xA9,
    INS_LDA_ZP = 0xA5,
    INS_LDA_ZPX = 0xB5,
    INS_JSR = 0x20
};

#endif // EM_UTILS_H