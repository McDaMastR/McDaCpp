#include <iostream>

int main()
{
    int x = 5;
    char y = 'h';
    double z = 6.78;

    enum w{yes, no, maybe, neva};
    w n = no;

    if(x == 5 && z == 6.78) // the && (and) logical operator asks if both expressions are true
    {
        std::cout << "x = 5 and z = 6.78\n";
    }
    else if(x == 5 || z == 6.78) // the || (or) logical operator asks if one of the expressions are true
    {
        std::cout << "x = 5 or z = 6.78\n";
        return -1; // Using a return statement inside an if statemment terminates the program immidiatly
    }
    else if(!(x == 5)) // the !() (not) logical operator inverts the expression from true to false or false to true
    {
        std::cout << "x ≠ 5\n";
    }
    else if (z != 6.78) // the != comparison operator asks if z is not equal to 6.78
    {
        std::cout << "z ≠ 6.78\n";
    }
    else
    {
        std::cout << "How is this even being printed!?\n";
    }

    switch(y) // Has to be type integral (e.g. int, char, long)
    {
        case 'y': // Equilivant to "if (y == 'y')"
            std::cout << "y = y\n";
            break; // Breaks out of the switch statement
        
        case 'h': // Case value has to be exact value, cannot be ranges
            std::cout << "y = h\n";
            break;
        
        case 'f':
        case 'r': // Putting two cases together combines their outputs
            std::cout << "y = f or r\n";
            break;
        
        default: // Equilivant to an "else" statement
            std::cout << "y ≠ y or h\n";
            break;
    }
    return 0;
}