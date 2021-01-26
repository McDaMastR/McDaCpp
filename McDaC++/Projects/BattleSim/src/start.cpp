#include "../include/start.hpp"

#define CLEAR system("clear")

Details &Details::get()
{
    static Details details;
    return details;
}

void Details::set_pwd(const std::string &pwd) {get().private_set_pwd(pwd);}

void Details::set_guest() {get().private_set_guest();}

void Details::private_set_pwd(const std::string &pwd) {this->pwd = pwd;}

void Details::private_set_guest()
{
    guest = true;
    uint16_t number = inc_guests();
    LV = 1;
    EXP = 0;
    uname = "Guest" + std::to_string(number);
}
 
uint8_t Details::inc_guests()
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
    #ifdef DEBUG
    printf("Start function successfully called\nInput = %i\n", input);
    #endif

    const std::string msg = R"(Enter 1 to start
Enter 2 for the tutorial
Enter 3 for credits
Enter 4 to login
Enter 5 to log out
Enter 6 to quit: )";

    #ifdef DEBUG
    printf("After init of msg var\nInput = %i\n", input);
    std::cout << msg;
    printf("After outputing msg var\nInput = %i\n", input);
    std::cin >> input; // Input being interpreted as hex not char
    printf("Input = %i\nEnding start() function\n", input);
    #else
    std::cout << msg;
    std::cin >> input;
    #endif
}

std::optional<uint8_t> cfm_battle()
{
    union 
    {
        uint8_t input;
        uint8_t no_opp;
    };
    uint16_t LV = Details::get().LV;

    do
    {
       if(LV == 0)
        {
            do
            {
                std::cout << "You have to be signed in to battle\n";
                std::cout << "Enter 1 to sign in\nEnter 2 to go to home screen: ";
                std::cin >> input;
            }
            while(!(input == 1 || input == 2));

            if(input == 1) 
            {
                sign_in();
                continue;
            }

            return {};
        }

        if(LV < 10) no_opp = 1;
        else if(LV < 25) no_opp = 2;
        else if(LV < 40) no_opp = 3;
        else no_opp = 4;

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
    } 
    while(false);

    return no_opp;
}

void tutorial()
{
    uint8_t input;

    const std::string options = R"(Welcome to the tutorial for C++ Battle Simulator
This battle simulator relies on the input of numbers, each of which correspond to a different action.

For battling, enter 1
For multiplayer, enter 2
To end the tutorial, enter 3 here: )";

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

    const std::string multiplayer = R"(MULTIPLAYER
By entering in 4 at the menu screen, you can begin multiplayer mode.

Multiplayer mode is where you battle a human instead of a computer.

Multiplayer battling works the same way that computer battling works, but with a few differences:
    When starting, both players must enter their name.
    Each player will take one action each turn, first player 1, then player 2.)";

    do
    {
        std::cout << options;
        std::cin >> input;
        CLEAR;

        switch(input)
        {
            case 1:
                std::cout << battling << std::endl;
                std::cin.get();
                break;
            
            case 2:
                std::cout << multiplayer << std::endl;
                std::cin.get();
                break;
            
            case 3:
                return;
        }
        CLEAR;
    }
    while(true);
}

std::optional<std::array<uint16_t, 3>> custom()
{
    uint8_t input;
    uint16_t LV_input;
    uint16_t difficulty_input;
    std::array<uint16_t, 3> values;

    do
    {
        if(Details::get().LV == 0)
        {
            do
            {
                std::cout << "To play custom mode, you need to sign in\n";
                std::cout << "To sign in, enter 1\nTo go to home screen, enter 2: ";
                std::cin >> input;
            }
            while(input != 1 && input != 2);

            if(input == 1)
            {
                sign_in();
                continue;
            }

            else return {};
        }

        else if(Details::get().guest)
        {
            std::cout << "To play custom mode, you need to be signed in with an account\n";
            std::cin.get();
            return {};
        }

        else if(Details::get().LV < 30)
        {
            std::cout << "To play custom mode, you need to be of at least LV 30\n";
            std::cin.get();
            return {};
        }
    }
    while(false);

    do
    {
        CLEAR;

        std::cout << "What will be your LV?: ";
        std::cin >> LV_input;

        if(LV_input < 1) continue;

        std::cout << "What will be the difficulty of your opponent/s?: ";
        std::cin >> difficulty_input;

        if(difficulty_input < 1) continue;
    }
    while(false);

    values[0] = LV_input;
    values[1] = difficulty_input;
    values[2] = 1;
    return values;
}

std::array<std::string, 2> multiplayer()
{
    uint8_t i = 0;
    std::array<std::string, 2> unames;

    std::cout << "Enter player 1's username: ";
    getline(std::cin, unames[0]);

    do
    {
        if(i != 0) 
        {
            CLEAR;
            std::cout << "Enter player 1's username: " << unames[0] << std::endl;
            std::cout << "That username has already been taken\n";
        }

        std::cout << "Enter player 2's username: ";
        getline(std::cin, unames[1]);
        i++;
    }
    while(unames[0] == unames[1]);

    return unames;
}

void credits()
{
    std::string credits = R"(
       Programming | Seth McDonald
                   |
    Special Thanks | Patrick Cornale
                   | Jaryn Oliphant
                   | Maisie Wallace
                   | Charlotte Cosgrove)";

    std::cout << credits << std::endl;
    std::cin.get();
}

void sign_in()
{
    uint8_t input;
    std::string username;
    std::string password;
    std::string file_pwd;
    bool uname_incorrect = false;
    bool pwd_incorrect = false;
    Details &details = Details::get();

    std::cout << "To sign in, enter 1\nTo sign up, enter 2\nTo continue as a guest, enter 3: ";
    std::cin >> input;
    CLEAR;
    
    switch(input)
    {
        case 1:
            do
            {
                if(uname_incorrect) std::cout << "Username is incorrect\n";
                else if(pwd_incorrect) std::cout << "Password is incorrect\n";

                std::cout << "Please enter your username: ";
                std::cin.ignore();
                getline(std::cin, username);
                std::cout << "Please enter your password: ";
                std::cin.ignore();
                getline(std::cin, password);

                std::ifstream login ("/Accounts/" + username + ".txt");

                if(!login.is_open())
                {
                    uname_incorrect = true;
                    continue;
                }

                login >> file_pwd;

                if(file_pwd != password) pwd_incorrect = true;

                else
                {
                    login >> details.LV;
                    login >> details.EXP;
                    details.uname = username;
                    details.set_pwd(password);

                    std::cout << "Welcome back, " << username << std::endl;
                    std::cin.get();
                    break;
                }
            }
            while (true);
            break;
        
        case 2:
            do
            {
                std::string username;
                std::string password1;
                std::string password2;
                bool pwd_dont_match = false;
                bool uname_taken = false;

                if(pwd_dont_match) std::cout << "The two passwords do not match\n";
                else if(uname_taken) std::cout << "That username has already been taken\n";

                std::cout << "Enter your username: ";
                getline(std::cin, username);
                std::cout << "Enter your password: ";
                getline(std::cin, password1);
               
                CLEAR;
                if(pwd_dont_match) std::cout << "The two passwords do not match\n";
                else if(uname_taken) std::cout << "That username has already been taken\n";

                std::cout << "Enter your username: " << username << std::endl;
                std::cout << "Confirm your password: ";
                getline(std::cin, password2);

                if(password1 != password2)
                {
                    pwd_dont_match = true;
                    continue;
                }

                std::ifstream check ("/Accounts/" + username + ".txt");

                if(check.is_open()) uname_taken = true;

                else
                {
                    std::ofstream signup ("/Accounts/" + username + ".txt");
                    signup << password1;
                    signup << 1;
                    signup << 0;
                    details.LV = 1;
                    details.EXP = 0;
                    details.uname = username;
                    details.set_pwd(password1);

                    std::cout << username << " has successfully been created\n";
                    std::cin.get();
                    break;
                }
            }
            while(true);
            break;
        
        case 3:
            details.set_guest();
            break;
    }
}
