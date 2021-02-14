#include "../include/battle.hpp"

static void battleLoop(std::vector<std::string> &text, std::vector<Fighter *> &fighters);
static void actionPriority(std::vector<std::string> &text, std::vector<Fighter *> &fighters);
static void battleOver(std::vector<Fighter *> &fighters);

void battleInit(const uint16_t no_opp)
{
    std::vector<std::string> text;
    std::vector<Fighter *> fighters;
    fighters.reserve(no_opp);

    User user(Details::get().uname, Details::get().LV);
    fighters.emplace_back(&user);

    Computer opp1(1, static_cast<uint16_t>(Details::get().LV * 1.65f));
    fighters.emplace_back(&opp1);

    if(Computer opp2(2, static_cast<uint16_t>(Details::get().LV * 1.65f)); no_opp == 2) fighters.emplace_back(&opp2);
    if(Computer opp3(3, static_cast<uint16_t>(Details::get().LV * 1.65f)); no_opp == 3) fighters.emplace_back(&opp3);
    if(Computer opp4(4, static_cast<uint16_t>(Details::get().LV * 1.65f)); no_opp == 4) fighters.emplace_back(&opp4);

    battleLoop(text, fighters);
}

static void battleLoop(std::vector<std::string> &text, std::vector<Fighter *> &fighters)
{
    while(true)
    {
        text.clear();

        fighters[0]->healthBar();
        switch(fighters.size())
        {
            case 5:
                fighters[4]->healthBar();
                fighters[4]->mainAction();
                [[fallthrough]];
            case 4:
                fighters[3]->healthBar();
                fighters[3]->mainAction();
                [[fallthrough]];
            case 3:
                fighters[2]->healthBar();
                fighters[2]->mainAction();
                [[fallthrough]];
            default:
                fighters[1]->healthBar();
                fighters[1]->mainAction();
                break;
        }
        fighters[0]->mainAction();

        if(fighters[0]->use == Fighter::Use::null)
            continue;

        actionPriority(text, fighters);

        for(std::string line : text)
            std::cout << line << '\n';
        std::cin.get();
        std::cin.get();

		CLEAR;

        if( (fighters.size() == 2 && (!fighters[0]->health || (!fighters[1]->health                                               	))) ||
			(fighters.size() == 3 && (!fighters[0]->health || (!fighters[1]->health && !fighters[2]->health						    ))) ||
			(fighters.size() == 4 && (!fighters[0]->health || (!fighters[1]->health && !fighters[2]->health && !fighters[3]->health ))) ||
			(fighters.size() == 5 && (!fighters[0]->health || (!fighters[1]->health && !fighters[2]->health && !fighters[3]->health && !fighters[4]->health))))
            
			break;
    }
	battleOver(fighters);
}

static void actionPriority(std::vector<std::string> &text, std::vector<Fighter *> &fighters)
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
            if(fighters[0]->use == Fighter::Use::reflector) 	text.emplace_back("You equiped your reflector");
            if(fighters[1]->use == Fighter::Use::reflector) 	text.emplace_back("Opponent 1 equiped their reflector");
            if(fighters[0]->use == Fighter::Use::absorber ) 	text.emplace_back("You equiped your absorber");
            if(fighters[1]->use == Fighter::Use::absorber ) 	text.emplace_back("Opponent 1 equiped their absorber");

            if(fighters[0]->use == Fighter::Use::def_inc  ) 	fighters[0]->incDef (text);
            if(fighters[1]->use == Fighter::Use::def_inc  ) 	fighters[1]->incDef (text);
            if(fighters[0]->use == Fighter::Use::atk_inc  ) 	fighters[0]->incAtk (text);
            if(fighters[1]->use == Fighter::Use::atk_inc  ) 	fighters[1]->incAtk (text);
            if(fighters[0]->use == Fighter::Use::hlth_inc ) 	fighters[0]->incHlth(text);
            if(fighters[1]->use == Fighter::Use::hlth_inc ) 	fighters[1]->incHlth(text);

            if(fighters[0]->use == Fighter::Use::taunt    )  	fighters[0]->taunt(text);
            if(fighters[1]->use == Fighter::Use::taunt    )		fighters[1]->taunt(text);

            if(fighters[0]->use == Fighter::Use::attack_1 )
            {
                fighters[0]->attack(text, fighters[1]->magic_def, fighters[1]->taunt_amount, 1);

                if(fighters[1]->use == Fighter::Use::reflector)
                    fighters[1]->useReflector(text, fighters[0]->damage, fighters[0]->health);

                else if(fighters[1]->use == Fighter::Use::absorber)
                    fighters[1]->useAbsorber(text, fighters[0]->damage);

                else
                {
                    if(fighters[0]->damage > fighters[1]->health) 
                        fighters[0]->damage = fighters[1]->health;

                    fighters[1]->health -= fighters[0]->damage;

					if(fighters[0]->damage)
						text.emplace_back("You inflicted " + std::to_string(fighters[0]->damage) + " damage");
					else
						text.emplace_back("You missed!");
                }
            }

            if(!fighters[1]->health)
                break;

            if(fighters[1]->use == Fighter::Use::attack_use)
            {
                fighters[1]->attack(text, fighters[0]->magic_def, fighters[0]->taunt_amount);

                if(fighters[0]->use == Fighter::Use::reflector)
                    fighters[0]->useReflector(text, fighters[1]->damage, fighters[1]->health, 1);

                else if(fighters[0]->use == Fighter::Use::absorber)
                    fighters[0]->useAbsorber(text, fighters[1]->damage, 1);

                else
                {
                    if(fighters[1]->damage > fighters[0]->health) 
                        fighters[1]->damage = fighters[0]->health;

                    fighters[0]->health -= fighters[1]->damage;
					
					if(fighters[1]->damage)
						text.emplace_back("Opponent 1 inflicted " + std::to_string(fighters[1]->damage) + " damage");
					else
						text.emplace_back("Opponent 1 missed!");
                }
            }

            fighters[0]->use = Fighter::Use::null;
            fighters[1]->use = Fighter::Use::null;
            break;
    }
}

static void battleOver(std::vector<Fighter *> &fighters)
{
    Details &details = Details::get();
    
	if(fighters[0]->health)
	{
		std::cout << "VICTORY!!!\n";

		if(fighters.size() - 1 == 1)
			std::cout << "You knocked out a difficulty " << details.LV << " opponent!\n";

		else
			std::cout << "You knocked out " << fighters.size() - 1 << " difficulty " << details.LV << " opponents!\n";

		std::cout << "Remaining health: " << fighters[0]->health << '\n';

		std::cin.get();
		CLEAR;

		details++;
		std::cout << "Your EXP increased by 1!\n";

		if(details.EXP == 0)
			std::cout << "Your LV increased!\n";
	}
	else
	{
		std::cout << "FAILURE\n";

		if(fighters.size() - 1 == 1)
		{
			std::cout << "You were knocked out by a difficulty " << details.LV << " opponent\n";
			std::cout << "Opponent's remaining health: " << fighters[1]->health << '\n';
		}
		else
		{
			std::cout << "You were knocked out by " << fighters.size() - 1 << " difficulty " << details.LV << " opponents\n";

			if(fighters.size() - 1 == 2)
				std::cout << "Opponents' combined remaining health: " << fighters[1]->health + fighters[2]->health  << '\n';

			else if(fighters.size() - 1 == 3)
				std::cout << "Opponents' combined remaining health: " << fighters[1]->health + fighters[2]->health + fighters[3]->health << '\n';

			else
				std::cout << "Opponents' combined remaining health: " << fighters[1]->health + fighters[2]->health + fighters[3]->health + fighters[4]->health << '\n';
		}
	}
	
	std::cin.get();
	CLEAR;
}
