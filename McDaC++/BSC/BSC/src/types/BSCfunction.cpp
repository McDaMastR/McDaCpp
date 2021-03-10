#include "../../include/BSCtypes.hpp"

namespace bsc
{
	template <class T, std::size_t S>
	returnArray<T, S>::returnArray(const T array[S]) noexcept
	{
		for(std::size_t i = 0; i < S; i++)
			this->array[i] = array[i];
	}

	template <std::size_t S>
	[[nodiscard]] multiBool<S>::multiBool(const bool array[S]) noexcept
	{
		for(std::size_t i = 0; i < S; i++)
			bits[i] = 0;
		for(std::size_t i = 0; i < S * 8; i++)
			if(array[i])
			{
				std::size_t result = 1;
				for(std::size_t j = 0; j < i + 1; j++)
					result *= 2;
				result--;
				bits[i / 8] += result;
			}
	}
	template <std::size_t S>
	[[nodiscard]] multiBool<S>::multiBool(const multiBool &other) noexcept
	{
		for(std::size_t i = 0; i < S; i++)
			bits[i] = other.pureBits().array[i];
	}
	template <std::size_t S>
	[[nodiscard]] multiBool<S>::multiBool(const multiBool &&other) noexcept
	{
		for(std::size_t i = 0; i < S; i++)
			bits[i] = other.pureBits().array[i];
	}

	template <std::size_t S>
	returnArray<uint8_t, S> &multiBool<S>::pureBits() noexcept
	{
		return {bits};
	}
	template <std::size_t S>
	const returnArray<uint8_t, S> multiBool<S>::pureBits() const noexcept
	{
		return {bits};
	}

	template <std::size_t S>
	uint8_t &multiBool<S>::pureByte(const size_t index) noexcept
	{
		return index < S + 1 ? bits[index] : 0;
	}
	template <std::size_t S>
	const uint8_t multiBool<S>::pureByte() const noexcept
	{
		return index < S + 1 ? bits[index] : 0;
	}

	template <std::size_t S>
	void multiBool<S>::manipBits(const uint8_t array[S]) noexcept
	{
		for(std::size_t i = 0; i < S * 8; i++)
			if(array[i])
			{
				std::size_t result = 1;
				for(std::size_t j = 0; j < i + 1; j++)
					result *= 2;
				result--;
				bits[i / 8] += result;
			}
	}
	template <std::size_t S>
	void multiBool<S>::manipByte(const std::size_t index, const uint8_t byte) noexcept
	{
		if(index > S - 1)
			return;
		bits[index] = byte;
	}
}