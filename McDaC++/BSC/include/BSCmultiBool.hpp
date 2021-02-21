#pragma once

#include "BSCtypes.hpp"

namespace bsc
{
	// For amount of bools needed to be stored
	template <uint16_t S>
	class MultiBool
	{
	public:
		// Constructors
		[[nodiscard]] MultiBool() noexcept;
		[[nodiscard]] explicit MultiBool(bool *array[S]) noexcept;

		// Destructors
		~MultiBool() noexcept;

		// Getters
		[[nodiscard]] inline constexpr bool getFirst() noexcept {return bits % 2;}
		[[nodiscard]] inline constexpr const bool getFirst() const noexcept {return bits % 2;}

		[[nodiscard]] inline constexpr bool getSecond() noexcept {return 0b00000010 & bits == 2;}
		[[nodiscard]] inline constexpr const bool getSecond() const noexcept {return 0b00000010 & bits == 2;}
		
		[[nodiscard]] inline constexpr bool getThird() noexcept {return 0b00000100 & bits == 4;}
		[[nodiscard]] inline constexpr const bool getThird() const noexcept {return 0b00000100 & bits == 4;}

		[[nodiscard]] inline constexpr bool getFourth() noexcept {return 0b00001000 & bits == 8;}
		[[nodiscard]] inline constexpr const bool getFourth() const noexcept {return 0b00001000 & bits == 8;}

		[[nodiscard]] inline constexpr bool getFifth() noexcept {return 0b00010000 & bits == 16;}
		[[nodiscard]] inline constexpr const bool getFifth() const noexcept {return 0b00010000 & bits == 16;}

		[[nodiscard]] inline constexpr bool getSixth() noexcept {return 0b00100000 & bits == 32;}
		[[nodiscard]] inline constexpr const bool getSixth() const noexcept {return 0b00100000 & bits == 32;}

		[[nodiscard]] inline constexpr bool getSeventh() noexcept {return 0b01000000 & bits == 64;}
		[[nodiscard]] inline constexpr const bool getSeventh() const noexcept {return 0b01000000 & bits == 64;}

		[[nodiscard]] inline constexpr bool getEighth() noexcept {return 0b10000000 <= bits;}
		[[nodiscard]] inline constexpr const bool getEighth() const noexcept {return 0b10000000 <= bits;}

		//Setters
		inline constexpr void setFirst(const bool value) noexcept;
		inline constexpr void setFirst(const bool &&value) noexcept;

		inline constexpr void setSecond(const bool value) noexcept;
		inline constexpr void setSecond(const bool &&value) noexcept;

		inline constexpr void setThird(const bool value) noexcept;
		inline constexpr void setThird(const bool &&value) noexcept;

		inline constexpr void setFourth(const bool value) noexcept;
		inline constexpr void setFourth(const bool &&value) noexcept;

		inline constexpr void setFifth(const bool value) noexcept;
		inline constexpr void setFifth(const bool &&value) noexcept;

		inline constexpr void setSixth(const bool value) noexcept;
		inline constexpr void setSixth(const bool &&value) noexcept;

		inline constexpr void setSeventh(const bool value) noexcept;
		inline constexpr void setSeventh(const bool &&value) noexcept;

		inline constexpr void setEighth(const bool value) noexcept;
		inline constexpr void setEighth(const bool &&value) noexcept;

	private:
		uint8_t bits;
	};
}
