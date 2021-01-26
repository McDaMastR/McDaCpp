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

    static void set_pwd(const std::string &pwd);

    static void set_guest();

    void operator = (const Details &) = delete;

    void operator ++ (int);

private:
    std::string pwd;
    inline static uint8_t guest_num = 0;

    Details() = default;
    ~Details() = default;

    void private_set_pwd(const std::string &pwd);

    void private_set_guest();

    static uint8_t inc_guests();
};

void start(uint8_t &input);

std::optional<uint8_t> cfm_battle();

void tutorial();

void credits();

void sign_in();
