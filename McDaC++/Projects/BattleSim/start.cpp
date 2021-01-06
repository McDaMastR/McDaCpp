#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include "start.hpp"
#include "basic.hpp"

void Details::set_pwd(const std::string pwd)
{
    this->pwd = pwd;
}

void Details::set_guest()
{
    guest = true;
    unsigned long number = inc_guests();
    LV = 1;
    EXP = 0;
    uname = "Guest" [+ number];
}

int Details::inc_guests()
{
    guest_num++;
    return guest_num;
}

void Details::operator = (Details details)
{
    LV = details.LV;
    EXP = details.EXP;
    uname = details.uname;
    guest = details.guest;
    pwd = details.pwd;
}

int start()
{
    int input;

    do
    {
        bsc::clear();

        std::cout << "Enter 1 to start\nEnter 2 for the tutorial\nEnter 3 for custom battle\nEnter 4 for multiplayer\n"
                    "Enter 5 for credits\nEnter 6 to login\nEnter 7 to log out\nEnter 8 to quit: ";
        std::cin >> input;
    }
    while(0 < input && input < 9);

    return input;
}

void cfm_battle(const int LV)
{
    int input;

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
            while(input != 1 && input != 2);

            if(input == 1) 
            {
                sign_in();
                continue;
            }
        }

        else
        {
            std::cout << "You are on LV " << LV << ".\n";
            std::cout << "Your opponent will also be difficulty " << LV << ".\n";
            std::cout << "Continue?: ";
            std::cin.get();
        }
    } 
    while(false);
}

int tutorial()
{
    short input;

    std::string options = "Welcome to the tutorial for C++ Battle Simulator\n\
This battle simulator relies on the input of numbers, each of which correspond to a different action.\n\
\n\
For battling, enter 1\n\
For multiplayer, enter 2\n\
To end the tutorial, enter 3 here: ";

    std::string battling = "BATTLING\n\
When you begin a battle, you will have multiple options to choose from.\n\
\n\
You can Attack by entering 1, which will lower the opponent's health:\n\
    When attacking, the word \"wait...\" will appear.\n\
    Shortly after, the word \"NOW!\" will appear.\n\
    When \"NOW\" appears, you must press enter.\n\
    The quicker it is pressed, the more damage will be done.\n\
    But if you press enter before the word \"NOW!\" appears, you will be penalized.\n\
\n\
You can use Magic by entering 2, which will give you multiple options:\n\
    You can Increase Defence by entering 1, which will decrease damage taken from opponent's attacks.\n\
    You can Increase Attack by entering 2, which will increase the damage given to the opponent.\n\
    You can Heal by entering 3, which will increase your health.\n\
    You can go Back by entering 4, which will take you back to the first four options.\n\
\n\
You can Use Items by entering 3, which will give you multiple options:\n\
    You can Use Reflector by entering 1, which will reflect the damage from the opponent.\n\
    You can Use Absorber by entering 2, which will absorb the damage from the opponent.\n\
        Absorbing attacks increases your magic effectiveness.\n\
    You can go Back by entering 3, which will take you back to the first four options.\n\
    All items will decrease in efficiency the more that item is used.\n\
\n\
You can Taunt by entering 4, which will make their attacks stronger, but their defence weaker.\n\
\n\
After you have made your move, the opponent will make theirs with any of the above actions.\n\
After both you and the opponent have made your actions, press enter to proceed.\n\
\n\
Different actions are prioritized over others, so that they occur before the other action.\n\
The prioritizing of these actions are are: 1. Items | 2. Magic | 3. Taunt | 4. Attack\n\
\n\
To win the battle, you must lower the health of the opponent to 0.\n\
But if your health gets lowered to 0 first, you will lose the battle.\n\
Having both your health and the opponent's health lowered to 0 would cause a draw.";

    std::string multiplayer = "MULTIPLAYER\n\
By entering in 4 at the menu screen, you can begin multiplayer mode.\n\
\n\
Multiplayer mode is where you battle a human instead of a computer.\n\
\n\
Multiplayer battling works the same way that computer battling works, but with a few differences:\n\
    When starting, both players must enter their name.\n\
    Each player will take one action each turn, first player 1, then player 2.";

    do
    {
        std::cout << options;
        std::cin >> input;
        bsc::clear();

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
                return 0;
        }
        bsc::clear();
    }
    while(true);
}

std::array<int, 3> custom(const int LV, const bool guest)
{
    int input;
    int LV_input;
    int difficulty_input;
    std::array<int, 3> values = {0, 0, 0};

    if(LV == 0)
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
            return values;
        }

        else {return values;}
    }

    else if(guest == true)
    {
        std::cout << "To play custom mode, you need to be signed in with an account\n";
        std::cin.get();
        return values;
    }

    else if(LV < 30)
    {
        std::cout << "To play custom mode, you need to be of at least LV 30\n";
        std::cin.get();
        return values;
    }

    do
    {
        std::cout << "What will be your LV?: ";
        std::cin >> LV_input;

        if(LV_input < 1) {continue;}

        std::cout << "What will be the difficulty of your opponent/s?: ";
        std::cin >> difficulty_input;

        if(difficulty_input < 1) {continue;}
    }
    while(false);

    values[0] = LV_input;
    values[1] = difficulty_input;
    values[2] = 1;
    return values;
}

std::array<std::string, 2> multiplayer()
{
    int i = 0;
    std::array<std::string, 2> unames;

    std::cout << "Enter player 1's username: ";
    getline(std::cin, unames[0]);

    do
    {
        if(i != 0) 
        {
            bsc::clear();
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
    std::string credits = "\
       Programming | Seth McDonald\n\
    Special Thanks | Patrick Cornale\n\
                   | Jaryn Oliphant\n\
                   | Maisie Wallace\n\
                   | Charlotte Cosgrove";

    std::cout << credits << std::endl;
    std::cin.get();
}

Details sign_in()
{
    int input;
    unsigned int LV;
    unsigned int EXP;
    std::string username;
    std::string password;
    std::string file_pwd;
    bool uname_incorrect = false;
    bool pwd_incorrect = false;
    Details details;

    std::cout << "To sign in, enter 1\nTo sign up, enter 2\nTo continue as a guest, enter 3: ";
    std::cin >> input;
    bsc::clear();
    
    switch(input)
    {
        case 1:
            do
            {
                if(uname_incorrect) {std::cout << "Username is incorrect\n";}
                else if(pwd_incorrect) {std::cout << "Password is incorrect\n";}

                std::cout << "Please enter your username: ";
                getline(std::cin, username);
                std::cout << "Please enter your password: ";
                getline(std::cin, password);

                std::ifstream login ("/Accounts/" + username + ".txt");

                if(!login.is_open())
                {
                    uname_incorrect = true;
                    continue;
                }

                login >> file_pwd;

                if(file_pwd != password) {pwd_incorrect = true;}

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

                if(pwd_dont_match) {std::cout << "The two passwords do not match\n";}
                else if(uname_taken) {std::cout << "That username has already been taken\n";}

                std::cout << "Enter your username: ";
                getline(std::cin, username);
                std::cout << "Enter your password: ";
                getline(std::cin, password1);
               
                bsc::clear();
                if(pwd_dont_match) {std::cout << "The two passwords do not match\n";}
                else if(uname_taken) {std::cout << "That username has already been taken\n";}

                std::cout << "Enter your username: " << username << std::endl;
                std::cout << "Confirm your password: ";
                getline(std::cin, password2);

                if(password1 != password2)
                {
                    pwd_dont_match = true;
                    continue;
                }

                std::ifstream check ("/Accounts/" + username + ".txt");

                if(check.is_open()) {uname_taken = true;}

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
    return details;
}