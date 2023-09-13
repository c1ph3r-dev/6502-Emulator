#ifndef EM6502_UTILS_H_
#define EM6502_UTILS_H_

namespace EM6502
{
    using Byte = unsigned char;
    using Word = unsigned short;
    using Register = Byte;

    using u32 = unsigned int;
    using s32 = signed int;

    static constexpr u32 MAX_MEM = 1024 * 64;
}
#endif // EM6502_UTILS_H