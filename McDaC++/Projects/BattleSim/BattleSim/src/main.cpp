#include "../include/battle.hpp"

//
//  Author: Seth McDonald
//  Product: C++ Battle Simulator
//

/*  
    TODO 
    - Nothing

    HOW TO COMPILE AND RUN IN TERMINAL
    For debugging:
    - time g++ -std=c++2a -Og -D DEBUG -Wall -Wextra -Wshadow -Wdouble-promotion -o BattleSimDebug main.cpp start.cpp battle.cpp fighter.cpp
    - ./BattleSimDebug

    For distribution:
    - g++ -std=c++2a -O3 -o BattleSim main.cpp start.cpp battle.cpp fighter.cpp
    - ./BattleSim
*/

#ifdef DEBUG
constexpr const std::string_view admin_pwd = "best_pwd";
#endif

int main()
{
    uint16_t input;
    uint16_t no_opp;

#ifdef DEBUG
    {
        std::string admin_input;
        std::cout << "In debug mode\nEnter admin pwd to continue: ";
        std::cin >> admin_input;
        if(admin_input == admin_pwd)
        {
            Details::get().LV = 2;
            battleInit(1);
        }
        else
        {
            std::cout << "Incorrect. Terminating program\n";
            std::cin.get();
            return 0;
        }
    }
#endif
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
                if((no_opp = cfmBattle()))
                    battleInit(no_opp);
                break;
            
            case 2:
                tutorial();
                break;

            case 3:
                credits();
				std::cin.get();
                break;
            
            case 4:
                signIn();
                break;

            case 5:
				signOut();
                break;
            
            case 6:
                return 0;
        }
    }
}
