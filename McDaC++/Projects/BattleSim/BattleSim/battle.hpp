#pragma once

#include <string>
#include <array>
#include <vector>
#include "start.hpp"
#include "fighter.hpp"

void battle_init(const std::string &name, const uint8_t no_opp, Details &details);

void battle_loop(std::vector<std::string> &text, Details &details, std::vector<Fighter *> &fighters);

void action_priority(std::vector<std::string> &text, std::vector<Fighter *> &fighters);

void multi_battle(const std::array<std::string, 2> &names);

void custom_battle(const std::array<uint16_t, 3> &values);
