#include <iostream>
#include <string>

int main()
{
    std::string word;
    std::string name = "Seth"; // defines the variable as a string object
    std::cout << name[0] << std::endl; // returns the first character of name
    std::string full_name = name + " McDonald";
    std::cout << full_name << std::endl;
    full_name += "!"; // Appends this character the the end of full_name
    std::cout << full_name << std::endl;
    getline(std::cin, word); // Gets entire input for user instead of just the first word
    std::cout << word << std::endl;

    std::cout << full_name.length() << std::endl << std::endl; // Says the length of characters in full_name
    name.append("!"); // Appends ! to the end of name
    std::cout << name << std::endl;
    name.insert(3, "         "); // Inserts the second argument into the string in the 3rd character's place
    std::cout << name << std::endl;
    name.erase(3, 9); // Erases 9 characters starting from the 3rd character
    std::cout << name << std::endl;
    name.pop_back(); // Remover the last character from the string
    std::cout << name << std::endl;
    name.replace(0, 4, "McDaMastR"); // Replaces 4 characters from the 0th character to "McDaMastR"
    std::cout << name << std::endl;
    full_name.find("McDonald"); // returns the index of the first character of "McDonald"
    full_name.substr(0, 4); // returns 4 characters from index 0
    full_name.find_first_of("aeiou"); // returns the index of the first vowel
    if (full_name == "Seth McDonald!") std::cout << "Seth";

    return 0;
}
