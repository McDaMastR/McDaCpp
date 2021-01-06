#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <unistd.h>
#include "fighter.hpp"
#include "basic.hpp"

Fighter::Fighter(const int LV)
    : LV(LV), max_health(40 * LV), health(40 * LV) {add_one();}

Fighter::~Fighter() {take_one();}

void Fighter::add_one() {no_inst++;}

void Fighter::take_one() {no_inst--;}

int Fighter::get_inst() {return no_inst;}

void Fighter::health_bar() {std::cout << health << "/" << max_health << std::endl;}

void Fighter::use_reflector(std::vector<std::string> &text, unsigned int opp_attack, unsigned int &opp_health, const int opp_no)
{
    item_random = rand() % 101;

    if(opp_attack == 0) 
    {
        if(opp_no == 0) {text.push_back("You missed!");}

        else {text.push_back("Opponent " + std::to_string(opp_no) + "'s attack missed!");}
    }

    else if(reflector_health < 0)
    {
        if(opp_attack > health) {opp_attack = health;}
             
        health -= opp_attack;
            
        if(opp_no == 0) 
        {
            text.push_back("But opponent " + std::to_string(number) + "'s reflector had already been destroyed!");
            text.push_back("You inflicted " + std::to_string(opp_attack) + " damage");
        }

        else 
        {
            text.push_back("But your reflector had already been destroyed!");
            text.push_back("Opponent " + std::to_string(opp_no) + " inflicted " + std::to_string(opp_attack) + " damage");
        }
    }

    else if(reflector_health == 0)
    {
        opp_attack *= 1.6;
            
        if(opp_attack > health) {opp_attack = health;}

        health -= opp_attack;
        reflector_health -= 20;

        if(opp_no == 0) 
        {
            text.push_back("You destroyed opponent " + std::to_string(number) + "'s reflector!");
            text.push_back("You inflicted " + std::to_string(opp_attack) + " damage");
        }

        else 
        {
            text.push_back("Opponent " + std::to_string(opp_no) + "'s attack destroyed your reflector!");
            text.push_back("Opponent " + std::to_string(opp_no) + " inflicted " + std::to_string(opp_attack) + " damage");
        }
    }

    else if(item_random > reflector_health)
    {
        opp_attack *= 1.3;

        if(opp_attack > health) {opp_attack = health;}

        health -= opp_attack;

        if(opp_no == 0) 
        {
            text.push_back("You broke through opponent " + std::to_string(number) + "'s reflector!");
            text.push_back("You inflicted " + std::to_string(opp_attack) + " damage");
        }

        else 
        {
            text.push_back("Opponent " + std::to_string(opp_no) + "'s attack broke through your reflector!");
            text.push_back("Opponent " + std::to_string(opp_no) + " inflicted " + std::to_string(opp_attack) + " damage");
        }
    }

    else
    {
        if(opp_attack > opp_health) {opp_attack = opp_health;}

        opp_health -= opp_attack;

        reflector_health -= 20;

        if(opp_no == 0) 
        {
            text.push_back("Your attack was reflected!");
            text.push_back("Your reflection inflicted " + std::to_string(opp_attack) + " damage");
        }

        else 
        {
            text.push_back("Opponent " + std::to_string(opp_no) + "'s attack was reflected!");
            text.push_back("Opponent " + std::to_string(opp_no) + "'s reflection inflicted " + std::to_string(opp_attack) + " damage");
        }
    }
}

void Fighter::use_absorber(std::vector<std::string> &text, unsigned int opp_attack, const int opp_no)
{
    item_random = rand() % 101;

    if(opp_attack == 0) 
    {
        if(opp_no == 0) {text.push_back("You missed!");}

        else {text.push_back("Opponent " + std::to_string(opp_no) + "'s attack missed!");}
    }

    else if(absorber_health < 0)
    {
        if(opp_attack > health) {opp_attack = health;}
             
        health -= opp_attack;
            
        if(opp_no == 0) 
        {
            text.push_back("But opponent " + std::to_string(number) + "'s absorber had already been destroyed!");
            text.push_back("You inflicted " + std::to_string(opp_attack) + " damage");
        }

        else 
        {
            text.push_back("But your absorber had already been destroyed!");
            text.push_back("Opponent " + std::to_string(opp_no) + " inflicted " + std::to_string(opp_attack) + " damage");
        }
    }

    else if(absorber_health == 0)
    {
        opp_attack *= 1.6;
            
        if(opp_attack > health) {opp_attack = health;}

        health -= opp_attack;
        absorber_health -= 20;

        if(opp_no == 0) 
        {
            text.push_back("You destroyed opponent " + std::to_string(number) + "'s absorber!");
            text.push_back("You inflicted " + std::to_string(opp_attack) + " damage");
        }

        else 
        {
            text.push_back("Opponent " + std::to_string(opp_no) + "'s attack destroyed your absorber!");
            text.push_back("Opponent " + std::to_string(opp_no) + " inflicted " + std::to_string(opp_attack) + " damage");
        }
    }

    else if(item_random > absorber_health)
    {
        opp_attack *= 1.3;

        if(opp_attack > health) {opp_attack = health;}

        health -= opp_attack;

        if(opp_no == 0) 
        {
            text.push_back("You broke through opponent " + std::to_string(number) + "'s absorber!");
            text.push_back("You inflicted " + std::to_string(opp_attack) + " damage");
        }

        else 
        {
            text.push_back("Opponent " + std::to_string(opp_no) + "'s attack broke through your absorber!");
            text.push_back("Opponent " + std::to_string(opp_no) + " inflicted " + std::to_string(opp_attack) + " damage");
        }
    }

    else
    {
        opp_attack /= 200;

        if(opp_attack < 1)
        {
            opp_attack++;
        }

        magic_effect *= opp_attack;
        absorber_health -= 20;

        if(opp_no == 0) 
        {
            text.push_back("Your attack was absorbed!");
            text.push_back("Opponent " + std::to_string(number) + "'s magic effectiveness increased by " + std::to_string(opp_attack * 100 - 100) + "%");
        }

        else 
        {
            text.push_back("Opponent " + std::to_string(opp_no) + "'s attack was absorbed!");
            text.push_back("Your magic effectiveness increased by " + std::to_string(opp_attack * 100 - 100) + "%");
        }
    }
}

void Fighter::inc_def(std::vector<std::string> &text, const int opp_no)
{
    def_inc_use = false;

    def_inc = (((rand() % 501) + 500) / 1000) / magic_effect;
    magic_def *= def_inc;

    if(opp_no == 0) 
    {
        text.push_back("Opponent " + std::to_string(number) + " used magic on their defence");
        text.push_back("Opponent " + std::to_string(number) + "'s defence increased by " + std::to_string((int) (100 - def_inc * 100)) + "%!");
    }

    else 
    {
        text.push_back("You used magic on your defence");
        text.push_back("Your defence increased by " + std::to_string((int) (100 - def_inc * 100)) + "%!");
    }
}

void Fighter::inc_atk(std::vector<std::string> &text, const int opp_no)
{
    atk_inc_use = false;

    atk_inc = (((rand() % 501) + 1100)/1000) * magic_effect;
    magic_atk *= atk_inc;

    if(opp_no == 0) 
    {
        text.push_back("Opponent " + std::to_string(number) + " used magic on their attack");
        text.push_back("Opponent " + std::to_string(number) + "'s attack increased by " + std::to_string((int) (atk_inc * 100 - 100)) + "%!");
    }

    else 
    {
        text.push_back("You used magic on your attack");
        text.push_back("Your attack increased by " + std::to_string((int) (atk_inc * 100 - 100)) + "%!");
    }
}

void Fighter::inc_hlth(std::vector<std::string> &text, const int opp_no)
{
    hlth_inc_use = false;

    hlth_inc = ((rand() % 31) + 10) * magic_effect * LV / 5;
    health += hlth_inc;

    if(health > max_health)
    {
        hlth_inc -= health - max_health;
        health = max_health;
    }

    if(opp_no == 0) 
    {
        text.push_back("Opponent " + std::to_string(number) + " used magic on their health");
        text.push_back("Opponent " + std::to_string(number) + "'s health increased by " + std::to_string(hlth_inc) + "!");
    }

    else 
    {
        text.push_back("You used magic on your health");
        text.push_back("Your health increased by " + std::to_string(hlth_inc) + "!");
    }
}

void Fighter::taunt(std::vector<std::string> &text, const int opp_no)
{
    taunt_use = false;
    taunt_amount += ((rand() % 51) + 20) / 100;

    if(opp_no == 0) 
    {
        text.push_back("Opponent " + std::to_string(number) + " taunted you");
        text.push_back("You got frustrated...");
        text.push_back("Your attack increased!");
        text.push_back("Your defence decreased!");
    }

    else 
    {
        if(get_inst() - 1 == 1)
        {
            text.push_back("You taunted the opponent");
            text.push_back("The opponent got irritated...");
            text.push_back("The opponent's attack increased!");
            text.push_back("The opponent's defence decreased!");
        }

        else
        {
            text.push_back("You taunted the opponents");
            text.push_back("The opponents got frustrated...");
            text.push_back("The opponents' attack increased!");
            text.push_back("The opponents' defence decreased!");
        }
    }
}

User::User(const std::string name, const int LV)
    : Fighter(LV), name(name) {}

void User::main_action()
{
    loop = true;

    do
    {
        std::cout << "| Items | Magic | Taunt | Attack | ";
        std::cin >> main_input;

        switch(main_input)
        {
            case 1:
                item_action();
                break;
                    
            case 2:
                magic_action();
                break;
                    
            case 3:
                taunt_action();
                break;

            case 4:
                attack_action();
                break;
        }
        bsc::clear();
    }
    while(loop);
}

int User::item_action()
{
    do
    {
        std::cout << "| Reflector | Absorber | Back | ";
        std::cin >> secondary_input;

        switch(secondary_input)
        {
            case 1:
                reflector_use = true;
                loop = false;
                return 0;

            case 2:
                absorber_use = true;
                loop = false;
                return 0;

            case 3:
                return 0;
        }
    }
    while(true);
}

int User::magic_action()
{
    do
    {
        std::cout << "| Defence | Attack | Health | Back | ";
        std::cin >> secondary_input;

        switch(secondary_input)
        {
            case 1:
                def_inc_use = true;
                loop = false;
                return 0;

            case 2:
                atk_inc_use = true;
                loop = false;
                return 0;

            case 3:
                hlth_inc_use = true;
                loop = false;
                return 0;

            case 4:
                return 0;
        }
    }
    while(true);
}

int User::taunt_action()
{
    do
    {
        std::cout << "| All | Back | ";
        std::cin >> secondary_input;

        switch(secondary_input)
        {
            case 1:
                taunt_use = true;
                loop = false;
            case 2:
                return 0;
        }
    }
    while(true);
}

int User::attack_action()
{
    do
    {
        std::cout << "| Opp1 | Back | ";
        std::cin >> secondary_input;

        switch(secondary_input)
        {
            case 1:
                attack_1 = true;
                loop = false;
            case 2:
                return 0;
        }
    }
    while(true);
}

void User::attack(std::vector<std::string> &text, const int opp_no, const double opp_def, const double opp_taunt)
{
    then = 0;

    damage = magic_atk * taunt_amount * opp_def * opp_taunt * LV / 5;

    std::cout << "Wait...";
    sleep((rand() % 2) + 2);
    then = time(0);
    std::cout << "NOW!: ";
    std::cin.get();
    passed = time(0) - then;

    if(passed <= 0.1) {damage *= 60;}

    else if(passed <= 0.15) {damage *= 58;}

    else if(passed <= 0.2) {damage *= 56;}

    else if(passed <= 0.25) {damage *= 54;}
    
    else if(passed <= 0.3) {damage *= 52;}
    
    else if(passed <= 0.35) {damage *= 50;}
    
    else if(passed <= 0.4) {damage *= 48;}
    
    else if(passed <= 0.45) {damage *= 46;}
    
    else if(passed <= 0.5) {damage *= 44;}
    
    else if(passed <= 0.55) {damage *= 42;}

    else if(passed <= 0.6) {damage *= 40;}
    
    else if(passed <= 0.65) {damage *= 38;}
    
    else if(passed <= 0.7) {damage *= 36;}
    
    else if(passed <= 0.75) {damage *= 34;}
    
    else if(passed <= 0.8) {damage *= 32;}
    
    else if(passed <= 0.85) {damage *= 30;}
    
    else if(passed <= 0.9) {damage *= 28;}
    
    else if(passed <= 0.95) {damage *= 26;}

    else if(passed <= 1) {damage *= 24;}
    
    else if(passed <= 1.1) {damage *= 22;}
    
    else if(passed <= 1.2) {damage *= 20;}
    
    else if(passed <= 1.3) {damage *= 18;}
    
    else if(passed <= 1.4) {damage *= 16;}

    else if(passed <= 1.5) {damage *= 14;}

    else if(passed <= 1.6) {damage *= 12;}
    
    else if(passed <= 1.7) {damage *= 10;}
    
    else if(passed <= 1.8) {damage *= 8;}
    
    else if(passed <= 1.9) {damage *= 6;}
    
    else if(passed <= 2) {damage *= 4;}
    
    else if(passed <= 2.5) {damage *= 2;}
    
    else {damage = 0;}

    text.push_back("You attacked opponent " + std::to_string(opp_no));
}

Computer::Computer(const int number, const int LV)
    : Fighter(LV), number(number) {}

void Computer::main_action()
{
    main_input = rand() % 101;

    if(main_input < 40)
    {
        attack_use = true;
    }
    else if(main_input < 70)
    {
        secondary_input = (rand() % 90) + 1;

        if(secondary_input < 31) {def_inc_use = true;}

        else if(secondary_input < 61) {atk_inc_use = true;}

        else {hlth_inc_use = true;}
    }
    else if(main_input < 90)
    {
        secondary_input = rand() % 101;
        item_random = rand() % 101;

        if(secondary_input < 51) {reflector_use = true;}

        else {absorber_use = true;}
    }
    else
    {
        taunt_use = true;
    }
}

void Computer::attack(std::vector<std::string> &text, const double opp_defence, const double opp_taunt)
{
    damage = ((rand() % 30) + 1) * magic_atk * taunt_amount;
    damage *= opp_taunt * opp_defence * LV / 5;

    if(damage < LV) {damage += LV;}

    text.push_back("Opponent " + std::to_string(number) + " attacked you!");
}