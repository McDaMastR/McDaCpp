#include "../include/start.hpp"

#include <filesystem>

Details &Details::get()
{
    static Details details;
    return details;
}

void Details::setPwd(const std::string &pwd) {get().privateSetPwd(pwd);}

void Details::setGuest() {get().privateSetGuest();}

void Details::privateSetPwd(const std::string &pwd) {this->pwd = pwd;}

void Details::privateSetGuest()
{
    guest = true;
    uint16_t number = incGuests();
    LV = 1;
    EXP = 0;
    uname = "Guest" + std::to_string(number);
}
 
uint8_t Details::incGuests()
{
    guest_num++; 
    return guest_num;
}

void Details::operator ++ (int)
{
    EXP++;

    if(EXP == LV)
    {
        LV++;
        EXP = 0;
    }
}

void start(uint8_t &input)
{
    const std::string msg = R"(Enter 1 to start
Enter 2 for the tutorial
Enter 3 for credits
Enter 4 to login
Enter 5 to log out
Enter 6 to quit: )";

    std::cout << msg;
    std::cin >> input;
}

void cfmBattle(uint16_t &no_opp)
{
    const uint16_t LV = Details::get().LV;

   if(!LV)
    {
        do
        {
			CLEAR;
            std::cout << "You have to be signed in to battle\n";
            std::cout << "Enter 1 to sign in\nEnter 2 to go to home screen: ";
            std::cin >> no_opp;
        }
        while(!(no_opp == 1 || no_opp == 2));

		CLEAR;
		
        if(no_opp == 1)
			signIn();
		
		no_opp = 0;
		return;
	}

    if(LV < 10) 	 no_opp = 1;
    else if(LV < 25) no_opp = 2;
    else if(LV < 40) no_opp = 3;
    else 			 no_opp = 4;

    std::cout << "You are on LV " << LV << ".\n";
    if(no_opp == 1)
    {
        std::cout << "You will battle 1 opponent.\n";
        std::cout << "Your opponent will be difficulty " << LV << ".\n";
    }
    else
    {
        std::cout << "You will battle " << no_opp << " opponents.\n";
        std::cout << "Your opponents will be difficulty " << LV << ".\n";
    }
    std::cout << "Continue?: ";
    std::cin.get();
	std::cin.get();
}

void tutorial()
{
    uint8_t input;

    const std::string options = R"(Welcome to the tutorial for C++ Battle Simulator
This battle simulator relies on the input of numbers, each of which correspond to a different action.

For battling, enter 1
To end the tutorial, enter 2 here: )";

    const std::string battling = R"(BATTLING
When you begin a battle, you will have multiple options to choose from.

You can Attack by entering 1, which will lower the opponent's health:
    When attacking, the word "wait..." will appear.
    Shortly after, the word "NOW!" will appear.
    When "NOW" appears, you must press enter.
    The quicker it is pressed, the more damage will be done.
    But if you press enter before the word "NOW!" appears, you will be penalized.

You can use Magic by entering 2, which will give you multiple options:
    You can Increase Defence by entering 1, which will decrease damage taken from opponent's attacks.
    You can Increase Attack by entering 2, which will increase the damage given to the opponent.
    You can Heal by entering 3, which will increase your health.
    You can go Back by entering 4, which will take you back to the first four options.

You can Use Items by entering 3, which will give you multiple options:
    You can Use Reflector by entering 1, which will reflect the damage from the opponent.
    You can Use Absorber by entering 2, which will absorb the damage from the opponent.
        Absorbing attacks increases your magic effectiveness.
    You can go Back by entering 3, which will take you back to the first four options.
    All items will decrease in efficiency the more that item is used.

You can Taunt by entering 4, which will make their attacks stronger, but their defence weaker.

After you have made your move, the opponent will make theirs with any of the above actions.
After both you and the opponent have made your actions, press enter to proceed.

Different actions are prioritized over others, so that they occur before the other action.
The prioritizing of these actions are are: 1. Items | 2. Magic | 3. Taunt | 4. Attack

To win the battle, you must lower the health of the opponent to 0.
But if your health gets lowered to 0 first, you will lose the battle.
Having both your health and the opponent's health lowered to 0 would cause a draw.)";

    while(true)
    {
		CLEAR;
        std::cout << options;
        std::cin >> input;
        CLEAR;
		
        switch(input)
        {
            case 0x31: // 1
                std::cout << battling << std::endl;
                std::cin.get();
				std::cin.get();
                break;
            
            case 0x32: // 2
                return;
        }
    }
}

void credits()
{
    const std::string credits = R"(
       Programming | Seth McDonald
                   |
    Special Thanks | Patrick Cornale
                   | Jaryn Oliphant
                   | Maisie Wallace
                   | Charlotte Cosgrove)";

    std::cout << credits << std::endl;
    std::cin.get();
}

void signIn()
{
	/*
	 Structure of account file:
	 
	 Line 1:
		Password
	 Line 2:
		LV
	 Line 3:
		EXP
	 */
	
    uint8_t input;
    std::string username;
    std::string password1;
	std::string password2;
    std::string file_pwd;
    bool uname_incorrect = false;
    bool pwd_incorrect = false;
    Details &details = Details::get();

    std::cout << "To sign in, enter 1\nTo sign up, enter 2\nTo continue as a guest, enter 3: ";
    std::cin >> input;
    CLEAR;
    
    switch(input)
    {
        case 0x31: // 1
            while(true)
            {
				CLEAR;
				
				std::cout << "Signing in\nEnter q to stop\n\n";
				
                if(uname_incorrect) std::cout << "Username is incorrect\n";
                else if(pwd_incorrect) std::cout << "Password is incorrect\n";

                std::cout << "Please enter your username: ";
				std::cin >> username;
				
				if(username == "q")
					return;
				
                std::cout << "Please enter your password: ";
				std::cin >> password1;
				
                std::ifstream login ("../Accounts/" + username + ".txt");

                if(!login.is_open())
                {
                    uname_incorrect = true;
                    continue;
                }

                login >> file_pwd;

                if(file_pwd != password1)
					pwd_incorrect = true;

                else
                {
                    login >> details.LV;
                    login >> details.EXP;
                    details.uname = username;
                    details.setPwd(password1);

                    std::cout << "Welcome back, " << username << std::endl;
                    std::cin.get();
					std::cin.get();
                    break;
                }
            }
            break;
        
		case 0x32: // 2
            while(true)
            {
				CLEAR;

				std::cout << "Signing up\n!No Spaces!\nEnter q to stop\n\n";
				
                if(pwd_incorrect) std::cout << "The two passwords do not match\n";
                else if(uname_incorrect) std::cout << "That username has already been taken\n";

                std::cout << "Enter your username: ";
				std::cin >> username;
				
				if(username == "q")
					return;
				
                std::cout << "Enter your password: ";
				std::cin >> password1;
               
                CLEAR;
				std::cout << "Signing up\n!No Spaces!\nEnter q to stop\n\n";
				
                if(pwd_incorrect) std::cout << "The two passwords do not match\n";
                else if(uname_incorrect) std::cout << "That username has already been taken\n";

                std::cout << "Enter your username: " << username << std::endl;
				std::cout << "Enter your password: \n";
                std::cout << "Confirm your password: ";
				std::cin >> password2;

                if(password1 != password2)
                {
                    pwd_incorrect = true;
                    continue;
                }

				std::filesystem::create_directory("../Accounts");
                std::ifstream check ("../Accounts/" + username + ".txt");

                if(check.is_open())
				{
					uname_incorrect = true;
					check.close();
					continue;
				}

				std::ofstream signup ("../Accounts/" + username + ".txt");
				signup << password1; // Write password into file
				signup << std::endl << 1; // Write LV
				signup << std::endl << 0; // Write EXP
				details.LV = 1;
				details.EXP = 0;
				details.uname = username;
				details.setPwd(password1);

				std::cout << username << " has successfully been created\n";
				std::cin.get();
				std::cin.get();
				break;
            }
            break;
        
        case 0x33: // 3
            details.setGuest();
            break;
    }
}

void signOut()
{
	if(!Details::get().LV)
		std::cout << "You are not logged in\n";
	else
	{
		Details::get().LV = 0;
		std::cout << "You have been successfully logged out\n";
	}
	std::cin.get();
	std::cin.get();
}
