#include "../../include/BSCgrid.hpp"

namespace bsc
{
    template <class T, std::size_t S>
    grid<T, S>::~grid()
    {
        delete[] array;
    }

    template <class T, std::size_t S>
    grid<T, S>::grid(const std::size_t *lengths[S])
        : lengths(*lengths)
    {
        switch (S) {
            case 0:
                array = nullptr;
                break;
            case 1:
                array = new T[lengths[0]];
                break;
            case 2:
                array = new T[lengths[0]][lengths[1]];
                break;
            case 3:
                array = new T[lengths[0]][lengths[1]][lengths[2]];
                break;
        }
    }
    
    template <class T, std::size_t S>
    std::size_t grid<T, S>::dimensions() const
    {
        return S;
    }

    template <class T, std::size_t S>
    std::size_t grid<T, S>::length(const std::size_t place) const
    {
        return lengths[place + 1];
    }
}
