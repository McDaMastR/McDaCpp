#ifndef FIGHTER_HPP
#define FIGHTER_HPP

#include <string>
#include <vector>

class Fighter
{
    public:
        unsigned int health;
        unsigned int damage;
        unsigned int main_input;
        double taunt_amount = 1;
        double magic_def = 1;
        double magic_atk = 1;
        bool reflector_use = false;
        bool absorber_use = false;
        bool def_inc_use = false;
        bool atk_inc_use = false;
        bool hlth_inc_use = false;
        bool taunt_use = false;

        Fighter(const int LV);

        ~Fighter();

        void health_bar();

        void use_reflector(std::vector<std::string> &text, unsigned int opp_attack, unsigned int &opp_health, const int opp_no = 0);

        void use_absorber(std::vector<std::string> &text, unsigned int opp_attack, const int opp_no = 0);

        void inc_def(std::vector<std::string> &text, const int opp_no = 0);

        void inc_atk(std::vector<std::string> &text, const int opp_no = 0);

        void inc_hlth(std::vector<std::string> &text, const int opp_no = 0);

        void taunt(std::vector<std::string> &text, const int opp_no = 0);
    
    protected:
        const unsigned int max_health;
        unsigned int secondary_input;
        unsigned int item_random;
        unsigned int LV;
        unsigned int reflector_health = 100;
        unsigned int absorber_health = 100;
        unsigned int hlth_inc;
        double def_inc;
        double atk_inc;
        double magic_effect = 1;
        static unsigned int no_inst;

        static void add_one();

        static void take_one();

        static int get_inst();
    
    private:
        int number;
};
unsigned int Fighter::no_inst = 0;

class User : public Fighter
{
    private:
        unsigned int then;
        double passed;
        bool loop = false;

    public:
        bool attack_1 = false;
        bool attack_2 = false;
        bool attack_3 = false;
        bool attack_4 = false;
        const std::string name;

        User(const std::string name, const int LV);

        void main_action();

        int item_action();

        int magic_action();

        int taunt_action();

        int attack_action();

        void attack(std::vector<std::string> &text, const int opp_no, const double opp_def, const double opp_taunt);
};

class Computer : public Fighter
{
    public:
        unsigned int number;
        bool attack_use = false;

        Computer(const int number, const int LV);

        void main_action();

        void attack(std::vector<std::string> &text, const double opp_defence, const double opp_taunt);
};

#endif // FIGHTER_HPP