#include <iostream>
#include <string>

class Position
{
    public:
        int x = 10;
        int y = 20;

        Position operator + (Position pos) /* Overloading the + operator to the needs of the Position class
        The 1st Position refers to the type that is being returned
        The operator refers to the operator it is overloading (pos1 (+) pos2)
        The (Position pos) 1st refers to the type of the 1st and 2nd variable being added (POS1 + POS2),
        then refers to that 2nd vraiable being added as "pos" */
        {
            Position new_pos;
            new_pos.x = x + pos.x; // new_pos.x = return value; x = 1st value being added; pos.x = 2nd value being added
            new_pos.y = y + pos.y; // Same as line above but for y not x
            return new_pos; // returns the value for the end result of the calculation
        }

        bool operator == (Position pos) // Same thing as above but overloading == not +
        {
            if(x == pos.x && y == pos.y)
            {
                return true;
            }
            return false;
        }
};

std::ostream& operator << (std::ostream& output, const Position pos) /* The overloading of operators with two different data
types requires to be outside of a class */
{
    output << pos.x << ", " << pos.y;
    return output;
}

std::istream& operator >> (std::istream& input, Position &pos) // This overloading with class Position can only access public variables
{
    input >> pos.x >> pos.y;
    return input;
}

int main()
{
    Position pos1, pos2, pos4;
    Position pos3 = pos1 + pos2;
    std::cout << pos3.x << "\t" << pos3.y << std::endl;

    if(pos1 == pos2)
    {
        std::cout << "This works!!!" << std::endl;
    }

    std::cout << pos3 << std::endl;
    std::cout << "Enter the x & y: ";
    std::cin >> pos4;
    std::cout << pos4;

    return 0;
}