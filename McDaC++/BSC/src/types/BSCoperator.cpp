#include "../include/BSCtypes.hpp"
#include "../include/BSCmath.hpp"
#include "../include/BSCmultiBool.hpp"

namespace bsc
{
    constexpr int128_t int128_t::operator+(const int128_t other)
    {
        int128_t temp;
        temp.v2 = v2 + other.v2;
        if(v1 + other.v1 > ULONG_MAX)
        {
            if(v2 == LONG_MAX)
            {
                temp.v1 = 0;
                temp.v2 = LONG_MIN;
                return temp;
            }
            temp.v1 = other.v1 - v1;
            temp.v2++;
            return temp;
        }
        temp.v1 = v1 + other.v1;
        return temp;
    }

    constexpr const int128_t int128_t::operator+(const int128_t other) const
    {
        int128_t temp;
        temp.v2 = v2 + other.v2;
        if(v1 + other.v1 > ULONG_MAX)
        {
            if(v2 == LONG_MAX)
            {
                temp.v1 = 0;
                temp.v2 = LONG_MIN;
                return temp;
            }
            temp.v1 = other.v1 - v1;
            temp.v2++;
            return temp;
        }
        temp.v1 = v1 + other.v1;
        return temp;
    }

    constexpr int128_t int128_t::operator-(const int128_t other)
    {
        int128_t temp;
        temp.v2 = v2 - other.v2;
        if(v1 < other.v1)
        {
            temp.v2--;
            temp.v1 = ULONG_MAX - other.v1 + v1 + 1;
            return temp;
        }
        temp.v1 = v1 - other.v1;
        return temp;
    }

    constexpr const int128_t int128_t::operator-(const int128_t other) const
    {
        int128_t temp;
        temp.v2 = v2 - other.v2;
        if(v1 < other.v1)
        {
            temp.v2--;
            temp.v1 = ULONG_MAX - other.v1 + v1 + 1;
            return temp;
        }
        temp.v1 = v1 - other.v1;
        return temp;
    }

    constexpr int128_t int128_t::operator*(const int128_t other)
    {
        if(!(other.v1 || other.v2) || !(v1 || v2))
            return {0, 0};
        int128_t temp;
        temp.v2 = v2 * other.v2;
        temp.v1 = v1;
        for(uint16_t i = 2; i < other.v1 + 1; i++)
        {
            if(temp.v1 + v1 > ULONG_MAX)
            {
                temp.v2++;
                temp.v1 = v1 + temp.v1 - ULONG_MAX;;
                continue;
            }
            temp.v1 += v1;
        }
        return temp;
    }
    constexpr const int128_t int128_t::operator*(const int128_t other) const
    {
        if(!(other.v1 || other.v2) || !(v1 || v2))
            return {0, 0};
        int128_t temp;
        temp.v2 = v2 * other.v2;
        temp.v1 = v1;
        for(uint16_t i = 2; i < other.v1 + 1; i++)
        {
            if(temp.v1 + v1 > ULONG_MAX)
            {
                temp.v2++;
                temp.v1 = v1 + temp.v1 - ULONG_MAX;;
                continue;
            }
            temp.v1 += v1;
        }
        return temp;
    }

    constexpr int128_t int128_t::operator/(const int128_t other) // Do after const / operator
    {
        
    }

    constexpr const int128_t int128_t::operator/(const int128_t other) const
    {
        if(!other)
        {
            try                         {throw std::exception();}
            catch(std::exception& e)    {std::cout << e.what() << '\n';}
        }
        if(!(*this))
        {
            return {0, 0};
        }
    }
}
