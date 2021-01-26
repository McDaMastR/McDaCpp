#pragma once

#include "bspch.hpp"

class Fighter
{
public:
    uint32_t health, damage;
    float taunt_amount = 1, magic_def = 1, magic_atk = 1;

    union F_Union // Fighter Union
    {
        enum class Input : uint8_t 
        {
            null = 0,
            item, magic, taunt, attack
        };

        enum class Use : uint8_t
        {
            null = 0,
            reflector, absorber,
            def_inc, atk_inc, hlth_inc,
            taunt,
            attack_1, attack_2, attack_3, attack_4,
            attack_use
        };
    };

    F_Union::Input main_input = F_Union::Input::null;
    F_Union::Use use = F_Union::Use::null;

    Fighter(const uint16_t LV);

    ~Fighter();

    void health_bar() const;
    
    virtual void main_action() = 0;

    void use_reflector(std::vector<std::string> &text, uint32_t opp_attack, uint32_t &opp_health, const uint8_t opp_no = 0);

    void use_absorber(std::vector<std::string> &text, uint32_t opp_attack, const uint8_t opp_no = 0);

    void inc_def(std::vector<std::string> &text, const uint8_t opp_no = 0);

    void inc_atk(std::vector<std::string> &text, const uint8_t opp_no = 0);

    void inc_hlth(std::vector<std::string> &text, const uint8_t opp_no = 0);

    void taunt(std::vector<std::string> &text, const uint8_t opp_no = 0);
    
    virtual void attack(std::vector<std::string> &text, const float opp_def, const float opp_taunt, const uint8_t opp_no = 0) = 0;
    
protected:
    const uint32_t max_health;
    uint32_t hlth_inc;
    uint16_t LV;
    uint8_t secondary_input, item_random;
    uint8_t reflector_health = 100, absorber_health = 100;
    float def_inc, atk_inc, magic_effect = 1;
    std::random_device rand_gen;
    inline static uint8_t no_inst = 0;

    static void add_one();

    static void take_one();

    static uint8_t get_inst();
    
private:
    uint8_t number;
};

class User : public Fighter
{
public:
    const std::string name;

    User(const std::string &name, const uint16_t LV);

    ~User() = default;

    void main_action() override;

    void item_action();

    void magic_action();

    void taunt_action();

    void attack_action();

    void attack(std::vector<std::string> &text, const float opp_def, const float opp_taunt, const uint8_t opp_no = 0) override;

private:
    std::__1::chrono::steady_clock::time_point then, now;
    std::chrono::duration<double> passed;
    bool loop = false;
};

class Computer : public Fighter
{
public:
    uint8_t number;

    Computer(const uint8_t number, const uint16_t LV);

    ~Computer() = default;

    void main_action() override;

    void attack(std::vector<std::string> &text, const float opp_defence, const float opp_taunt, const uint8_t opp_no = 0) override;
};
