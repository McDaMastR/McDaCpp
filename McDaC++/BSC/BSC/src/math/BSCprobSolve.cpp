#include "../../include/BSCmath.hpp"

namespace bsc::math
{
    [[nodiscard]] constexpr std::size_t primeNum(const std::size_t num) noexcept
    {
        if(!num)
            return 0;
        if(num == 1)
            return 2;
        std::size_t counter = 1;
        std::size_t number  = 1;
        while (counter < num)
        {
            number += 2;
            if(isPrime(number))
                counter++;
        }
        return number;
    }
    [[nodiscard]] constexpr bool isPrime(const std::size_t num) noexcept
    {
        if(num < 2) 
            return false;
        if(num < 4) 
            return true;
        if(num % 2 == 0 || num % 3 == 0) 
            return false;
        for(std::size_t i = 5; i * i < num + 1; i += 6)
            if(!(num % i && num % (i + 2))) 
                return false;
        return true;
    }
    [[nodiscard]] constexpr std::size_t fibNum(const std::size_t num) noexcept
    {
        return static_cast<std::size_t>(in_sqrt5<long double> * static_cast<long double>(pow(phi<double>, num) - pow(in_phi<double>, num)));
    }
    [[nodiscard]] bool isFib(const std::size_t num) noexcept
    {
        return perfSqr(5 * num * num + 4) || perfSqr(5 * num * num - 4);
    }

    [[nodiscard]] constexpr double maxAreaGivenPre(const std::size_t cir) noexcept // Max area with circle (most area)
    {
        return cir * cir / (4 * pi<double>);
    }
    [[nodiscard]] constexpr float maxAreaGivenPre(const float cir) noexcept
    {
        return cir * cir / (4 * pi<float>);
    }
    [[nodiscard]] constexpr double maxAreaGivenPre(const double cir) noexcept
    {
        return cir * cir / (4 * pi<double>);
    }
    [[nodiscard]] constexpr long double maxAreaGivenPre(const long double cir) noexcept
    {
        return cir * cir / (4 * pi<long double>);
    }
    [[nodiscard]] constexpr double maxAreaGivenPreS(const std::size_t pre) noexcept // Max area with square
    {
        return pre * pre / 16.0;
    }
    [[nodiscard]] constexpr float maxAreaGivenPreS(const float pre) noexcept
    {
        return pre * pre / 16;
    }
    [[nodiscard]] constexpr double maxAreaGivenPreS(const double pre) noexcept
    {
        return pre * pre / 16;
    }
    [[nodiscard]] constexpr long double maxAreaGivenPreS(const long double pre) noexcept
    {
        return pre * pre / 16;
    }
    [[nodiscard]] double maxPreGivenArea(const std::size_t area) noexcept
    {
        return 4 * sqrt(area);
    }
    [[nodiscard]] float maxPreGivenArea(const float area) noexcept
    {
        return 4 * sqrt(area);
    }
    [[nodiscard]] double maxPreGivenArea(const double area) noexcept
    {
        return 4 * sqrt(area);
    }
    [[nodiscard]] long double maxPreGivenArea(const long double area) noexcept
    {
        return 4 * sqrt(area);
    }

	[[nodiscard]] returnArray<std::size_t, 3> genPythagTriple(const std::size_t u, const std::size_t v) noexcept
	{
		std::size_t temp[3];
		temp[0] = u * u - v * v;
		temp[1] = 2 * u * v;
		temp[2] = u * u + v * v;
		return temp;
	}
}