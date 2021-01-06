#include <iostream>
#include <vector> // To use vectors
#include <array> // To use STD arrays

int main()
{
    int array[20]; // The amount of values allowed to be stored inside an array is constant

    std::cout << "Values to be stored in array:" << std::endl;

    for(int i = 0; i < 20; i++) // Storing values in array
    {
        std::cin >> array[i];
    }
    for(int i = 0; i < 20; i++) // Array has to be printed out 1 value at a time
    {
        std::cout << array[i] << "\t";
    }


    std::vector<int> vector; // The amount of values allowed in a vector changes to the amount that is in it every time that is changed

    std::cout << "Values to be stored in vector:" << std::endl;

    for(int i = 0; i < 20; i++) // Storing values in vector
    {
        int input;
        std::cin >> input;
        vector.push_back(input); // Adds input to the end of vector

        if(i == 19)
        {
            vector.pop_back(); // Removes the last value from vector
        }
    }
    for(int i = 0; i < 20; i++) // Vector has to be printed out 1 value at a time
    {
        std::cout << vector[i] << "\t";
    }
    std::cout << vector.size() << std::endl; // Returns the size of the vector (not in index values)
    std::cout << vector.front() << std::endl; // Returns the first value of the vector
    std::cout << vector.back() << std::endl; // Returns the last value of the vector


    std::array<int, 20> STLarray; // STD array is statically sized (size cannot be changed)

    std::cout << "Values to be stored in STL array:" << std::endl;

    for(int i = 0; i < 20; i++)
    {
        std::cin >> STLarray[i];
    }
    for(int i = 0; i < 20; i++)
    {
        std::cout << STLarray[i] << "\t";
    }
    std::cout << STLarray.size() << std::endl; // Returns the size of the STD array (not in index values)


    int arrays[][3] = 
    {
        {1, 2, 3}, 
        {4, 5, 6}, 
        {7, 8, 9}, 
        {10, 11, 12}
    }; // Creates a multidimentional array (arrays within arrays)

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 3; j++) // Need a nested for loop to print out all the values of a multidimentional array
        {
            std::cout << arrays[i][j] << "\t";
        }
        std::cout << std::endl;
    }


    std::vector<std::vector<int>> vectors = 
    {
        {1, 2, 3}, 
        {4, 5, 6}, 
        {7, 8, 9}, 
        {10, 11, 12}
    }; // Creates a nested vector (vectors within vectors)

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            std::cout << vectors[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}