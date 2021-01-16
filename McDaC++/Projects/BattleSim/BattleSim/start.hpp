#pragma once

#include <array>
#include <string>

struct Details
{
    public:
        int LV = 0;
        int EXP;
        std::string uname;
        bool guest = false;

        void set_pwd(const std::string &pwd);

        void set_guest();

        static int inc_guests();

        void operator = (Details details);

        void operator ++ (int);

    private:
        std::string pwd;
        inline static unsigned long guest_num = 0;
};

int start();

bool cfm_battle(const int LV);

int tutorial();

std::array<int, 3> custom(const int LV, const bool guest);

std::array<std::string, 2> multiplayer();

void credits();

Details sign_in();