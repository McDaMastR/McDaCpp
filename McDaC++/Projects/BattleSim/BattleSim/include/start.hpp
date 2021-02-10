#pragma once

#include "bspch.hpp"

struct [[nodiscard("Discarding return value of type Details")]] Details
{
public:
    uint16_t EXP, LV = 0;
    std::string uname;
    bool guest = false;

    Details(const Details &) = delete;

    [[nodiscard("Discarding reference to Details singleton")]] static Details &get();

    static void setPwd (const std::string &pwd);

    static void setGuest();

    void operator = (const Details &) = delete;

    void operator ++ (int);

private:
    std::string pwd;
    constinit inline static uint16_t guest_num = 0;

    [[nodiscard("Discarding creation of object of type Details")]] Details() = default;
    ~Details() = default;

    void privateSetPwd(const std::string &pwd);

    void privateSetGuest();

    static uint16_t incGuests();
};

uint16_t start();

uint16_t cfmBattle();

void tutorial();

void credits();

void signIn();

void signOut();
