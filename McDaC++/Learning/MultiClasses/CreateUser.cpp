#include <iostream>
#include <string>
#include <vector>
#include "CreateUser.h"

        int CreateUser::get_count()
        {
            return user_count;
        }

        CreateUser::CreateUser(std::string first_name, std::string last_name)
        {
            user_count++;

            this->first_name = first_name;
            this->last_name = last_name;
        }

        CreateUser::CreateUser()
        {
            user_count++;
        }

        CreateUser::~CreateUser()
        {
            user_count--;
        }

        void CreateUser::set_id(int id)
        {
            this->id = id;
        }

        int CreateUser::get_id()
        {
            return id;
        }

        void output_id(CreateUser user);

int CreateUser::user_count = 0;

void output_id(CreateUser user)
{
    std::cout << user.id;
}

int add_user(std::vector<CreateUser> &users, CreateUser user)
{
    for(int i = 0; i < users.size(); i++)
    {
        if(users[i].first_name == user.first_name && users[i].last_name == user.last_name)
        {
            return i;
        }
    }
    users.push_back(user);
    return users.size() - 1;
}