#pragma once

#include <string>
#include <vector>

class Fighter
{
public:
    uint32_t health;
    uint32_t damage;
    uint8_t main_input;
    float taunt_amount = 1;
    float magic_def = 1;
    float magic_atk = 1;
    bool reflector_use = false;
    bool absorber_use = false;
    bool def_inc_use = false;
    bool atk_inc_use = false;
    bool hlth_inc_use = false;
    bool taunt_use = false;

    Fighter(const uint16_t LV);

    ~Fighter();

    void health_bar();
    
    virtual void main_action() = 0;

    void use_reflector(std::vector<std::string> &text, uint32_t opp_attack, uint32_t &opp_health, const uint8_t opp_no = 0);

    void use_absorber(std::vector<std::string> &text, uint32_t opp_attack, const uint8_t opp_no = 0);

    void inc_def(std::vector<std::string> &text, const uint8_t opp_no = 0);

    void inc_atk(std::vector<std::string> &text, const uint8_t opp_no = 0);

    void inc_hlth(std::vector<std::string> &text, const uint8_t opp_no = 0);

    void taunt(std::vector<std::string> &text, const uint8_t opp_no = 0);
    
    virtual void attack(std::vector<std::string> &text, const float opp_def, const float opp_taunt, const uint8_t opp_no = 0) = 0;
    
    virtual const uint8_t get_attack_use() const = 0;
    
protected:
    const uint32_t max_health;
    uint8_t secondary_input;
    uint8_t item_random;
    uint16_t LV;
    uint8_t reflector_health = 100;
    uint8_t absorber_health = 100;
    uint32_t hlth_inc;
    float def_inc;
    float atk_inc;
    float magic_effect = 1;
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

    void main_action();

    void item_action();

    void magic_action();

    void taunt_action();

    void attack_action();

    void attack(std::vector<std::string> &text, const float opp_def, const float opp_taunt, const uint8_t opp_no = 0);
    
    const uint8_t get_attack_use() const;

private:
    uint32_t then;
    uint8_t passed; // TODO fix data type of "passed"
    uint8_t attack_opp_no;
    bool loop = false;
};

class Computer : public Fighter
{
public:
    uint8_t number;
    bool attack_use = false;

    Computer(const uint8_t number, const uint16_t LV);

    void main_action();

    void attack(std::vector<std::string> &text, const float opp_defence, const float opp_taunt, const uint8_t opp_no = 0);
    
    const uint8_t get_attack_use() const;
};
