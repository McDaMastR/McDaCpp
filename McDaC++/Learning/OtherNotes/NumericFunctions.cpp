#include <iostream>
#include <cmath> // Includes the numerix functions

int main()
{
    short w = -5;
    short x = 25;
    short y = 5;
    float z = 3.25;

    std::cout << pow(x, y) << std::endl; // Raises x to the power of y
    std::cout << sqrt(x) << std::endl; // Gives the square root of x
    std::cout << remainder(x, z) << std::endl; // Gives the remainer of x / y
    std::cout << fmax(x, y) << std::endl; // returns the argument with the highest value
    std::cout << fmin(x, y) << std::endl; // returns the argument with the lowest value 
    std::cout << round(z) << std::endl; // rounds z up or down correspondingly
    std::cout << ceil(z) << std::endl; // returns z rounded up
    std::cout << floor(z) << std::endl; // returns z rounded down
    std::cout << trunc(z) << std::endl; // crops off the decimals of z
    std::cout << abs(w) << std::endl; // returns the absolute value of w

    return 0;
}