#include <iostream>
#include <string>
#include <vector>
#include "CreateUser.h"

int main()
{
    std::vector<CreateUser> users;

    CreateUser McDaMastR("Seth", "McDonald");
    std::cout << McDaMastR.first_name << std::endl;

    CreateUser user2;

    std::cout << CreateUser::get_count() << std::endl;
    
    output_id(user2);

    return 0;
}