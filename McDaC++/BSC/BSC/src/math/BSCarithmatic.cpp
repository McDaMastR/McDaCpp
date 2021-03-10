#include "../../include/BSCmath.hpp"

namespace bsc::math
{
    template <class T>
    [[nodiscard]] constexpr inline T add(const T num1, const T num2) noexcept // Do paramater packaging
    {
        return num1 + num2;
    }
    template <class T, class S>
    [[deprecated("Unsafe to use multiple data types"), nodiscard]] constexpr inline T add(const T num1, const S num2) noexcept
    {
        return num1 + num2;
    }
    template <class T>
    [[nodiscard]] constexpr inline T sub(const T num1, const T num2) noexcept
    {
        return num1 - num2;
    }
    template <class T, class S>
    [[deprecated("Unsafe to use multiple data types"), nodiscard]] constexpr inline T sub(const T num1, const S num2) noexcept
    {
        return num1 - num2;
    }
    template <class T>
    [[nodiscard]] constexpr inline T mul(const T num1, const T num2) noexcept
    {
        return num1 * num2;
    }
    template <class T, class S>
    [[deprecated("Unsafe to use multiple data types"), nodiscard]] constexpr inline T mul(const T num1, const S num2) noexcept
    {
        return num1 * num2;
    }
    template <class T>
    [[nodiscard]] constexpr inline T div(const T num1, const T num2) noexcept
    {
        return num1 / num2;
    }
    template <class T, class S>
    [[deprecated("Unsafe to use multiple data types"), nodiscard]] constexpr inline T div(const T num1, const S num2) noexcept
    {
        return num1 / num2;
    }
    template <class T>
    [[nodiscard]] constexpr inline T mod(const T num1, const T num2) noexcept
    {
        return num1 % num2;
    }
    template <class T, class S>
    [[deprecated("Unsafe to use multiple data types"), nodiscard]] constexpr inline T mod(const T num1, const S num2) noexcept
    {
        return num1 % num2;
    }

	[[nodiscard]] constexpr double inverse(const std::size_t num) noexcept
	{
		return 1 / num;
	}
	[[nodiscard]] constexpr float inverse(const float num) noexcept
	{
		return 1 / num;
	}
	[[nodiscard]] constexpr double inverse(const double num) noexcept
	{
		return 1 / num;
	}
	[[nodiscard]] constexpr long double inverse(const long double num) noexcept
	{
		return 1 / num;
	}

	[[nodiscard]] constexpr int64_t abs(const int64_t num) noexcept
	{
		return num < 0 ? -num : num;
	}
	[[nodiscard]] constexpr float abs(const float num) noexcept
	{
		return num < 0 ? -num : num;
	}
	[[nodiscard]] constexpr double abs(const double num) noexcept
	{
		return num < 0 ? -num : num;
	}
	[[nodiscard]] constexpr long double abs(const long double num) noexcept
	{
		return num < 0 ? -num : num;
	}

    [[nodiscard]] constexpr std::size_t sqr(const std::size_t base) noexcept
    {
        return base * base;
    }
    [[nodiscard]] constexpr float sqr(const float base) noexcept
    {
        return base * base;
    }
    [[nodiscard]] constexpr double sqr(const double base) noexcept
    {
        return base * base;
    }
    [[nodiscard]] constexpr long double sqr(const long double base) noexcept
    {
        return base * base;
    }
    [[nodiscard]] constexpr std::size_t cbe(const std::size_t base) noexcept
    {
        return base * base * base;
    }
    [[nodiscard]] constexpr float cbe(const float base) noexcept
    {
        return base * base * base;
    }
    [[nodiscard]] constexpr double cbe(const double base) noexcept
    {
        return base * base * base;
    }
    [[nodiscard]] constexpr long double cbe(const long double base) noexcept
    {
        return base * base * base;
    }

    [[nodiscard]] constexpr std::size_t pow(const std::size_t base, const std::size_t index) noexcept
    {
        if(!index)
            return 1;
        std::size_t result = base;
        for(std::size_t i = 1; i < index; i++)
            result *= base;
        return result;
    }
    [[nodiscard]] constexpr float pow(const float base, const std::size_t index) noexcept
	{
		if(!index)
            return 1;
        float result = base;
        for(std::size_t i = 1; i < index; i++)
            result *= base;
        return result;
	}
    [[nodiscard]] constexpr double pow(const double base, const std::size_t index) noexcept
    {
        if(!index)
            return 1;
        double result = base;
        for(std::size_t i = 1; i < index; i++)
            result *= base;
        return result;
    }
    [[nodiscard]] constexpr long double pow(const long double base, const std::size_t index) noexcept
	{
		if(!index)
            return 1;
        long double result = base;
        for(std::size_t i = 1; i < index; i++)
            result *= base;
        return result;
	}
    [[nodiscard]] constexpr float pow(const std::size_t base, const float index) noexcept
	{
		return pow(base, static_cast<std::size_t>(index)) * root(base, 1 / (index - static_cast<std::size_t>(index)));
	}
    [[nodiscard]] constexpr double pow(const std::size_t base, const double index) noexcept
	{
		return pow(base, static_cast<std::size_t>(index)) * root(base, 1 / (index - static_cast<std::size_t>(index)));
	}
    [[nodiscard]] constexpr long double pow(const std::size_t base, const long double index) noexcept
	{
		return pow(base, static_cast<std::size_t>(index)) * root(base, 1 / (index - static_cast<std::size_t>(index)));
	}
    [[nodiscard]] constexpr float pow(const float base, const float index) noexcept
	{
		return pow(base, static_cast<std::size_t>(index)) * root(base, 1 / (index - static_cast<std::size_t>(index)));
	}
    [[nodiscard]] constexpr double pow(const float base, const double index) noexcept
	{
		return static_cast<double>(pow(base, static_cast<std::size_t>(index))) * root(base, 1 / (index - static_cast<std::size_t>(index)));
	}
    [[nodiscard]] constexpr long double pow(const float base, const long double index) noexcept
	{
		return static_cast<long double>(pow(base, static_cast<std::size_t>(index))) * root(base, 1 / (index - static_cast<std::size_t>(index)));
	}
    [[nodiscard]] constexpr double pow(const double base, const float index ) noexcept
	{
		return pow(base, static_cast<std::size_t>(index)) * root(base, 1 / (index - static_cast<std::size_t>(index)));
	}
    [[nodiscard]] constexpr double pow(const double base, const double index) noexcept
	{
		return pow(base, static_cast<std::size_t>(index)) * root(base, 1 / (index - static_cast<std::size_t>(index)));
	}
    [[nodiscard]] constexpr long double pow(const double base, const long double index) noexcept
	{
		return static_cast<long double>(pow(base, static_cast<std::size_t>(index))) * root(base, 1 / (index - static_cast<std::size_t>(index)));
	}
    [[nodiscard]] constexpr long double pow(const long double base, const float index) noexcept
	{
		return pow(base, static_cast<std::size_t>(index)) * root(base, 1 / (index - static_cast<std::size_t>(index)));
	}
    [[nodiscard]] constexpr long double pow(const long double base, const double index) noexcept
	{
		return pow(base, static_cast<std::size_t>(index)) * root(base, 1 / (index - static_cast<std::size_t>(index)));
	}
    [[nodiscard]] constexpr long double pow(const long double base, const long double index) noexcept
	{
		return pow(base, static_cast<std::size_t>(index)) * root(base, 1 / (index - static_cast<std::size_t>(index)));
	}

    [[nodiscard]] constexpr double inSqr(const std::size_t base) noexcept
	{
		return 1 / (base * base);
	}
    [[nodiscard]] constexpr float inSqr(const float base) noexcept
	{
		return 1 / (base * base);
	}
    [[nodiscard]] constexpr double inSqr(const double base) noexcept
	{
		return 1 / (base * base);
	}
    [[nodiscard]] constexpr long double inSqr(const long double base) noexcept
	{
		return 1 / (base * base);
	}
    [[nodiscard]] constexpr double inCbe(const std::size_t base) noexcept
	{
		return 1 / (base * base * base);
	}
    [[nodiscard]] constexpr float inCbe(const float base) noexcept
	{
		return 1 / (base * base * base);
	}
    [[nodiscard]] constexpr double inCbe(const double base) noexcept
	{
		return 1 / (base * base * base);
	}
    [[nodiscard]] constexpr long double inCbe(const long double base) noexcept
	{
		return 1 / (base * base * base);
	}	
    [[nodiscard]] constexpr double inPow(const std::size_t base, const std::size_t index) noexcept
	{
		return 1 / pow(base, index);
	}
    [[nodiscard]] constexpr float inPow(const float base, const std::size_t index) noexcept
	{
		return 1 / pow(base, index);
	}
    [[nodiscard]] constexpr double inPow(const double base, const std::size_t index) noexcept
	{
		return 1 / pow(base, index);
	}
    [[nodiscard]] constexpr long double inPow(const long double base, const std::size_t index) noexcept
	{
		return 1 / pow(base, index);
	}
    [[nodiscard]] constexpr float inPow(const std::size_t base, const float index) noexcept
	{
		return 1 / pow(base, index);
	}
    [[nodiscard]] constexpr double inPow(const std::size_t base, const double index) noexcept
	{
		return 1 / pow(base, index);
	}
    [[nodiscard]] constexpr long double inPow(const std::size_t base, const long double index) noexcept
	{
		return 1 / pow(base, index);
	}
    [[nodiscard]] constexpr float inPow(const float base, const float index) noexcept
	{
		return 1 / pow(base, index);
	}
    [[nodiscard]] constexpr double inPow(const float base, const double index) noexcept
	{
		return 1 / pow(base, index);
	}
    [[nodiscard]] constexpr long double inPow(const float base, const long double index) noexcept
	{
		return 1 / pow(base, index);
	}
    [[nodiscard]] constexpr double inPow(const double base, const float index) noexcept
	{
		return 1 / pow(base, index);
	}
    [[nodiscard]] constexpr double inPow(const double base, const double index) noexcept
	{
		return 1 / pow(base, index);
	}
    [[nodiscard]] constexpr long double inPow(const double base, const long double index) noexcept
	{
		return 1 / pow(base, index);
	}
    [[nodiscard]] constexpr long double inPow(const long double base, const float index) noexcept
	{
		return 1 / pow(base, index);
	}
    [[nodiscard]] constexpr long double inPow(const long double base, const double index) noexcept
	{
		return 1 / pow(base, index);
	}
    [[nodiscard]] constexpr long double inPow(const long double base, const long double index) noexcept
	{
		return 1 / pow(base, index);
	}

    [[nodiscard]] double sqrt(std::size_t base) noexcept
    {
		if(!base || base == 1)
			return base;
        if constexpr(sizeof(std::size_t) == 8)
        {
            int64_t i;
            double j = static_cast<double>(base);
            i = *reinterpret_cast<int64_t *>(&j);
            i = 0x5f3759df - (i >> 1);
            j = *reinterpret_cast<double *>(&i);
            return j * i * static_cast<double>(1.5f - 0.5f * i * i);
        }
        int32_t i;
        float j = static_cast<float>(base);
        i = *reinterpret_cast<int32_t *>(&j);
        i = 0x5f3759df - (i >> 1);
        j = *reinterpret_cast<float *>(&i);
        return static_cast<double>(j * i * (1.5f - 0.5f * i * i));
    }
    [[nodiscard]] float sqrt(float base) noexcept
    {
		if(base == 0 || base == 1)
			return base;
        int32_t i;
        i = *reinterpret_cast<int32_t *>(&base);
        i = 0x5f3759df - (i >> 1);
        base = *reinterpret_cast<float *>(&i);
        return base * i * (1.5f - 0.5f * i * i);
    }
    [[nodiscard]] double sqrt(double base) noexcept
    {
		if(base == 0 || base == 1)
			return base;
        int64_t i;
        i = *reinterpret_cast<int64_t *>(&base);
        i = 0x5f3759df - (i >> 1);
        base = *reinterpret_cast<double *>(&i);
        return base * i * static_cast<double>(1.5f - 0.5f * i * i);
    }
    [[nodiscard]] long double sqrt(long double base) noexcept
    {
		if(base == 0 || base == 1)
			return base;
        int64_t i;
        i = *reinterpret_cast<int64_t *>(&base);
        i = 0x5f3759df - (i >> 1);
        base = *reinterpret_cast<long double *>(&i);
        base *= static_cast<long double>(i * (1.5f - 0.5f * i * i));
        return base * i * static_cast<long double>(1.5f - 0.5f * i * i);
    }
    [[nodiscard]] constexpr double cbrt(std::size_t base) noexcept
	{
		if(!base || base == 1)
			return base;
		double start = 0, end = base, mid, error; 
		while (true) 
		{ 
			mid = (start + end)/2; 
			error = abs(base - mid * mid * mid); 
			if (error <= 0.0000001) 
				return mid; 
			if (mid * mid * mid > base) 
				end = mid; 
			else
				start = mid; 
    	}
	}
    [[nodiscard]] constexpr float       cbrt(float base                                    ) noexcept;
    [[nodiscard]] constexpr double      cbrt(double base                                   ) noexcept;
    [[nodiscard]] constexpr long double cbrt(long double base                              ) noexcept;
    [[nodiscard]] constexpr std::size_t root(const std::size_t base, const std::size_t root) noexcept;
    [[nodiscard]] constexpr float       root(const float base, const std::size_t root      ) noexcept;
    [[nodiscard]] constexpr double      root(const double base, const std::size_t root     ) noexcept;
    [[nodiscard]] constexpr long double root(const long double base, const std::size_t root) noexcept;
    [[nodiscard]] constexpr float       root(const std::size_t base, const float root      ) noexcept;
    [[nodiscard]] constexpr double      root(const std::size_t base, const double root     ) noexcept;
    [[nodiscard]] constexpr long double root(const std::size_t base, const long double root) noexcept;
    [[nodiscard]] constexpr float       root(const float base, const float root            ) noexcept;
    [[nodiscard]] constexpr double      root(const float base, const double root           ) noexcept;
    [[nodiscard]] constexpr long double root(const float base, const long double root      ) noexcept;
    [[nodiscard]] constexpr double      root(const double base, const float root           ) noexcept;
    [[nodiscard]] constexpr double      root(const double base, const double root          ) noexcept;
    [[nodiscard]] constexpr long double root(const double base, const long double root     ) noexcept;
    [[nodiscard]] constexpr long double root(const long double base, const float root      ) noexcept;
    [[nodiscard]] constexpr long double root(const long double base, const double root     ) noexcept;
    [[nodiscard]] constexpr long double root(const long double base, const long double root) noexcept;

    [[nodiscard]] double inSqrt(std::size_t base) noexcept
    {
		if(base == 1)
			return base;
        if constexpr(sizeof(std::size_t) == 8)
        {
            int64_t i;
            double j = static_cast<double>(base);
            i = *reinterpret_cast<int64_t *>(&j);
            i = 0x5f3759df - (i >> 1);
            j = *reinterpret_cast<double *>(&i);
            return j * static_cast<double>(1.5 - 0.5 * j * j * j);
        }
        int32_t i;
        float j = static_cast<float>(base);
        i = *reinterpret_cast<int32_t *>(&j);
        i = 0x5f3759df - (i >> 1);
        j = *reinterpret_cast<float *>(&i);
        return static_cast<double>(j * (1.5f - 0.5f * j * j * j));
    }
    [[nodiscard]] float inSqrt(float base) noexcept
    {
		if(base == 1)
			return base;
        int32_t i;
        i = *reinterpret_cast<int32_t *>(&base);
        i = 0x5f3759df - (i >> 1);
        base = *reinterpret_cast<float *>(&i);
        return base * (1.5f - 0.5f * base * base * base);
    }
    [[nodiscard]] double inSqrt(double base) noexcept
    {
		if(base == 1)
			return base;
        int64_t i;
        i = *reinterpret_cast<int64_t *>(&base);
        i = 0x5f3759df - (i >> 1);
        base = *reinterpret_cast<double *>(&i);
        return base * static_cast<double>(1.5 - 0.5 * base * base * base);
    }
    [[nodiscard]] long double inSqrt(long double base) noexcept
    {
		if(base == 1)
			return base;
        int64_t i;
        i = *reinterpret_cast<int64_t *>(&base);
        i = 0x5f3759df - (i >> 1);
        base = *reinterpret_cast<long double *>(&i);
        return static_cast<long double>(base * (1.5l - 0.5l * base * base * base));
    }

    [[nodiscard]] bool perfSqr(const std::size_t num) noexcept
    {
        return static_cast<std::size_t>(sqrt(num)) * static_cast<std::size_t>(sqrt(num)) == num;
    }

    [[deprecated("Has no effect on input")]] constexpr std::size_t ceil(const std::size_t num) noexcept
    {
        return num;
    }
    [[nodiscard]] constexpr float ceil(const float num) noexcept
    {
        return num == static_cast<int>(num) ? num : static_cast<int>(num + 1);
    }
    [[nodiscard]] constexpr double ceil(const double num) noexcept
    {
        return num == static_cast<int>(num) ? num : static_cast<int>(num + 1);
    }
    [[nodiscard]] constexpr long double ceil(const long double num) noexcept
    {
        return num == static_cast<long>(num) ? num : static_cast<long>(num + 1);
    }
    [[deprecated("Has no effect on input")]] constexpr std::size_t floor(const std::size_t num) noexcept
    {
        return num;
    }
    [[nodiscard]] constexpr float floor(const float num) noexcept
    {
        return num == static_cast<int>(num) ? num : static_cast<int>(num - 1);
    }
    [[nodiscard]] constexpr double floor(const double num) noexcept
    {
        return num == static_cast<int>(num) ? num : static_cast<int>(num - 1);
    }
    [[nodiscard]] constexpr long double floor(const long double num) noexcept
    {
        return num == static_cast<long>(num) ? num : static_cast<long>(num - 1);
    }
}
