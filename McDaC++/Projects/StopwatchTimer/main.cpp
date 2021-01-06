#include "functions.h"

int main()
{
    do
    {
        int input = start();

        clear();
    
        switch (input)
        {
        case 1:
            start_stopwatch();
            break;
        
        case 2:
            start_timer();
            break;

        case 3:
            return 0;
        }
    }
    while(true);
}