#pragma once

#include <string>
#include <array>
#include "start.hpp"
#include "fighter.hpp"

void battle_init(const std::string &name, const int no_opp, Details &details);

void battle_loop(std::vector<std::string> &text, const int no_opp, Details &details, User &user, Computer &opp1);

void action_priority(std::vector<std::string> &text, const int no_opp, User &user, Computer &opp1);

void multi_battle(const std::array<std::string, 2> &names);

void custom_battle(const std::array<int, 3> values);