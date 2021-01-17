#pragma once

#include <array>
#include <string>

struct Details
{
public:
    uint16_t LV = 0;
    uint16_t EXP;
    std::string uname;
    bool guest = false;

    void set_pwd(const std::string &pwd);

    void set_guest();

    static uint8_t inc_guests();

    void operator = (Details details);

    void operator ++ (int);

private:
    std::string pwd;
    inline static uint8_t guest_num = 0;
};

uint8_t start();

bool cfm_battle(const uint16_t LV);

void tutorial();

std::array<uint16_t, 3> custom(const uint16_t LV, const bool guest);

std::array<std::string, 2> multiplayer();

void credits();

Details sign_in();
