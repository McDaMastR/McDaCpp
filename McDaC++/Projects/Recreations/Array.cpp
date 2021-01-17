#include <iostream>
#include <fstream>

#include "Basic.hpp"

namespace bsc
{
    template<class T, size_t S>
    class Array
    {
    public:
        unsigned int get_size()
        {
            return size;
        }

        Array operator + (Array sec_array)
        {
            T new_array[size + sec_array.get_size()];
            for(int i = 0; i < size; i++)
                new_array[i] = array[i];
            
            for(int i = size; i < size + sec_array.get_size(); i++)
                new_array[i] = sec_array[i];
            
            return new_array;
        }

        Array operator - (Array sec_array)
        {
            if(size - sec_array.get_size() < 0) 
                return -1;
            
            T new_array[size - sec_array.get_size()];
            for(int i = 0; i < size - sec_array.get_size(); i++)
                new_array[i] = array[i];
            
            return new_array;
        }

        T operator [] (int place)
        {
            return array[place];
        }

    private:
        unsigned int size = S;
        T array[S];
    };
}