#include <iostream>

int main()
{
    int number;
    int output = 1;
    std::cout << "What number should be factorialed?: ";
    std::cin >> number;

    for(int i = number; i > 1; i--)
    {
        output *= i;
    }

    std::cout << number << "! = " << output << std::endl;

    return 0;
}