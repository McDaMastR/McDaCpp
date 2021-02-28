#pragma once
#include "BSCpch.hpp"

// TODO create mathematical bsc vector

// Wrapper over multi-dimentional array

namespace bsc
{
    template <class T, std::size_t S>
    class grid
    {
        // TODO
        // make functions noexcept after defined
        // Declare methods
        // Possible static members
 
    public:
        // Constructors/Destructor
        grid() noexcept                   = default;
        ~grid()                                    ;
        grid(const grid &other)           = delete ;
        grid(const grid &&other)          = delete ;
        grid operator=(const grid &other) = delete ;
		grid(const std::size_t *lengths[S])        ;

        std::size_t dimensions() const; // Get S (dimensions of array)
        std::size_t length(const std::size_t index) const;

        // Arithmatic operator overloads
              grid operator+(const grid &other)      ;
        const grid operator+(const grid &other) const;
              grid operator-(const grid &other)      ;
        const grid operator-(const grid &other) const;
              grid operator*(const grid &other)      ;
        const grid operator*(const grid &other) const;
              grid operator/(const grid &other)      ;
        const grid operator/(const grid &other) const;
              grid operator%(const grid &other)      ;
        const grid operator%(const grid &other) const;

        // Bitwise operator overloads
              grid operator& (const grid &other)      ;
        const grid operator& (const grid &other) const;
              grid operator| (const grid &other)      ;
        const grid operator| (const grid &other) const;
              grid operator^ (const grid &other)      ;
        const grid operator^ (const grid &other) const;
              grid operator~ (                 )      ;
        const grid operator~ (                 ) const;
              grid operator<<(const grid &other)      ;
        const grid operator<<(const grid &other) const;
              grid operator>>(const grid &other)      ;
        const grid operator>>(const grid &other) const;


        // Arithmatic/Bitwise-assignment operator overloads
        grid operator+= (const grid &other);
        grid operator-= (const grid &other);
        grid operator*= (const grid &other);
        grid operator/= (const grid &other);
        grid operator%= (const grid &other);
        grid operator&= (const grid &other);
        grid operator|= (const grid &other);
        grid operator^= (const grid &other);
        grid operator<<=(const grid &other);
        grid operator>>=(const grid &other);
        grid operator++ (                 );
        grid operator++ (const int        );
        grid operator-- (                 );
        grid operator-- (const int        );

        // Comparision operator overloads
        bool operator== (const grid &other) const		   ;
        bool operator!= (const grid &other) const		   ;
        bool operator>  (const grid &other) const		   ;
        bool operator>= (const grid &other) const		   ;
        bool operator<  (const grid &other) const		   ;
        bool operator<=	(const grid &other) const		   ;
        auto operator<=>(const grid &other) const = default;

        // Accessability operator overloads
              T operator[](const std::size_t index)      ;
        const T operator[](const std::size_t index) const;
              T operator()(const std::size_t index)      ;
        const T operator()(const std::size_t index) const;

        // Memory allocation operator overloads
        void *operator new     (const std::size_t size);
        void *operator new[]   (const std::size_t size);
        void  operator delete  (void *obj       	  );
        void  operator delete[](void *obj       	  );

        // Miscellaneous operator overloads
        grid operator+() const;
        grid operator-() const;

    private:
        T *array = nullptr;
        std::size_t lengths[S];
    };
}
