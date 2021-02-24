#include "../../include/BSCgrid.hpp"

namespace bsc
{
    template <class T, std::size_t S>
    grid<T, S>::grid()
	{
		array = nullptr;
	}

    template <class T, std::size_t S>
    grid<T, S>::~grid()
	{
		delete[] array;
	}

    template <class T, std::size_t S>
    grid<T, S>::grid(const grid &other)
	{

	}

    template <class T, std::size_t S>
    grid<T, S>::grid(const grid &&other)
	{

	}

    template <class T, std::size_t S>
    grid<T, S> grid<T, S>::operator=(const grid &other)
	{

	}

    template <class T, std::size_t S>
	grid<T, S>::grid(const std::size_t *lengths[S])
	{
		if(!S)
			array = nullptr;
		if(S == 1)
			array = new T[lengths[0]];
		if(S == 2)
			array = new T[lengths[0]][lengths[1]];
		if(S == 3)
			array = new T[lengths[0]][lengths[1]][lengths[2]];
		if(S == 4)
			array = new T[lengths[0]][lengths[1]][lengths[2]][lengths[3]];
		if(S == 5)
			array = new T[lengths[0]][lengths[1]][lengths[2]][lengths[3]][lengths[4]];
		if(S == 6)
			array = new T[lengths[0]][lengths[1]][lengths[2]][lengths[3]][lengths[4]][lengths[5]];	
	}
}