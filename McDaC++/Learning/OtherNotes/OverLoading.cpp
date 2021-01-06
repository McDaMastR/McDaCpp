#include <iostream>

double area(double x)
{
    return x * x;
}

double area(double x, double y) // Even though this function has the same identifier (name) as the one above,
// they can run perfectly seperatly because the compiler tells them apart with the number/type of arguments in each.
{
    return x * y;
}

double power(double x, int y = 2) // Defaulting variable y to 2 causes the 2nd argument to become optional
{
    double ans = 1;

    for(int i = 0; i < y; i++)
    {
        ans *= x;
    }

    return ans;
}

template <typename T> // Type T generalises to any data type
void swap(T &x, T &y)
{
    T z = x;
    x = y;
    y = z;
}

template <typename T>
void swap(T x[], T y[], int size) // Overloading two templetized functions
{
    for(int i = 0; i < size; i++)
    {
        T z = a[i];
        a[i] = b[i];
        b[i] = z[i];
    }
}

int main()
{
    int a = 10;
    int b = 20;
    int c[] = {1, 2, 3, 4, 5};
    int d[] = {6, 7, 8, 9, 10};

    std::cout << area(2) << std::endl;
    std::cout << area(2, 4) << std::endl;
    std::cout << power(4) << std::endl;
    std::cout << a << "\t" << b << std::endl;
    swap(a, b);
    std::cout << a << "\t" << b << std::endl;

    return 0;
}