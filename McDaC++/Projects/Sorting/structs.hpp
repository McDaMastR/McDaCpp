#include <string_view>
#include <vector>
#include <array>
#include <utility>

struct Entity
{
private:
    inline static uint32_t no_inst = 0;

    enum class Sex
    {
        male = 0, female
    };

public:
    const std::pair<std::string_view, std::string_view> name;
    const uint16_t age;
    const Sex sex;

    Entity(const std::pair<std::string_view, std::string_view> &name, const uint16_t age, const bool sex);
    Entity(const std::pair<std::string_view, std::string_view> &name, const uint16_t &&age, const bool sex);
    Entity(const std::pair<std::string_view, std::string_view> &name, const uint16_t age, const bool &&sex);
    Entity(const std::pair<std::string_view, std::string_view> &name, const uint16_t &&age, const bool &&sex);
};

struct Student : public Entity
{
	const uint16_t year_level;
};

struct Teacher : public Entity
{
private:
    enum class Specialty
    {
        math = 0, physics, chemistry, bioligy,
        english, history, geography,
        sport, health,
        com_science, graphics, robotics,
        woodwork, struct_design,
        vis_art, music, 
        other_languages
    };

public:
    const std::vector<Specialty> specialties;
};

struct HigherAuthority : public Entity
{
private:
    enum class Authority
    {
        principal = 0, vice_princeipal,
        HOD, other
    };
public:
     
};

struct YearLevel
{
private:
    enum class Activity
    {
        activity1 = 0, activity2, activity3, 
        activity4, activity5, activity6,
        activity7, activity8, activity9
    };

public:
    std::vector<Teacher> teachers;
    std::vector<Student> students;
    uint16_t classroom;
    const uint8_t class_name;

    std::array<Activity, 9> activities;

    static std::array<std::array<Activity, 9>, /* no of classes */ 10> activity_board;
    static uint16_t current;
};
