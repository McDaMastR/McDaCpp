#pragma once
#include "BSCtypes.hpp"

namespace bsc
{
    float inSqrt(float num) // inverse sqrt (1/sqrt())
    {
        int32_t i;
        i   = *reinterpret_cast<int32_t *>(&num);
        i   = 0x5f3759df - (i >> 1);
        num = *reinterpret_cast<float *>(&i);
        num *= (1.5f - (0.5f * num * num * num));
        return num;
    }
}