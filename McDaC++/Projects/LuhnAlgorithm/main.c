#include "application.h"

// To compile:
// gcc -std=gnu2x -O3 -Wall -Wextra -Wpedantic application.c main.c -o LuhnAlgorithm

int main()
{
	Application_t app = createApplication();
	runApplication(&app);
}
