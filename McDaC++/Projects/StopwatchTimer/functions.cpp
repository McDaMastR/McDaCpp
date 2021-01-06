#include <iostream>
#include <string>
#include <unistd.h>
#include "functions.h"

void clear()
{
    system("clear");
}

int start()
{
    int input;
    std::cout << "Enter 1 for stopwatch\nEnter 2 for timer\nEnter 3 to quit:";
    std::cin >> input;
    return input;
}

void start_stopwatch()
{
    int secs = 0;
    int mins = 0;
    int hours = 0;

    std::cout << "Enter to start:";
    std::cin.get();
    std::cout << std::endl;

    while(true)
    {
        clear();

        std::cout << hours << ":" << mins << ":" << secs << std::endl;

        sleep(1);

        if(secs != 59)
        {
            secs++;
        }
        else
        {
            secs = 0;

            if(mins != 59)
            {
                mins++;
            }
            else
            {
                mins = 0;

                if(hours != 23)
                {
                    hours++;
                }
                else;
                {
                    break;
                }
            }
        }
    }
}

void start_timer()
{
    int mins;
    int secs;

    std::cout << "Length of timer (mins): ";
    std::cin >> mins;
    std::cout << "Length of timer (secs): ";
    std::cin >> secs;
    std::cout << "Enter to start: ";
    std::cin.get();

    while(true)
    {
        clear();

        std::cout << mins << ":" << secs << std::endl;

        sleep(1);

        if(secs != 0)
        {
            secs--;
        }
        else
        {
            secs = 59;

            if(mins != 0)
            {
                mins--;
            }
            else
            {
                std::cout << "Finished!" << std::endl;
                break;
            }
        }
    }
}