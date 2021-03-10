#include "../../include/BSCtypes.hpp"

namespace bsc
{
	template <std::size_t S>
	bool &multiBool<S>::operator[](const std::size_t index) noexcept
	{
		for(std::size_t i = 8;; i *= 2)
			if(index * 8 < i * 8 - 8)
			{
				float temp = index / 8.0f;
				if(temp == i / 8)
					return bits[i / 8 - 1] % 2;
				if(temp == i / 8 + 1)
					return 0b00000010 & bits[i / 8 - 1];
				if(temp == i / 8 + 2)
					return 0b00000100 & bits[i / 8 - 1];
				if(temp == i / 8 + 3)
					return 0b00001000 & bits[i / 8 - 1];
				if(temp == i / 8 + 4)
					return 0b00010000 & bits[i / 8 - 1];
				if(temp == i / 8 + 5)
					return 0b00100000 & bits[i / 8 - 1];
				if(temp == i / 8 + 6)
					return 0b01000000 & bits[i / 8 - 1];
				return bits[i / 8 - 1] > 127;
			}
	}
	template <std::size_t S>
	const bool multiBool<S>::operator[](const std::size_t index) const noexcept
	{
		for(std::size_t i = 8;; i *= 2)
			if(index * 8 < i * 8 - 8)
			{
				float temp = index / 8.0f;
				if(temp == i / 8)
					return bits[i / 8 - 1] % 2;
				if(temp == i / 8 + 1)
					return 0b00000010 & bits[i / 8 - 1];
				if(temp == i / 8 + 2)
					return 0b00000100 & bits[i / 8 - 1];
				if(temp == i / 8 + 3)
					return 0b00001000 & bits[i / 8 - 1];
				if(temp == i / 8 + 4)
					return 0b00010000 & bits[i / 8 - 1];
				if(temp == i / 8 + 5)
					return 0b00100000 & bits[i / 8 - 1];
				if(temp == i / 8 + 6)
					return 0b01000000 & bits[i / 8 - 1];
				return bits[i / 8 - 1] > 127;
			}
	}
	template <std::size_t S>
	uint8_t &multiBool<S>::operator()(const std::size_t index) noexcept
	{
		return bits[index];
	}
	template <std::size_t S>
	const uint8_t multiBool<S>::operator()(const std::size_t index) const noexcept
	{
		return bits[index];
	}
}
