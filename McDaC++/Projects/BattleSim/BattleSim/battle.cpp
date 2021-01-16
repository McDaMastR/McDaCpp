#include <iostream>
#include <string>
#include <array>
#include <vector>
#include "basic.hpp"
#include "start.hpp"
#include "fighter.hpp"
#include "battle.hpp"

void battle_init(const std::string &name, const int no_opp, Details &details)
{
    // Maybe create fighters on the heap
    std::vector<std::string> text;

    User user(name, details.LV);

    // switch(no_opp) Fix this
    // {
    //    case 4:
            // Computer opp4(4, details.LV);
    //    case 3:
            // Computer opp3(3, details.LV);
    //    case 2:
            // Computer opp2(2, details.LV);
    //    case 1:
            Computer opp1(1, details.LV);
    //        break;
    // }

    battle_loop(text, no_opp, details, user, opp1);
}

void battle_loop(std::vector<std::string> &text, const int no_opp, Details &details, User &user, Computer &opp1)
{
    do
    {
        text.clear();
        text.reserve(2);

        user.health_bar();
        // switch(no_opp) Fix this
        // {
        //     case 4:
        //         opp4.health_bar();
        //         opp4.main_action();
        //     case 3:
        //         opp3.health_bar();
        //         opp3.main_action();
        //     case 2:
        //         opp2.health_bar();
        //         opp2.main_action();
        //     case 1:
                opp1.health_bar();
                opp1.main_action();
        //         break;
        // }

        user.main_action();

        action_priority(text, no_opp, user, opp1);

        for(std::string line : text)
        {
            std::cout << line << std::endl;
        }
        std::cin.get();

        if(user.health == 0 || opp1.health == 0)
        {
            bsc::clear();
            break;
        }
    }
    while(true);

    if(user.health == 0)
    {
        std::cout << "VICTORY!!!\n";

        if(no_opp == 1) {std::cout << "You knocked out a difficulty " << details.LV << " opponent!\n";}

        else {std::cout << "You knocked out " << no_opp << " difficulty " << details.LV << " opponents!\n";}

        std::cout << "Remaining health: " << user.health << std::endl;

        std::cin.get();
        bsc::clear();

        details++;
        std::cout << "Your EXP increased by 1!\n";

        if(details.EXP == 0) {std::cout << "Your LV increased!\n";}
    }

    else
    {
        std::cout << "FAILURE\n";

        if(no_opp == 1) 
        {
            std::cout << "You were knocked out by a difficulty " << details.LV << " opponent\n";
            std::cout << "Opponent's remaining health: " << opp1.health << std::endl;
        }

        else // Fix this
        {
            std::cout << "You were knocked out by " << no_opp << " difficulty " << details.LV << " opponents\n";

            if(no_opp == 2) {std::cout << "Opponents' combined remaining health: " << opp1.health /*+ opp2.health */ << std::endl;}

            else if(no_opp == 3) {std::cout << "Opponents' combined remaining health: " << opp1.health /*+ opp2.health + opp3.health*/ << std::endl;}

            else {std::cout << "Opponents' combined remaining health: " << opp1.health /*+ opp2.health + opp3.health + opp4.health*/ << std::endl;}
        }

        std::cin.get();
        bsc::clear();
    }
}

void action_priority(std::vector<std::string> &text, const int no_opp, User &user, Computer &opp1)
{
    bsc::clear();

    switch(no_opp)
    {
        case 4:
            break;
        
        case 3:
            break;
        
        case 2:
            break;
        
        case 1:
            if(user.reflector_use) {text.emplace_back("You equiped your reflector");}
    
            if(opp1.reflector_use) {text.emplace_back("Opponent 1 equiped their reflector");}

            if(user.absorber_use) {text.emplace_back("You equiped your absorber");}

            if(opp1.absorber_use) {text.emplace_back("Opponent 1 equiped their absorber");}

            if(user.def_inc_use) {user.inc_def(text);}

            if(opp1.def_inc_use) {opp1.inc_def(text);}

            if(user.atk_inc_use) {user.inc_atk(text);}

            if(opp1.atk_inc_use) {opp1.inc_atk(text);}

            if(user.hlth_inc_use) {user.inc_hlth(text);}

            if(opp1.hlth_inc_use) {opp1.inc_hlth(text);}

            if(user.taunt_use) {user.taunt(text, no_opp);}

            if(opp1.taunt_use) {opp1.taunt(text);}

            if(user.attack_1) 
            {
                user.attack(text, 1, opp1.magic_def, opp1.taunt_amount);

                if(opp1.reflector_use) {opp1.use_reflector(text, user.damage, user.health);}

                else if(opp1.absorber_use) {opp1.use_absorber(text, user.damage);}

                else 
                {
                    if(user.damage > opp1.health) {user.damage = opp1.health;}

                    opp1.health -= user.damage;

                    text.emplace_back("You inflicted " + std::to_string(user.damage) + " damage");
                }
            }

            if(opp1.health == 0) {break;}

            if(opp1.attack_use)
            {
                opp1.attack(text, user.magic_def, user.taunt_amount);

                if(user.reflector_use) {user.use_reflector(text, 1, opp1.damage, opp1.health);}

                else if(user.absorber_use) {user.use_absorber(text, opp1.damage);}

                else
                {
                    if(opp1.damage > user.health) {opp1.damage = user.health;}

                    user.health -= opp1.damage;

                    text.emplace_back("Opponent 1 inflicted " + std::to_string(opp1.damage) + " damage");
                }
            }

            user.reflector_use = false;
            user.absorber_use = false;

            opp1.reflector_use = false;
            opp1.absorber_use = false;
             
            break;
    }
}

void multi_battle(const std::array<std::string, 2> &names)
{

}

void custom_battle(const std::array<int, 3> values)
{

}