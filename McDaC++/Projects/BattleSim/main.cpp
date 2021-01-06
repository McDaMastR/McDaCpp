#include <iostream>
#include <ctime>
#include <string>
#include <array>
#include "start.hpp"
#include "battle.hpp"
#include "basic.hpp"

int main()
{
    srand(time(NULL));

    short input;
    std::array<int, 3> custom_values;
    std::array<std::string, 2> unames;
    Details details;

    std::cout << "Welcome to C++ battle simulator!\nEnter to start";
    std::cin.get();

    while(true)
    {
        bsc::clear();
        input = start();
        bsc::clear();

        switch(input)
        {
            case 1:
                cfm_battle(details.LV);
                battle_init(details.uname, 1, details.LV, details.EXP);
                break;
            
            case 2:
                tutorial();
                break;
            
            case 3:
                custom_values = custom(details.LV, details.guest);
                custom_battle(custom_values);
                break;
            
            case 4:
                unames = multiplayer();
                multi_battle(unames);
                break;
            
            case 5:
                credits();
                break;
            
            case 6:
                details = sign_in();
                break;

            case 7:
                details.LV = 0;
                break;
            
            case 8:
                return 0;
        }
    }
}