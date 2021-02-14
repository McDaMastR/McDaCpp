#pragma once
#include <climits>

namespace bsc
{
#if defined(__APPLE__)
    typedef struct 
    {
        long v2; // 
        unsigned long v1;

        inline bsc::si16b operator + (bsc::si16b other)
        {
            if(v1 + other.v1 > ULONG_MAX)
            {
                if(v2 == LONG_MAX)
                {
                    v1 = 0;
                    v2 = 0;
                    return *this;
                }
                v1 = other.v1 - v1;
                v2++;
                return *this;
            }
            v1 += other.v1;
            return *this;
        }

        inline bsc::si16b operator - (bsc::si16b other)
        {
            if(v1 - other.v1)
            {
                
            }

            if(v2 - other.v2 < LONG_MIN)
            {

            }
        }
    } si16b;

    typedef struct
    {
        unsigned long v1;
        unsigned long v2;
    } ui16b;
#endif // __APPLE__
}