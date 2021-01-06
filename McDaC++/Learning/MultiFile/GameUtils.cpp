#include <iostream>
#include <fstream>
#include <vector>
#include "GameUtils.h"

int menu()
{
    int input;
    do
    {
        std::cout << "Enter 1 to play guessing game\nEnter 2 to play <game>\nEnter 3 to quit: ";
        std::cin >> input;
    } 
    while(!(input == 1 || input == 2 || input == 3));

    return input;
}

int read_file()
{
    int high_score;

    std::ifstream high_score_file ("HighScore.txt");
    high_score_file >> high_score;

    if(!high_score_file.is_open())
    {
        high_score = 0;
    }

    return high_score;
}

int random_number()
{
    int upper_bound;
    int lower_bound;

    std::cout << "Enter the upper bound of the random number: ";
    std::cin >> upper_bound;
    std::cout << "Enter the lower bound of the random number: ";
    std::cin >> lower_bound;

    int ran_num = (rand() % upper_bound - lower_bound) + lower_bound;
    return ran_num;
}

void guess_number(const int ran_num, std::vector<int> &guesses)
{
    int guess;
    bool tried = false;
    do
    {
        if(tried)
        {
            guesses.push_back(guess);

            if(guess > ran_num)
            {
                std::cout << "Lower!: ";
                std::cin >> guess;
            }
            else
            {
                std::cout << "Higher!: ";
                std::cin >> guess;
            }
        }
        else
        {
            std::cout << std::endl;
            std::cout << "Guess the number!: ";
            std::cin >> guess;
            tried = true;
        }
    }
    while(guess != ran_num);
}

void guessed(const int tries, const int high_score)
{
    std::cout << "You guessed the number in " << tries << " tries!" << std::endl;

    if(high_score == 0)
    {
        std::cout << "High score is currently unavalible" << std::endl;
    }
    else if(tries < high_score)
    {
        std::ofstream high_score_file ("HighScore.txt");
        high_score_file << tries;
        high_score_file.close();

        std::cout << "You got a new high score!" << std::endl;
    }
}

void see_guesses(const std::vector<int> guesses)
{
    int see;

    std::cout << "See all of your guesses? (Press 1 to see them, otherwise press 2): ";
    std::cin >> see;
    
    if(see == 1)
    {
        for(int i = 0; i < guesses.size(); i++)
        {
            if(i == guesses.size() - 1)
            {
                std::cout << guesses[i] << std::endl;
            }
            else
            {
                std::cout << guesses[i] << ", ";
            }
        }
    }
}

void guessing_game()
{
    int high_score;
    int ran_num;
    bool tried = false;
    std::vector<int> guesses;

    high_score = read_file();

    ran_num = random_number();

    guess_number(ran_num, guesses);

    guessed(guesses.size(), high_score);

    see_guesses(guesses);
}

void other_game()
{

}