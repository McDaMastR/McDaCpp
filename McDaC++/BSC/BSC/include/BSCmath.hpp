#pragma once
#include "BSCtypes.hpp"

namespace bsc::math
{
    template <class T>
    constexpr const inline T pi        = T(3.141592653589793238l ); // Long doubles are precise to 18 digits
    template <class T>
    constexpr const inline T e         = T(2.718281828459045235l );
    template <class T>
    constexpr const inline T phi       = T(1.618033988749894848l );
    template <class T>
    constexpr const inline T in_phi    = T(-0.618033988749894848l);
    template <class T>
    constexpr const inline T sqrt2     = T(1.414213562373095048l );
    template <class T>
    constexpr const inline T sqrt3     = T(1.732050807568877293l );
    template <class T>
    constexpr const inline T sqrt5     = T(2.236067977499789696l );
    template <class T>
    constexpr const inline T in_sqrt5  = T(0.447213595499957927l );

    template <class T>
    [[nodiscard]]                                                  constexpr T add(const T num1, const T num2) noexcept;
    template <class T, class S>
    [[deprecated("Unsafe to use multiple data types"), nodiscard]] constexpr T add(const T num1, const S num2) noexcept;
    template <class T>
    [[nodiscard]]                                                  constexpr T sub(const T num1, const T num2) noexcept;
    template <class T, class S>
    [[deprecated("Unsafe to use multiple data types"), nodiscard]] constexpr T sub(const T num1, const S num2) noexcept;
    template <class T>
    [[nodiscard]]                                                  constexpr T mul(const T num1, const T num2) noexcept;
    template <class T, class S>
    [[deprecated("Unsafe to use multiple data types"), nodiscard]] constexpr T mul(const T num1, const S num2) noexcept;
    template <class T>
    [[nodiscard]]                                                  constexpr T div(const T num1, const T num2) noexcept;
    template <class T, class S>
    [[deprecated("Unsafe to use multiple data types"), nodiscard]] constexpr T div(const T num1, const S num2) noexcept;
    template <class T>
    [[nodiscard]]                                                  constexpr T mod(const T num1, const T num2) noexcept;
    template <class T, class S>
    [[deprecated("Unsafe to use multiple data types"), nodiscard]] constexpr T mod(const T num1, const S num2) noexcept;

    [[nodiscard]] constexpr std::size_t sqr(const std::size_t base                         ) noexcept;
    [[nodiscard]] constexpr float       sqr(const float base                               ) noexcept;
    [[nodiscard]] constexpr double      sqr(const double base                              ) noexcept;
    [[nodiscard]] constexpr long double sqr(const long double base                         ) noexcept;
    [[nodiscard]] constexpr std::size_t cbe(const std::size_t base                         ) noexcept;
    [[nodiscard]] constexpr float       cbe(const float base                               ) noexcept;
    [[nodiscard]] constexpr double      cbe(const double base                              ) noexcept;
    [[nodiscard]] constexpr long double cbe(const long double base                         ) noexcept;
    [[nodiscard]] constexpr std::size_t pow(const std::size_t base, const std::size_t index) noexcept;
    [[nodiscard]] constexpr float       pow(const float base, const std::size_t index      ) noexcept;
    [[nodiscard]] constexpr double      pow(const double base, const std::size_t index     ) noexcept;
    [[nodiscard]] constexpr long double pow(const long double base, const std::size_t index) noexcept;
    [[nodiscard]] constexpr float       pow(const std::size_t base, const float index      ) noexcept;
    [[nodiscard]] constexpr double      pow(const std::size_t base, const double index     ) noexcept;
    [[nodiscard]] constexpr long double pow(const std::size_t base, const long double index) noexcept;
    [[nodiscard]] constexpr float       pow(const float base, const float index            ) noexcept;
    [[nodiscard]] constexpr double      pow(const float base, const double index           ) noexcept;
    [[nodiscard]] constexpr long double pow(const float base, const long double index      ) noexcept;
    [[nodiscard]] constexpr double      pow(const double base, const float index           ) noexcept;
    [[nodiscard]] constexpr double      pow(const double base, const double index          ) noexcept;
    [[nodiscard]] constexpr long double pow(const double base, const long double index     ) noexcept;
    [[nodiscard]] constexpr long double pow(const long double base, const float index      ) noexcept;
    [[nodiscard]] constexpr long double pow(const long double base, const double index     ) noexcept;
    [[nodiscard]] constexpr long double pow(const long double base, const long double index) noexcept;

    [[nodiscard]] constexpr std::size_t inSqr(const std::size_t base                         ) noexcept;
    [[nodiscard]] constexpr float       inSqr(const float base                               ) noexcept;
    [[nodiscard]] constexpr double      inSqr(const double base                              ) noexcept;
    [[nodiscard]] constexpr long double inSqr(const long double base                         ) noexcept;
    [[nodiscard]] constexpr std::size_t inCbe(const std::size_t base                         ) noexcept;
    [[nodiscard]] constexpr float       inCbe(const float base                               ) noexcept;
    [[nodiscard]] constexpr double      inCbe(const double base                              ) noexcept;
    [[nodiscard]] constexpr long double inCbe(const long double base                         ) noexcept;
    [[nodiscard]] constexpr std::size_t inPow(const std::size_t base, const std::size_t index) noexcept;
    [[nodiscard]] constexpr float       inPow(const float base, const std::size_t index      ) noexcept;
    [[nodiscard]] constexpr double      inPow(const double base, const std::size_t index     ) noexcept;
    [[nodiscard]] constexpr long double inPow(const long double base, const std::size_t index) noexcept;
    [[nodiscard]] constexpr float       inPow(const std::size_t base, const float index      ) noexcept;
    [[nodiscard]] constexpr double      inPow(const std::size_t base, const double index     ) noexcept;
    [[nodiscard]] constexpr long double inPow(const std::size_t base, const long double index) noexcept;
    [[nodiscard]] constexpr float       inPow(const float base, const float index            ) noexcept;
    [[nodiscard]] constexpr double      inPow(const float base, const double index           ) noexcept;
    [[nodiscard]] constexpr long double inPow(const float base, const long double index      ) noexcept;
    [[nodiscard]] constexpr double      inPow(const double base, const float index           ) noexcept;
    [[nodiscard]] constexpr double      inPow(const double base, const double index          ) noexcept;
    [[nodiscard]] constexpr long double inPow(const double base, const long double index     ) noexcept;
    [[nodiscard]] constexpr long double inPow(const long double base, const float index      ) noexcept;
    [[nodiscard]] constexpr long double inPow(const long double base, const double index     ) noexcept;
    [[nodiscard]] constexpr long double inPow(const long double base, const long double index) noexcept;

    [[nodiscard]] double      sqrt(std::size_t base                              ) noexcept;
    [[nodiscard]] float       sqrt(float base                                    ) noexcept;
    [[nodiscard]] double      sqrt(double base                                   ) noexcept;
    [[nodiscard]] long double sqrt(long double base                              ) noexcept;
    [[nodiscard]] double      cbrt(std::size_t base                              ) noexcept;
    [[nodiscard]] float       cbrt(float base                                    ) noexcept;
    [[nodiscard]] double      cbrt(double base                                   ) noexcept;
    [[nodiscard]] long double cbrt(long double base                              ) noexcept;
    [[nodiscard]] std::size_t root(const std::size_t base, const std::size_t root) noexcept;
    [[nodiscard]] float       root(const float base, const std::size_t root      ) noexcept;
    [[nodiscard]] double      root(const double base, const std::size_t root     ) noexcept;
    [[nodiscard]] long double root(const long double base, const std::size_t root) noexcept;
    [[nodiscard]] float       root(const std::size_t base, const float root      ) noexcept;
    [[nodiscard]] double      root(const std::size_t base, const double root     ) noexcept;
    [[nodiscard]] long double root(const std::size_t base, const long double root) noexcept;
    [[nodiscard]] float       root(const float base, const float root            ) noexcept;
    [[nodiscard]] double      root(const float base, const double root           ) noexcept;
    [[nodiscard]] long double root(const float base, const long double root      ) noexcept;
    [[nodiscard]] double      root(const double base, const float root           ) noexcept;
    [[nodiscard]] double      root(const double base, const double root          ) noexcept;
    [[nodiscard]] long double root(const double base, const long double root     ) noexcept;
    [[nodiscard]] long double root(const long double base, const float root      ) noexcept;
    [[nodiscard]] long double root(const long double base, const double root     ) noexcept;
    [[nodiscard]] long double root(const long double base, const long double root) noexcept;

    [[nodiscard]] double      inSqrt(std::size_t base                               ) noexcept;
    [[nodiscard]] float       inSqrt(float base                                     ) noexcept;
    [[nodiscard]] double      inSqrt(double base                                    ) noexcept;
    [[nodiscard]] long double inSqrt(long double base                               ) noexcept;
    [[nodiscard]] double      inCbrt(std::size_t base                               ) noexcept;
    [[nodiscard]] float       inCbrt(float base                                     ) noexcept;
    [[nodiscard]] double      inCbrt(double base                                    ) noexcept;
    [[nodiscard]] long double inCbrt(long double base                               ) noexcept;
    [[nodiscard]] std::size_t inRoot(const std::size_t base, const std::size_t root ) noexcept;
    [[nodiscard]] float       inRoot(const float base, const std::size_t root      ) noexcept;
    [[nodiscard]] double      inRoot(const double base, const std::size_t root     ) noexcept;
    [[nodiscard]] long double inRoot(const long double base, const std::size_t root) noexcept;
    [[nodiscard]] float       inRoot(const std::size_t base, const float root      ) noexcept;
    [[nodiscard]] double      inRoot(const std::size_t base, const double root     ) noexcept;
    [[nodiscard]] long double inRoot(const std::size_t base, const long double root) noexcept;
    [[nodiscard]] float       inRoot(const float base, const float root            ) noexcept;
    [[nodiscard]] double      inRoot(const float base, const double root           ) noexcept;
    [[nodiscard]] long double inRoot(const float base, const long double root      ) noexcept;
    [[nodiscard]] double      inRoot(const double base, const float root           ) noexcept;
    [[nodiscard]] double      inRoot(const double base, const double root          ) noexcept;
    [[nodiscard]] long double inRoot(const double base, const long double root     ) noexcept;
    [[nodiscard]] long double inRoot(const long double base, const float root      ) noexcept;
    [[nodiscard]] long double inRoot(const long double base, const double root     ) noexcept;
    [[nodiscard]] long double inRoot(const long double base, const long double root) noexcept;

    [[nodiscard]] bool perfSqr(const std::size_t num) noexcept;

    [[deprecated("Has no effect on input")]] constexpr std::size_t ceil (const std::size_t num) noexcept;
    [[nodiscard]]                            constexpr float       ceil (const float num      ) noexcept;
    [[nodiscard]]                            constexpr double      ceil (const double num     ) noexcept;
    [[nodiscard]]                            constexpr long double ceil (const long double num) noexcept;
    [[deprecated("Has no effect on input")]] constexpr std::size_t floor(const std::size_t num) noexcept;
    [[nodiscard]]                            constexpr float       floor(const float num      ) noexcept;
    [[nodiscard]]                            constexpr double      floor(const double num     ) noexcept;
    [[nodiscard]]                            constexpr long double floor(const long double num) noexcept;

    [[nodiscard]] constexpr std::size_t primeNum(const std::size_t num) noexcept;
    [[nodiscard]] constexpr bool        isPrime (const std::size_t num) noexcept;
    [[nodiscard]] constexpr std::size_t fibNum  (const std::size_t num) noexcept;
    [[nodiscard]] constexpr bool        isFib   (const std::size_t num) noexcept;

    [[nodiscard]] constexpr double      maxAreaGivenPre (const std::size_t cir  ) noexcept;
    [[nodiscard]] constexpr float       maxAreaGivenPre (const float cir        ) noexcept;
    [[nodiscard]] constexpr double      maxAreaGivenPre (const double cir       ) noexcept;
    [[nodiscard]] constexpr long double maxAreaGivenPre (const long double cir  ) noexcept;
    [[nodiscard]] constexpr double      maxAreaGivenPreS(const std::size_t pre  ) noexcept;
    [[nodiscard]] constexpr float       maxAreaGivenPreS(const float pre        ) noexcept;
    [[nodiscard]] constexpr double      maxAreaGivenPreS(const double pre       ) noexcept;
    [[nodiscard]] constexpr long double maxAreaGivenPreS(const long double pre  ) noexcept;
    [[nodiscard]]           double      maxPreGivenArea (const std::size_t area) noexcept;
    [[nodiscard]]           float       maxPreGivenArea (const float area      ) noexcept;
    [[nodiscard]]           double      maxPreGivenArea (const double area     ) noexcept;
    [[nodiscard]]           long double maxPreGivenArea (const long double area) noexcept;
}