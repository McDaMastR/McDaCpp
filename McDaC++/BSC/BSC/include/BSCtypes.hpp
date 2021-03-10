#pragma once
#include "BSCpch.hpp"

namespace bsc
{
	template <class T, std::size_t S>
	struct [[nodiscard]] returnArray
	{
		returnArray(const T array[S]) noexcept;
		const T array[S];
	};

	template <std::size_t S>
	class multiBool
	{
	public:
		// Constructors
		[[nodiscard]] 		   multiBool(						) noexcept = default;
		[[nodiscard]] explicit multiBool(const bool array[8]	) noexcept			;
		[[nodiscard]] 		   multiBool(const multiBool &other	) noexcept			;
		[[nodiscard]] 		   multiBool(const multiBool &&other) noexcept			;

		// Destructor
		~multiBool() noexcept = default;

		// Getters
			  &returnArray<uint8_t, S> pureBits() 	   	noexcept;
		const  returnArray<uint8_t, S> pureBits() const noexcept;
			  &uint8_t 				   pureByte() 	   	noexcept;
		const  uint8_t 				   pureByte() const noexcept;

		// Setters
		void manipBits(const uint8_t bits[S]					  ) noexcept;
		void manipByte(const std::size_t index, const uint8_t byte) noexcept;

		// Operator overloads
			  bool 	  &operator[](const std::size_t index) 	   	 noexcept;
		const bool 	   operator[](const std::size_t index) const noexcept;
			  uint8_t &operator()(const std::size_t index) 		 noexcept;
		const uint8_t  operator()(const std::size_t index) const noexcept;

	private:
		uint8_t bits[S];
	};
}