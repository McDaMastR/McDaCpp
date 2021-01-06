#include <iostream>
#include <cmath>

double power(double base, int exponent)
{
    double result = 1;
    for(int i = 0; i < exponent; i++) // Learn about for and while loop syntax
    {
        result = result * base;
    }
    return result;
}

void print_power(double base, int exponent)
{
    double result = power(base, exponent);
    std::cout << base << "^" << exponent << "=" << result << std::endl;
}

int main()
{
    double base;
    int exponent;
    std::cout << "Base: ";
    std::cin >> base;
    std::cout << "Exponent: ";
    std::cin >> exponent;
    print_power(base, exponent);

    return 0;
}