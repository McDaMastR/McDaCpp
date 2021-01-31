#pragma once

#include "bspch.hpp"

struct Details
{
public:
    uint16_t EXP, LV = 0;
    std::string uname;
    bool guest = false;

    Details(const Details &) = delete;

    static Details &get();

    static void setPwd (const std::string &pwd);

    static void setGuest();

    void operator = (const Details &) = delete;

    void operator ++ (int);

private:
    std::string pwd;
    inline static uint8_t guest_num = 0;

    Details() = default;
    ~Details() = default;

    void privateSetPwd(const std::string &pwd);

    void privateSetGuest();

    static uint8_t incGuests();
};

void start(uint8_t &input);

void cfmBattle(uint16_t &no_opp);

void tutorial();

void credits();

void signIn();

void signOut();
