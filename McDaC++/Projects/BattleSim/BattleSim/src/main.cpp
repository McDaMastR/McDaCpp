#include "../include/battle.hpp"

int main()
{
    uint8_t input;
    uint16_t no_opp;

#ifdef DEBUG
	printf("In debug mode\n");
#endif
    std::cout << "Welcome to C++ battle simulator!\nEnter to start";
    std::cin.get();
	
    while(true)
    {
        CLEAR;
        start(input);
        CLEAR;

        switch(input)
        {
            case 0x31: // 1
                cfmBattle(no_opp);
                if(no_opp) battleInit(no_opp);
                break;
            
            case 0x32: // 2
                tutorial();
                break;

            case 0x33: // 3
                credits();
				std::cin.get();
                break;
            
            case 0x34: // 4
                signIn();
                break;

            case 0x35: // 5
				signOut();
                break;
            
            case 0x36: // 6
                return 0;

			default:
				continue;
        }
    }
}
