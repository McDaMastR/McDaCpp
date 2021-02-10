#include "structs.hpp"

Entity::Entity(const std::pair<std::string_view, std::string_view> &name, const uint16_t age, const bool sex)
	: name(name), age(age), sex(static_cast<Sex>(sex)) {no_inst++;}

Entity::Entity(const std::pair<std::string_view, std::string_view> &name, const uint16_t &&age, const bool sex)
	: name(name), age(age), sex(static_cast<Sex>(sex)) {no_inst++;}

Entity::Entity(const std::pair<std::string_view, std::string_view> &name, const uint16_t age, const bool &&sex)
	: name(name), age(age), sex(static_cast<Sex>(sex)) {no_inst++;}

Entity::Entity(const std::pair<std::string_view, std::string_view> &name, const uint16_t &&age, const bool &&sex)
	: name(name), age(age), sex(static_cast<Sex>(sex)) {no_inst++;}