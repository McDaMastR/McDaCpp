#include <iostream> // Enables input and output

// using namespace std;
// using std::cout;

int main() // main function
{
    const short variable1 = 5; // Creates variable
    //int variable1; Declares variable1 exists
    //variable1 = 5; Initializes variable1

    std::string name;

    std::cout << "Hello world\n"; // "cout" with "using namespace std;" or "using std::cout;"
    std::cout << "My variable value is " << variable1 << std::endl;
    std::cout << "Name: ";
    std::cin >> name;
    std::cout << "Your name is " << name << std::endl; // The << and >> symbolise the data flow
    
    return 0;
}