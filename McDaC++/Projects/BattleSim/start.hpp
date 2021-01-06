#ifndef START_HPP
#define START_HPP

#include <array>
#include <string>

struct Details
{
    int LV = 0;
    int EXP;
    std::string uname;
    bool guest = false;

    void set_pwd(const std::string pwd);

    void set_guest();

    static int inc_guests();

    void operator = (Details details);

    private:
        std::string pwd;
        static unsigned long guest_num;
};
unsigned long Details::guest_num = 0;

int start();

void cfm_battle(const int LV);

int tutorial();

std::array<int, 3> custom(const int LV, const bool guest);

std::array<std::string, 2> multiplayer();

void credits();

Details sign_in();

#endif // START_HPP