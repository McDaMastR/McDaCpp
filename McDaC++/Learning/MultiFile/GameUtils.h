#include <vector>

int menu();

int read_file();

int random_number();

void guess_number(const int ran_num, std::vector<int> &guesses);

void guessed(const int tries, const int high_score);

void see_guesses(const std::vector<int> guesses);

void guessing_game();

void other_game();