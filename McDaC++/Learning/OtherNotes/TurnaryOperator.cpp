#include <iostream>

int main()
{
    short ans = 12;
    short guess;

    std::cout << "Guess the number: ";
    std::cin >> guess;

    short points = guess == ans ? 10 : 0; // Asks if expression (guess == ans) is true. If so, points = 10, if false, points = 0
    std::cout << points << std::endl; // This operator can also be used without being assigned to a variable

    return 0;
}