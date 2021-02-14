#include "../include/fighter.hpp"

#include <cmath>

std::istream &operator >> (std::istream &input, Fighter::Input &value)
{
    uint16_t temp;
    input >> temp;
    value = static_cast<Fighter::Input>(temp);
    return input;
}

Fighter::Fighter(const uint16_t LV) noexcept
    : health(40 * LV), max_health(40 * LV), LV(LV) {addOne();}

Fighter::~Fighter() noexcept {takeOne();}

void Fighter::addOne() noexcept {no_inst++;}

void Fighter::takeOne() noexcept {no_inst--;}

uint16_t Fighter::getInst() noexcept {return no_inst;}

void Fighter::healthBar() const noexcept {std::cout << health << '/' << max_health << '\n';}

void Fighter::useReflector(std::vector<std::string> &text, uint32_t opp_attack, uint32_t &opp_health, const uint16_t opp_no)
{
    std::uniform_int_distribution<uint16_t> distribution(1, 100);
    item_random = distribution(rand_gen);

    if(!opp_attack)
    {
        if(!opp_no)
            text.emplace_back("You missed!");

        else
            text.emplace_back("Opponent " + std::to_string(opp_no) + "'s attack missed!");
    }
    else if(reflector_health < 0)
    {
        text.reserve(2);
        
        if(opp_attack > health) 
            opp_attack = health;
             
        health -= opp_attack;
            
        if(!opp_no) 
        {
            text.emplace_back("But opponent " + std::to_string(number) + "'s reflector had already been destroyed!");
            text.emplace_back("You inflicted " + std::to_string(opp_attack) + " damage");
        }
        else 
        {
            text.emplace_back("But your reflector had already been destroyed!");
            text.emplace_back("Opponent " + std::to_string(opp_no) + " inflicted " + std::to_string(opp_attack) + " damage");
        }
    }
    else if(!reflector_health)
    {
        text.reserve(2);
        
        opp_attack = static_cast<uint32_t>(opp_attack * 1.6);
            
        if(opp_attack > health) 
            opp_attack = health;

        health -= opp_attack;
        reflector_health -= 20;

        if(!opp_no) 
        {
            text.emplace_back("You destroyed opponent " + std::to_string(number) + "'s reflector!");
            text.emplace_back("You inflicted " + std::to_string(opp_attack) + " damage");
        }
        else 
        {
            text.emplace_back("Opponent " + std::to_string(opp_no) + "'s attack destroyed your reflector!");
            text.emplace_back("Opponent " + std::to_string(opp_no) + " inflicted " + std::to_string(opp_attack) + " damage");
        }
    }
    else if(item_random > reflector_health)
    {
        text.reserve(2);
        
		opp_attack = static_cast<uint32_t>(opp_attack * 1.3);

        if(opp_attack > health) 
            opp_attack = health;

        health -= opp_attack;

        if(!opp_no) 
        {
            text.emplace_back("You broke through opponent " + std::to_string(number) + "'s reflector!");
            text.emplace_back("You inflicted " + std::to_string(opp_attack) + " damage");
        }
        else 
        {
            text.emplace_back("Opponent " + std::to_string(opp_no) + "'s attack broke through your reflector!");
            text.emplace_back("Opponent " + std::to_string(opp_no) + " inflicted " + std::to_string(opp_attack) + " damage");
        }
    }
    else
    {
        text.reserve(2);
        
        if(opp_attack > opp_health) 
            opp_attack = opp_health;

        opp_health -= opp_attack;

        reflector_health -= 20;

        if(!opp_no) 
        {
            text.emplace_back("Your attack was reflected!");
            text.emplace_back("Your reflection inflicted " + std::to_string(opp_attack) + " damage");
        }
        else 
        {
            text.emplace_back("Opponent " + std::to_string(opp_no) + "'s attack was reflected!");
            text.emplace_back("Opponent " + std::to_string(opp_no) + "'s reflection inflicted " + std::to_string(opp_attack) + " damage");
        }
    }
}

void Fighter::useAbsorber(std::vector<std::string> &text, uint32_t opp_attack, const uint16_t opp_no)
{
    std::uniform_int_distribution<uint16_t> distribution(1, 100);
    item_random = distribution(rand_gen);

    if(!opp_attack)
    {
        if(!opp_no)
            text.emplace_back("You missed!");

        else
            text.emplace_back("Opponent " + std::to_string(opp_no) + "'s attack missed!");
    }
    else if(absorber_health < 0)
    {
        text.reserve(2);
        
        if(opp_attack > health) 
            opp_attack = health;
             
        health -= opp_attack;
            
        if(!opp_no) 
        {
            text.emplace_back("But opponent " + std::to_string(number) + "'s absorber had already been destroyed!");
            text.emplace_back("You inflicted " + std::to_string(opp_attack) + " damage");
        }
        else 
        {
            text.emplace_back("But your absorber had already been destroyed!");
            text.emplace_back("Opponent " + std::to_string(opp_no) + " inflicted " + std::to_string(opp_attack) + " damage");
        }
    }
    else if(!absorber_health)
    {
        text.reserve(2);
        
		opp_attack = static_cast<uint32_t>(opp_attack * 1.6);
            
        if(opp_attack > health) 
            opp_attack = health;

        health -= opp_attack;
        absorber_health -= 20;

        if(!opp_no) 
        {
            text.emplace_back("You destroyed opponent " + std::to_string(number) + "'s absorber!");
            text.emplace_back("You inflicted " + std::to_string(opp_attack) + " damage");
        }
        else 
        {
            text.emplace_back("Opponent " + std::to_string(opp_no) + "'s attack destroyed your absorber!");
            text.emplace_back("Opponent " + std::to_string(opp_no) + " inflicted " + std::to_string(opp_attack) + " damage");
        }
    }
    else if(item_random > absorber_health)
    {
        text.reserve(2);
        
		opp_attack = static_cast<uint32_t>(opp_attack * 1.3);

        if(opp_attack > health) 
            opp_attack = health;

        health -= opp_attack;

        if(!opp_no) 
        {
            text.emplace_back("You broke through opponent " + std::to_string(number) + "'s absorber!");
            text.emplace_back("You inflicted " + std::to_string(opp_attack) + " damage");
        }
        else 
        {
            text.emplace_back("Opponent " + std::to_string(opp_no) + "'s attack broke through your absorber!");
            text.emplace_back("Opponent " + std::to_string(opp_no) + " inflicted " + std::to_string(opp_attack) + " damage");
        }
    }
    else
    {
        text.reserve(2);
        
        magic_inc = std::ceilf(opp_attack / (10 * LV)) / 100 + 1;

        magic_effect *= magic_inc;
        absorber_health -= 20;

        if(!opp_no) 
        {
            text.emplace_back("Your attack was absorbed!");
            text.emplace_back("Opponent " + std::to_string(number) + "'s magic effectiveness increased by " + std::to_string(static_cast<uint16_t>(std::ceilf((magic_inc - 1) * 100))) + "%");
        }
        else 
        {
            text.emplace_back("Opponent " + std::to_string(opp_no) + "'s attack was absorbed!");
            text.emplace_back("Your magic effectiveness increased by " + std::to_string(static_cast<uint16_t>(std::ceilf((magic_inc - 1) * 100))) + "%");
        }
    }
}

void Fighter::incDef(std::vector<std::string> &text)
{
    text.reserve(2);

    std::uniform_real_distribution<float> distribution(0.5f, 1.0f);
    def_inc = distribution(rand_gen) / magic_effect;
    magic_def *= def_inc;

    if(number) 
    {
        text.emplace_back("Opponent " + std::to_string(number) + " used magic on their defence");
        text.emplace_back("Opponent " + std::to_string(number) + "'s defence increased by " + std::to_string((int) (100 - def_inc * 100)) + "%!");
    }
    else
    {
        text.emplace_back("You used magic on your defence");
        text.emplace_back("Your defence increased by " + std::to_string((int) (100 - def_inc * 100)) + "%!");
    }
}

void Fighter::incAtk(std::vector<std::string> &text)
{
    text.reserve(2);

    std::uniform_real_distribution<float> distribution(1.1f, 1.6f);
    atk_inc = distribution(rand_gen) * magic_effect;
    magic_atk *= atk_inc;

    if(number) 
    {
        text.emplace_back("Opponent " + std::to_string(number) + " used magic on their attack");
        text.emplace_back("Opponent " + std::to_string(number) + "'s attack increased by " + std::to_string((int) (atk_inc * 100 - 100)) + "%!");
    }
    else 
    {
        text.emplace_back("You used magic on your attack");
        text.emplace_back("Your attack increased by " + std::to_string((int) (atk_inc * 100 - 100)) + "%!");
    }
}

void Fighter::incHlth(std::vector<std::string> &text)
{
    text.reserve(2);

    std::uniform_int_distribution<uint16_t> distribution(10, 40);
    hlth_inc = static_cast<uint32_t>(std::ceilf(distribution(rand_gen) * magic_effect * LV / 5));
    health += hlth_inc;

    if(health > max_health)
    {
        hlth_inc -= health - max_health;
        health = max_health;
    }
    if(number) 
    {
        text.emplace_back("Opponent " + std::to_string(number) + " used magic on their health");
        text.emplace_back("Opponent " + std::to_string(number) + "'s health increased by " + std::to_string(hlth_inc) + "!");
    }
    else 
    {
        text.emplace_back("You used magic on your health");
        text.emplace_back("Your health increased by " + std::to_string(hlth_inc) + "!");
    }
}

void Fighter::taunt(std::vector<std::string> &text)
{
    text.reserve(4);

    std::uniform_real_distribution<float> distribution(0.25f, 0.7f);
	taunt_inc = distribution(rand_gen);
    taunt_amount += taunt_inc;

    if(number) 
    {
        text.emplace_back("Opponent " + std::to_string(number) + " taunted you");
        text.emplace_back("You got frustrated...");
        text.emplace_back("Your attack increased by " + std::to_string(taunt_inc * 100) + "%!");
        text.emplace_back("Your defence decreased by " + std::to_string(taunt_inc * 100) + "%!");
    }
    else if(getInst() - 1 == 1)
    {
        text.emplace_back("You taunted the opponent");
        text.emplace_back("The opponent got irritated...");
        text.emplace_back("The opponent's attack increased by " + std::to_string(taunt_inc * 100) + "%!");
        text.emplace_back("The opponent's defence decreased by " + std::to_string(taunt_inc * 100) + "%!");
    }
    else
    {
        text.emplace_back("You taunted the opponents");
        text.emplace_back("The opponents got frustrated...");
        text.emplace_back("The opponents' attack increased by " + std::to_string(taunt_inc * 100) + "%!");
        text.emplace_back("The opponents' defence decreased by " + std::to_string(taunt_inc * 100) + "%!");
    }
}

User::User(const std::string_view &name, const uint16_t LV) noexcept
    : Fighter(LV), name(name) {}

void User::mainAction()
{
    std::cout << "| Items | Magic | Taunt | Attack | ";
    std::cin >> main_input;

    switch(main_input)
    {
        case Input::item:
            itemAction();
            break;
                    
        case Input::magic:
            magicAction();
            break;
				
        case Input::taunt:
            tauntAction();
            break;

        case Input::attack:
            attackAction();
            [[fallthrough]];
    	default:
    		break;
    }
    CLEAR;
}

void User::itemAction()
{
    while(true)
    {
        std::cout << "| Reflector | Absorber | Back | ";
        std::cin >> secondary_input;

        switch(secondary_input)
        {
            case 1:
                use = Use::reflector;
                return;

            case 2:
                use = Use::absorber;
                [[fallthrough]];
            case 3:
                return;
        }
    }
}

void User::magicAction()
{
	while(true)
    {
        std::cout << "| Defence | Attack | Health | Back | ";
        std::cin >> secondary_input;

        switch(secondary_input)
        {
			case 1:
                use = Use::def_inc;
                return;

            case 2:
                use = Use::atk_inc;
                return;

            case 3:
                use = Use::hlth_inc;
                [[fallthrough]];
            case 4:
                return;
        }
    }
}

void User::tauntAction()
{
	while(true)
    {
        std::cout << "| All | Back | ";
        std::cin >> secondary_input;

        switch(secondary_input)
        {
            case 1:
                use = Use::taunt;
                [[fallthrough]];
            case 2:
                return;
        }
    }
}

void User::attackAction()
{
    while(true)
    {
        std::cout << "| Opp1 | Back | ";
        std::cin >> secondary_input;

        switch(secondary_input)
        {
            case 1:
                use = Use::attack_1;
                [[fallthrough]];
            case 2:
                return;
        }
    }
}

void User::attack(std::vector<std::string> &text, const float opp_def, const float opp_taunt, const uint16_t opp_no)
{
    damage = static_cast<uint32_t>(std::ceilf(magic_atk * taunt_amount * opp_def * opp_taunt * LV / 5));
    std::uniform_int_distribution<uint16_t> distribution(2, 4);

    std::cout << "Wait...\n";
    sleep(distribution(rand_gen));
    std::cout << "NOW!: ";
	then = std::chrono::steady_clock::now();
    std::cin.get();
    std::cin.get();
    CLEAR;
    now = std::chrono::steady_clock::now();
    passed = now - then;

    if          (passed.count() <= 0.05)	damage  = 0;
    else if     (passed.count() <= 0.1)     damage *= 60;
    else if     (passed.count() <= 0.15)    damage *= 58;
    else if     (passed.count() <= 0.2)     damage *= 56;
    else if     (passed.count() <= 0.25)    damage *= 54;
    else if     (passed.count() <= 0.3)     damage *= 52;
    else if     (passed.count() <= 0.35)    damage *= 50;
    else if     (passed.count() <= 0.4)     damage *= 48;
    else if     (passed.count() <= 0.45)    damage *= 46;
    else if     (passed.count() <= 0.5)     damage *= 44;
    else if     (passed.count() <= 0.55)    damage *= 42;
    else if     (passed.count() <= 0.6)     damage *= 40;
    else if     (passed.count() <= 0.65)    damage *= 38;
    else if     (passed.count() <= 0.7)     damage *= 36;
    else if     (passed.count() <= 0.75)    damage *= 34;
    else if     (passed.count() <= 0.8)     damage *= 32;
    else if     (passed.count() <= 0.85)    damage *= 30;
    else if     (passed.count() <= 0.9)     damage *= 28;
    else if     (passed.count() <= 0.95)    damage *= 26;
    else if     (passed.count() <= 1)       damage *= 24;
    else if     (passed.count() <= 1.1)     damage *= 22;
    else if     (passed.count() <= 1.2)     damage *= 20;
    else if     (passed.count() <= 1.3)     damage *= 18;
    else if     (passed.count() <= 1.4)     damage *= 16;
    else if     (passed.count() <= 1.5)     damage *= 14;
    else if     (passed.count() <= 1.6)     damage *= 12;
    else if     (passed.count() <= 1.7)     damage *= 10;
    else if     (passed.count() <= 1.8)     damage *= 8;
    else if     (passed.count() <= 1.9)     damage *= 6;
    else if     (passed.count() <= 2)       damage *= 4;
    else if     (passed.count() <= 2.5)     damage *= 2;
    else                                    damage  = 0;

    text.emplace_back("You attacked opponent " + std::to_string(opp_no));
}

Computer::Computer(const uint16_t &&number, const uint16_t &&LV) noexcept
    : Fighter(LV) {this->number = number;}

void Computer::mainAction()
{
    uint16_t temp;
    std::uniform_int_distribution<uint16_t> distribution(1, 100);
    temp = distribution(rand_gen);

    if      (temp < 40)     main_input = Input::attack;
    else if (temp < 70)     main_input = Input::magic;
    else if (temp < 90)     main_input = Input::item;
    else                    main_input = Input::taunt;


    if(main_input == Input::attack)
        use = Use::attack_use;
        
    else if(main_input == Input::magic)
    {
        std::uniform_int_distribution<uint16_t> sec_distribution(1, 90);
        secondary_input = sec_distribution(rand_gen);

        if(secondary_input < 31) 
            use = Use::def_inc;

        else if(secondary_input < 61) 
            use = Use::atk_inc;

        else 
            use = Use::hlth_inc;
    }
    else if(main_input == Input::item)
    {
        secondary_input = distribution(rand_gen);

        if(secondary_input < 51) 
            use = Use::reflector;

        else 
            use = Use::absorber;
    }
    
    else
        use = Use::taunt;
}

void Computer::attack(std::vector<std::string> &text, const float opp_def, const float opp_taunt, const uint16_t)
{
    std::uniform_int_distribution<uint16_t> distribution(5, 35);
    damage = static_cast<uint32_t>(distribution(rand_gen) * magic_atk * taunt_amount * opp_taunt * opp_def * LV / 5);

    if(damage < LV) 
        damage += LV;

    text.emplace_back("Opponent " + std::to_string(number) + " attacked you!");
}
