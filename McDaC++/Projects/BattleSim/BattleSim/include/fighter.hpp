#pragma once

#include "bspch.hpp"

class [[nodiscard]] Fighter
{
public:
    uint32_t health, damage;
    float taunt_amount = 1.0f, magic_def = 1.0f, magic_atk = 1.0f;

    enum class [[nodiscard]] Input : uint16_t 
    {
        null = 0,
        item, magic, taunt, attack
    };

    enum class [[nodiscard]] Use : uint16_t
    {
        null = 0,
        reflector, absorber,
        def_inc, atk_inc, hlth_inc,
        taunt,
        attack_1, attack_2, attack_3, attack_4,
        attack_use
    };

    Input main_input = Input::null;
    Use use = Use::null;

    [[nodiscard]] explicit Fighter(const uint16_t LV) noexcept;

    virtual ~Fighter() noexcept;

    void healthBar() const noexcept;
    
    virtual void mainAction() = 0;

    void useReflector(std::vector<std::string> &text, uint32_t opp_attack, uint32_t &opp_health, const uint16_t opp_no = 0);

    void useAbsorber(std::vector<std::string> &text, uint32_t opp_attack, const uint16_t opp_no = 0);

    void incDef(std::vector<std::string> &text);

    void incAtk(std::vector<std::string> &text);

    void incHlth(std::vector<std::string> &text);

    void taunt(std::vector<std::string> &text);
    
    virtual void attack(std::vector<std::string> &text, const float opp_def, const float opp_taunt, const uint16_t opp_no = 0) = 0;
    
protected:
    const uint32_t max_health;
    uint32_t hlth_inc;
    uint16_t LV;
    uint16_t secondary_input, item_random;
    uint16_t reflector_health = 100, absorber_health = 100;
    uint16_t number = 0;
    float def_inc, atk_inc, taunt_inc, magic_inc, magic_effect = 1.0f;
    std::random_device rand_gen;
    constinit inline static uint16_t no_inst = 0;

    static void addOne() noexcept;

    static void takeOne() noexcept;

    [[nodiscard]] static uint16_t getInst() noexcept;
};

class [[nodiscard]] User : public Fighter
{
public:
    const std::string_view name;

    [[nodiscard]] User(const std::string_view &name, const uint16_t LV) noexcept;

    ~User() override = default;

    void mainAction() override;

    void itemAction();

    void magicAction();

    void tauntAction();

    void attackAction();

    void attack(std::vector<std::string> &text, const float opp_def, const float opp_taunt, const uint16_t opp_no = 0) override;

private:
    std::__1::chrono::steady_clock::time_point then, now;
    std::chrono::duration<double> passed;
};

class [[nodiscard]] Computer : public Fighter
{
public:
    [[nodiscard]] Computer(const uint16_t &&number, const uint16_t &&LV) noexcept;

    ~Computer() override = default;

    void mainAction() override;

    void attack(std::vector<std::string> &text, const float opp_defence, const float opp_taunt, const uint16_t opp_no = 0) override;
};
