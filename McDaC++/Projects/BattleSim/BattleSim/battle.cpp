#include <iostream>
#include <string>
#include <array>
#include <vector>
#include "start.hpp"
#include "fighter.hpp"
#include "battle.hpp"

#define CLEAR system("clear")

void battle_init(const std::string &name, const uint8_t no_opp, Details &details)
{
    std::vector<std::string> text;
    std::vector<Fighter *> fighters;
    fighters.reserve(no_opp + 1);

    User user(name, details.LV);
    fighters.emplace_back(&user);
    
    Computer opp1(1, details.LV);
    fighters.emplace_back(&opp1);
    
    if(no_opp == 2)
    {
        Computer opp2(2, details.LV);
        fighters.emplace_back(&opp2);
    }
    
    if(no_opp == 3)
    {
        Computer opp3(3, details.LV);
        fighters.emplace_back(&opp3);
    }
    
    if(no_opp == 4)
    {
        Computer opp4(4, details.LV);
        fighters.emplace_back(&opp4);
    }

    battle_loop(text, details, fighters);
}

void battle_loop(std::vector<std::string> &text, Details &details, std::vector<Fighter *> &fighters)
{
    do
    {
        text.clear();
        text.shrink_to_fit();

        fighters[0]->health_bar();
        
        switch(fighters.size())
        {
            case 5:
                fighters[4]->health_bar();
                fighters[4]->main_action();
            case 4:
                fighters[3]->health_bar();
                fighters[3]->main_action();
            case 3:
                fighters[2]->health_bar();
                fighters[2]->main_action();
            default:
                fighters[1]->health_bar();
                fighters[1]->main_action();
                break;
        }
        
        fighters[0]->main_action();

        action_priority(text, fighters);

        for(std::string line : text)
        {
            std::cout << line << std::endl;
        }
        std::cin.get();

        if(fighters.size() == 1 && (!fighters[0]->health || !fighters[1]->health))
        {
            CLEAR;
            break;
        }
        if(fighters.size() == 2 && (!fighters[0]->health || (!fighters[1]->health && !fighters[2]->health)))
        {
            CLEAR;
            break;
        }
        if(fighters.size() == 3 && (!fighters[0]->health || (!fighters[1]->health && !fighters[2]->health && !fighters[3]->health)))
        {
            CLEAR;
            break;
        }
        if(fighters.size() == 4 && (!fighters[0]->health || (!fighters[1]->health && !fighters[2]->health && !fighters[3]->health && !fighters[4]->health)))
        {
            CLEAR;
            break;
        }
    }
    while(true);

    if(fighters[0]->health)
    {
        std::cout << "VICTORY!!!\n";

        if(fighters.size() - 1 == 1) std::cout << "You knocked out a difficulty " << details.LV << " opponent!\n";

        else std::cout << "You knocked out " << fighters.size() - 1 << " difficulty " << details.LV << " opponents!\n";

        std::cout << "Remaining health: " << fighters[0]->health << std::endl;

        std::cin.get();
        CLEAR;

        details++;
        std::cout << "Your EXP increased by 1!\n";

        if(details.EXP == 0) std::cout << "Your LV increased!\n";
    }

    else
    {
        std::cout << "FAILURE\n";

        if(fighters.size() - 1 == 1)
        {
            std::cout << "You were knocked out by a difficulty " << details.LV << " opponent\n";
            std::cout << "Opponent's remaining health: " << fighters[1]->health << std::endl;
        }

        else
        {
            std::cout << "You were knocked out by " << fighters.size() - 1 << " difficulty " << details.LV << " opponents\n";

            if(fighters.size() - 1 == 2) std::cout << "Opponents' combined remaining health: " << fighters[1]->health + fighters[2]->health  << std::endl;

            else if(fighters.size() - 1 == 3) std::cout << "Opponents' combined remaining health: " << fighters[1]->health + fighters[2]->health + fighters[3]->health << std::endl;

            else std::cout << "Opponents' combined remaining health: " << fighters[1]->health + fighters[2]->health + fighters[3]->health + fighters[4]->health << std::endl;
        }

        std::cin.get();
        CLEAR;
    }
}

void action_priority(std::vector<std::string> &text, std::vector<Fighter *> &fighters)
{
    CLEAR;

    switch(fighters.size())
    {
        case 5:
            break;
        
        case 4:
            break;
        
        case 3:
            break;
        
        default:
            if(fighters[0]->reflector_use) text.emplace_back("You equiped your reflector");
    
            if(fighters[1]->reflector_use) text.emplace_back("Opponent 1 equiped their reflector");

            if(fighters[0]->absorber_use) text.emplace_back("You equiped your absorber");

            if(fighters[1]->absorber_use) text.emplace_back("Opponent 1 equiped their absorber");

            if(fighters[0]->def_inc_use) fighters[0]->inc_def(text);

            if(fighters[1]->def_inc_use) fighters[1]->inc_def(text);

            if(fighters[0]->atk_inc_use) fighters[0]->inc_atk(text);

            if(fighters[1]->atk_inc_use) fighters[1]->inc_atk(text);

            if(fighters[0]->hlth_inc_use) fighters[0]->inc_hlth(text);

            if(fighters[1]->hlth_inc_use) fighters[1]->inc_hlth(text);

            if(fighters[0]->taunt_use) fighters[0]->taunt(text, fighters.size() - 1);

            if(fighters[1]->taunt_use) fighters[1]->taunt(text);

            if(fighters[0]->get_attack_use() == 1)
            {
                fighters[0]->attack(text, fighters[1]->magic_def, fighters[1]->taunt_amount, 1);

                if(fighters[1]->reflector_use) fighters[1]->use_reflector(text, fighters[0]->damage, fighters[0]->health);

                else if(fighters[1]->absorber_use) fighters[1]->use_absorber(text, fighters[0]->damage);

                else 
                {
                    if(fighters[0]->damage > fighters[1]->health) fighters[0]->damage = fighters[1]->health;

                    fighters[1]->health -= fighters[0]->damage;

                    text.emplace_back("You inflicted " + std::to_string(fighters[0]->damage) + " damage");
                }
            }

            if(!fighters[1]->health) break;

            if(fighters[1]->get_attack_use())
            {
                fighters[1]->attack(text, fighters[0]->magic_def, fighters[0]->taunt_amount);

                if(fighters[0]->reflector_use) fighters[0]->use_reflector(text, fighters[0]->damage, fighters[0]->health, 1);

                else if(fighters[0]->absorber_use) fighters[0]->use_absorber(text, fighters[1]->damage, 1);

                else
                {
                    if(fighters[1]->damage > fighters[0]->health) fighters[1]->damage = fighters[0]->health;

                    fighters[0]->health -= fighters[1]->damage;

                    text.emplace_back("Opponent 1 inflicted " + std::to_string(fighters[1]->damage) + " damage");
                }
            }

            fighters[0]->reflector_use = false;
            fighters[0]->absorber_use = false;

            fighters[1]->reflector_use = false;
            fighters[1]->absorber_use = false;
             
            break;
    }
}

void multi_battle(const std::array<std::string, 2> &names)
{

}

void custom_battle(const std::array<uint16_t, 3> &values)
{

}
