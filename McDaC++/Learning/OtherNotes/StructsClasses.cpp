#include <iostream>
#include <string>
#include <vector>

struct User // Creates a struct
{ // By default, variables inside of a struct are public, not private
     std::string first_name;
     std::string last_name;
     private:
        int id;
}; // ALWAYS PUT SEMICOLON AFTER STRUCT/CLASS!!!

class CreateUser // By default, variables inside of a class are private, not public
{
    int id = 001;
    static int user_count; // Creates a static variable (can only be used in static methods)

    public:
        std::string first_name;
        std::string last_name;

        static int get_count() // Creates a static method
        {
            return user_count;
        }

        CreateUser(std::string first_name, std::string last_name) // Creates a custom constructor for the class
        {
            user_count++;

            this->first_name = first_name; // The "this->" specifies that the variable is from the class not input
            this->last_name = last_name;
        }

        CreateUser()
        {
            user_count++;
        }

        ~CreateUser() // Creates a custom destructor
        {
            user_count--;
        }

        void set_id(int id) // Creates a setter
        {
            this->id = id;
        }

        int get_id()
        {
            return id;
        }

        friend void output_id(CreateUser user); // Initializes a friend method
};

int CreateUser::user_count = 0; // Sets static variable inside of class CreateUser

void output_id(CreateUser user) /* Have to define friend method outside of class 
friend methods/functions are stand alone functions, so they are not part of the class, even when initialized in it */
{
    std::cout << user.id; // Friend method can access private information even when initialized with public access
}

int add_user(std::vector<CreateUser> &users, CreateUser user)
{
    for(int i = 0; i < users.size(); i++)
    {
        if(users[i].first_name == user.first_name && users[i].last_name == user.last_name) // Checks if user already exists
        {
            return i;
        }
    }
    users.push_back(user);
    return users.size() - 1;
}

int main()
{
    std::vector<CreateUser> users;

    CreateUser McDaMastR("Seth", "McDonald"); // Creates an object "McDaMastR"
    // Cannot assign a value to McDaMastR.id because it is private not public
    std::cout << McDaMastR.first_name << std::endl;

    CreateUser user2;

    std::cout << CreateUser::get_count() << std::endl; // returns the amount of users
    
    output_id(user2);

    return 0;
}