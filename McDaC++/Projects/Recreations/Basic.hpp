#pragma once

#include <iostream>
#include <fstream>

namespace bsc
{
    class Array
    {

    };

    // Vector class
    template<class T>
    class Vector
    {
    public:
        // Constructors/destructors
        Vector();

        ~Vector();

        Vector(const Vector<T> &other);

        Vector(const Vector<T> &&other);

        // Methods
        size_t get_size() const;

        size_t get_capacity() const;

        void resize(const size_t size);

        void reserve(const size_t capacity);

        void push_back(const T object);

        void push_forward(const T object);

        void push_back(const T &&object);

        void push_forward(const T &&object);

        void push_back_ref(const T &object);

        void push_forward_ref(const T &object);

        template<class... Args>
        void emplace_back(Args&&... args);

        template<class... Args>
        void emplace_forward(Args&&... args);

        void pop_back();

        void pop_forward();

        T& get(const size_t place);

        const T& get(const size_t place) const;

        void set(size_t place, T object);

        void reload();

        void clear();

        bool is_empty();

        bool is_clear();

        void auto_size();

        T& swap(const size_t place, const T object);

        void print() const;

        // Operator overloads
        Vector operator + (const Vector<T> vector);

        Vector operator - (const Vector<T> vector);

        void operator = (Vector<T> vector);

        bool operator == (const Vector<T> vector);

        bool operator != (const Vector<T> vector);

        bool operator < (const Vector<T> vector);

        bool operator <= (const Vector<T> vector);

        bool operator > (const Vector<T> vector);

        bool operator >= (const Vector<T> vector);

        void operator ++ ();

        void operator ++ (int);

        void operator -- ();

        void operator -- (int);

        T& operator [] (const size_t place);

        const T& operator [] (const size_t place) const;

        friend std::ostream &operator << (std::ostream &output, const Vector<T> &vector);

        friend std::istream &operator >> (std::istream &input, Vector<T> &vector);

        friend std::ofstream &operator << (std::ofstream &output, Vector<T> &vector);

        friend std::ifstream &operator >> (std::ifstream &output, Vector<T> &vector);

    private:
        size_t size;
        size_t capacity;
        T *array_ptr = nullptr;
        T &array_ref = *array_ptr;
    };
    
    template<class T>
    std::ostream &operator << (std::ostream &output, const Vector<T> &vector);

    template<class T>
    std::istream &operator >> (std::istream &input, Vector<T> &vector);

    template<class T>
    std::ofstream &operator << (std::ofstream &output, const Vector<T> &vector);

    template<class T>
    std::ifstream &operator >> (std::ifstream &input, Vector<T> &vector);
}