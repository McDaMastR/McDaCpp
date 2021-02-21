#pragma once
#include "BSCpch.hpp"

namespace bsc
{
    struct int128_t
    {
        // Values
        int64_t v2;
        uint64_t v1;

        // Constructors/Destructor
        int128_t() noexcept;
        explicit int128_t(const int128_t &other) noexcept;
        int128_t(const int64_t &first, const int64_t &second) noexcept;
        int128_t(const int128_t &&other) noexcept;
        int128_t(const int64_t &&first, const int64_t &&second) noexcept;
        ~int128_t() noexcept;

        // Logical operators
        constexpr       int128_t operator+(const int128_t other);
        constexpr const int128_t operator+(const int128_t other) const;
        constexpr       int128_t operator-(const int128_t other);
        constexpr const int128_t operator-(const int128_t other) const;
        constexpr       int128_t operator*(const int128_t other);
        constexpr const int128_t operator*(const int128_t other) const;
        constexpr       int128_t operator/(const int128_t other);
        constexpr const int128_t operator/(const int128_t other) const;
        constexpr int128_t operator+=(const int128_t other);
        constexpr int128_t operator-=(const int128_t other);
        constexpr int128_t operator*=(const int128_t other);
        constexpr int128_t operator/=(const int128_t other);

        // Comparison operators
        constexpr bool operator==(const int128_t other) const;
        constexpr bool operator!=(const int128_t other) const;
        constexpr bool operator>(const int128_t other)  const;
        constexpr bool operator>=(const int128_t other) const;
        constexpr bool operator<(const int128_t other)  const;
        constexpr bool operator<=(const int128_t other) const;
        constexpr bool operator!()                      const;
    };

    struct uint128_t
    {
        unsigned long v1;
        unsigned long v2;
    };
}