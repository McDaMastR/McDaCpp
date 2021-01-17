#include <iostream>
#include <ctime>
#include <array>
#include "start.hpp"
#include "battle.hpp"

#define CLEAR system("clear")

int main()
{
    srand(time(0));

    uint8_t input;
    bool cfm;
    std::array<uint16_t, 3> custom_values;
    std::array<std::string, 2> unames;
    Details details;

    std::cout << "Welcome to C++ battle simulator!\nEnter to start";
    std::cin.get();

    while(true)
    {
        CLEAR;
        input = start();
        CLEAR;

        switch(input)
        {
            case 1:
                cfm = cfm_battle(details.LV);
                if(cfm) {battle_init(details.uname, 1, details);}
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
            
            default:
                std::cout << "This doesn't work... :(" << std::endl;
                std::cin.get();
                break;
        }
    }
}
