#include  <iostream>

// We usually a base-10 number system going from 0 - 9
// e.g. 1, 2, 3, 4, 5, 6, 7, 8, 9, 10

// Hexidecimal is a base-16 number system going from 0 - F
// e.g. 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F, 10

// Octal is a base-8 3-didget number system going from 000 - 007
// e.g. 000, 001, 002, 003, 004, 005, 006, 007, 010

// Binary is a base-2 8-didget number system going from 00000000 - 00000001
// e.g. 00000000, 00000001, 00000010

int main()
{
    int dec_number = 30; // Creates a number in base-10
    std::cout << dec_number << std::endl;

    int hex_number = 0x30; // Creates a number in hexidecimal (base-16)
    std::cout << hex_number << std::endl;
    std::cout << std::hex << hex_number << std::endl; // Prints hex_number in hexidecimal instead of base-10
    
    int oct_number = 030; // Creates a number in octal (base-8)
    std::cout << oct_number << std::endl;
    std::cout << std::oct << oct_number << std::endl;

    return 0;
}