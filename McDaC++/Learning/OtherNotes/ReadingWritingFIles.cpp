#include <iostream>
#include <string>
#include <vector>
#include <fstream> // Lets you read and write to files

int main()
{
    std::string input;
    std::vector<std::string> read;
    std::string file_input;

    std::cout << "What file should be opened?: ";
    std::cin >> input;

    std::ofstream file (input, std::ios::app); // Assigns the variable "file" to the file "Text.txt" (also opens Text.txt)
    // the std::ios::app causes the variable to append to Text.txt instead of overwriting it

    if(!(file.is_open())) // file.is_open() checks if file has been successfully opened
    {
        std::cout << "File is unable to be opened." << std::endl;
        return -1;
    }

    file << "Hello there."; // Writes the text "Hello there." to file (Text.txt)

    file.close(); // Closes the file. This statement is usually unnesesarry because c++ automatically closes the file


    std::cout << "What file should be read?: ";
    std::cin >> input;

    std::ifstream file2 (input); // The ifstream instead of ofstream indecates that the file will be read not written to

    while(file2 >> file_input) // Lets you read through the file line by line until it is all read
    {
        read.push_back(file_input);
    }

    for(std::string read_line : read) // Outputs every line in the file
    {
        std::cout << read_line << std::endl;
    }

    char get_text = file2.get(); // Get method gets the first character/interger from the file (not string)
    std::cout << get_text << std::endl;

    std::string line;
    getline(file2, line); // Gets the first line of the file and stores it in variable "line"
    std::cout << line << std::endl;

    return 0;
}