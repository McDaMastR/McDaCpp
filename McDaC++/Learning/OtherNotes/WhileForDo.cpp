#include <iostream>

int main()
{
    int i = 0;
    int j = 0;
    char k[] = {'a', 'b', 'c', 'd', 'e', 'f'};

    while(i < 10) // Loop runs while i < 10
    {
        std::cout << i << std::endl;
        i++; // Adds 1 to i
    }

    std::cout << std::endl;

    for(int i = 0; i < 10; i++) // Creates variable i; Runs loop while i < 10; Adds 1 to i every loop
    {
        std::cout << i << std::endl;
    }

    std::cout << std::endl;

    do // A do-while loop runs the code at least once, then checks if the condition is true
    {
        std::cout << j << std::endl;
        j++;
    } while(j < 10);

    for(char n : k) // Range-based for loop: creates variable (n) to represent each value of array/vector for each value in k
    {
        std::cout << n << std::endl; // c++11 extention needed for the Range-based for loop
    }

    return 0;
}