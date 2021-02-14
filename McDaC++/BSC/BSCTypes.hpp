#pragma once

namespace bsc
{
#if defined(__APPLE__)
    typedef char s1b; // Signed (s) One (1) Byte (b)
    typedef unsigned char u1b; // Unsigned (u)
    typedef short si2b; // Integral (i)
    typedef unsigned short ui2b;
    typedef int si4b;
    typedef unsigned int ui4b;
    typedef long si8b;
    typedef unsigned long ui8b;
    typedef float f4b; // Floating-point (f)
    typedef double f8b; // No unsigned floating-points
    typedef long double f16b;
    typedef bool b1b; // Boolean ((1st) b)
#endif // __APPLE__
}