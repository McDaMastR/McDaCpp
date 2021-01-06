#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
#include <string>
#include "GameUtils.h"

int main()
{
    srand(time(NULL));

    while(true)
    {
        int input = menu();
        switch(input)
        {
            case 1:
                guessing_game();
                break;

            case 2:
                other_game();
                break;

            default:
                return 0;
        }
    }
}