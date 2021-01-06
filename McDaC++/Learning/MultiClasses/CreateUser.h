#include <string>
#ifndef USER_H
#define USER_H

class CreateUser
{
    int id = 001;
    static int user_count;

    public:
        std::string first_name;
        std::string last_name;

        static int get_count();

        CreateUser(std::string first_name, std::string last_name);

        CreateUser();

        ~CreateUser();

        void set_id(int id);

        int get_id();

        friend void output_id(CreateUser user);
};
#endif