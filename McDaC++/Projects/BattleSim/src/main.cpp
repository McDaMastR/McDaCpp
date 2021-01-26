#include "../include/battle.hpp"

#define CLEAR system("clear")

int main() // TODO replace unions with std::variant
           // TODO define preprocesser variable for debugging
{
    uint8_t input;
    std::optional<uint8_t> no_opp;
    std::optional<std::array<uint16_t, 3>> custom_values;

    std::cout << "Welcome to C++ battle simulator!\nEnter to start";
    std::cin.get();

    while(true)
    {
        #ifdef DEBUG
        printf("Input = %i\nCalling start() function\n", input);
        start(input);
        printf("Start() function successfully ended\nInput = %i\nEntering switch statement\n", input);
        #else
        CLEAR;
        start(input);
        CLEAR;
        #endif

        switch(input)
        {
            case 1:
                no_opp = cfm_battle();
                if(no_opp) battle_init(no_opp);
                break;
            
            case 2:
                tutorial();
                break;

            case 3:
                credits();
                break;
            
            case 4:
                sign_in();
                break;

            case 5:
                Details::get().LV = 0;
                break;
            
            case 6:
                return 0;

            #ifdef DEBUG

            case 7:
                printf("Start() function not changing main input value");
                return -1;

            default:
                printf("Input value is being changed by something else");
                return -1;
            
            #endif
        }
    }
}
