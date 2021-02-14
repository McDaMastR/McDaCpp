#pragma once

#include "bspch.hpp"

struct [[nodiscard]] Details
{
public:
    uint16_t EXP, LV = 0;
    std::string uname;
    bool guest = false;

    Details(const Details &) = delete;

    [[nodiscard]] static Details &get() noexcept;

    static void setPwd (const std::string_view &pwd) noexcept;

    static void setGuest() noexcept;

    void operator = (const Details &) = delete;

    void operator ++ (int);

private:
    std::string_view pwd;
    constinit inline static uint16_t guest_num = 0;

    [[nodiscard]] Details() = default;
    ~Details() = default;

    void privateSetPwd(const std::string_view &pwd) noexcept;

    void privateSetGuest() noexcept;

    static uint16_t incGuests() noexcept;
};

uint16_t start();

uint16_t cfmBattle();

void tutorial();

void credits();

void signIn();

void signOut();
