// To define the Vector class in the basic (bsc) namespace header file
#include "Basic.hpp"

// Vector class is inside basic (bsc) namespace
namespace bsc
{
    // Constructors/destructors of Vector class

    // Default constructor
    template<class T>
    Vector<T>::Vector()
        : size(0), capacity(0), array_ptr(new T[0]) {}
        
    // Default destructor
    template<class T>
    Vector<T>::~Vector()
    {
        delete[] array_ptr;
    }

    // Copy constructor
    template<class T>
    Vector<T>::Vector(const Vector<T> &other)
    : size(other.get_size()), capacity(other.get_capacity()), array_ptr(new T[capacity])
    {
        for(size_t i = 0; i < other.get_size(); i++)
            array_ref[i] = other[i];
    }

    // Move constructor
    template<class T>
    Vector<T>::Vector(const Vector<T> &&other)
        : size(other.get_size()), capacity(other.get_capacity()), array_ptr(new T[capacity])
    {
        for(size_t i = 0; i < other.get_size(); i++)
            array_ref[i] = other[i];
    }


    // Methods of Vector class

    // Access size of vector
    template<class T>
    size_t Vector<T>::get_size() const
    {
        return size;
    }

    // Access capacity of vector
    template<class T>
    size_t Vector<T>::get_capacity() const
    {
        return capacity();
    }

    // Resize vector to given size
    template<class T>
    void Vector<T>::resize(const size_t size)
    {
        if(this->size == size)
            return;

        T temp_array[this->size] = array_ref;
        delete[] array_ptr;

        array_ptr = new T[size];
        for(size_t i = 0; i < this->size; i++)
        {
            if(i == size - 1)
                break;

            array_ref[i] = temp_array[i];
        }

        this->size = size;
    }

    // Allocate memory for given amount
    template<class T>
    void Vector<T>::reserve(const size_t capacity)
    {
        if(capacity == 0)
            return;
           
        this->capacity = capacity;
        T temp_array[size] = array_ref;
        delete[] array_ptr;

        array_ptr = new T[capacity];
        for(size_t i = 0; i < size; i++)
            array_ref[i] = temp_array[i];
    }

    // Add copy of object to back of vector
    template<class T>
    void Vector<T>::push_back(const T object)
    {
        size++;

        if(size - 1 == capacity)
        {
            T temp_array[size - 1] = array_ref;
            delete[] array_ptr;
            capacity++;

            array_ptr = new T[capacity];
            for(size_t i = 0; i < size; i++)
            {
                if(i == size - 1)
                    array_ref[i] = std::move(object);
                    
                else
                    array_ref[i] = temp_array[i];
            }
        }

        else
            array_ref[size] = std::move(object);
    }

    // Add copy of object to front of vector
    template<class T>
    void Vector<T>::push_forward(const T object)
    {
        size++;

        if(size - 1 == capacity)
        {
            T temp_array[size - 1] = array_ref;
            delete[] array_ptr;
            capacity++;

            array_ptr = new T[capacity];
            for(size_t i = 0; i < size; i++)
            {
                if(!i)
                    array_ref[0] = std::move(object);
                   
                else
                    array_ref[i] = temp_array[i - 1];
            }
        }
        else
        {
            for(size_t i = size; i > -1; i--)
            {
                if(!i)
                    array_ref[0] = std::move(object);

                else
                    array_ref[i - 1] = array_ref[i];
            }
        }
    }

    // Move given object to back of vector
    template<class T>
    void Vector<T>::push_back(const T &&object)
    {
        size++;

        if(size - 1 == capacity)
        {
            T temp_array[size - 1] = array_ref;
            delete[] array_ptr;
            capacity++;

            array_ptr = new T[capacity];
            for(size_t i = 0; i < size; i++)
            {
                if(i == size - 1)
                    array_ref[i] = std::move(object);
                    
                else
                    array_ref[i] = temp_array[i];
            }
        }

       else
            array_ref[size] = std::move(object);
    }

    // Move given object to front of vector
    template<class T>
    void Vector<T>::push_forward(const T &&object)
    {
        size++;

        if(size - 1 == capacity)
        {
            T temp_array[size - 1] = array_ref;
            delete[] array_ptr;
            capacity++;

            array_ptr = new T[capacity];
            for(size_t i = 0; i < size; i++)
            {
                if(!i)
                    array_ref[0] = std::move(object);
                    
                else
                    array_ref[i] = temp_array[i - 1];
            }
        }

        else
        {
            for(size_t i = size; i > -1; i--)
            {
                if(!i)
                    array_ref[0] = std::move(object);

                else
                    array_ref[i - 1] = array_ref[i];
            }
        }
    }

    // Add reference to object to back of vector
    template<class T>
    void Vector<T>::push_back_ref(const T &object)
    {
        size++;

        if(size - 1 == capacity)
        {
            T temp_array[size - 1] = array_ref;
            delete[] array_ptr;
            capacity++;

            array_ptr = new T[capacity];
            for(size_t i = 0; i < size; i++)
            {
                if(i == size - 1)
                    array_ref[i] = std::move(object);
                    
                else
                    array_ref[i] = temp_array[i];
            }
        }

        else
            array_ref[size] = std::move(object);
    }

    // Add reference to object to front of vector
    template<class T>
    void Vector<T>::push_forward_ref(const T &object)
    {
        size++;

        if(size - 1 == capacity)
        {
            T temp_array[size - 1] = array_ref;
            delete[] array_ptr;
            capacity++;

            array_ptr = new T[capacity];
            for(size_t i = 0; i < size; i++)
            {
                if(!i)
                    array_ref[0] = std::move(object);
                    
                else
                    array_ref[i] = temp_array[i - 1];
            }
        }
        else
        {
            for(size_t i = size; i > -1; i--)
            {
                if(!i)
                    array_ref[0] = std::move(object);

                else
                    array_ref[i - 1] = array_ref[i];
            }
        }
    }

    // Allocate memory if needed and construct object for that memory with given parmeters at the back of vector
    template<class T>
    template<class... Args>
    void Vector<T>::emplace_back(Args&&... args)
    {
        size++;

        if(size - 1 == capacity)
        {
            T temp_array[size - 1] = array_ref;
            delete[] array_ptr;
            capacity++;

            array_ptr = new T[capacity];
            for(size_t i = 0; i < size; i++)
            {
                if(i == size - 1)
                    new(&array_ref[i]) T(std::forward<Args>(args)...);
                  
                else
                    array_ref[i] = temp_array[i];
            }
        }

        else
            new(&array_ref(size)) T(std::forward<Args>(args)...);
    }

    // Allocate memory if needed and construct object for that memory with given parmeters at the front of vector
    template<class T>
    template<class... Args>
    void Vector<T>::emplace_forward(Args&&... args)
    {
        size++;

        if(size - 1 == capacity)
        {
            T temp_array[size - 1] = array_ref;
            delete[] array_ptr;
            capacity++;

            array_ptr = new T[capacity];
            for(size_t i = 0; i < size; i++)
            {
                if(!i)
                    new(&array_ref[0]) T(std::forward<Args>(args)...);
                  
                else
                    array_ref[i] = temp_array[i - 1];
            }
        }
        else
        {
            for(size_t i = size; i > -1; i--)
            {
                if(!i)
                    new(&array_ref[0]) T(std::forward<Args>(args)...);

                else
                    array_ref[i - 1] = array_ref[i];
            }
        }
    }

    // Set object at back of vector to null
    template<class T>
    void Vector<T>::pop_back()
    {
        for(size_t i = size; i > -1; i--)
        {
            if(array_ref[i])
            {
                array_ref[i] = 0;
                break;
            }
        }

        size--;
    }

    // Set object at front of vector to null
    template<class T>
    void Vector<T>::pop_forward()
    {
        for(size_t i = 0; i < size; i++)
        {
            if(!i)
                continue;
                
            array_ref[i - 1] = array_ref[i];
        }

        size--;
    }

    // Access object in given place of vector
    template<class T>
    T& Vector<T>::get(const size_t place)
    {
        if(place < 0 || place > size - 1)
            return;
            
        return array_ref[place];
    }

    // Access object in given place of vector (read only)
    template<class T>
    const T& Vector<T>::get(const size_t place) const
    {
        if(place < 0 || place > size - 1)
            return;
            
        return array_ref[place];
    }

    // Set object in given placement of vector to given object
    template<class T>
    void Vector<T>::set(size_t place, T object)
    {
        if(place < 0 || place > size - 1)
            return;
            
        array_ref[place] = std::move(object);
    }

    // Recreate the vector
    template<class T>
    void Vector<T>::reload()
    {
        T temp_array[size] = array_ref;
        delete[] array_ptr;

        array_ptr = new T[capacity];
        for(size_t i = 0; i < size; i++)
            array_ref[i] = temp_array[i];
    }

    // Clear all objects and allocated memory of vector
    template<class T>
    void Vector<T>::clear()
    {
        size = 0;
        capacity = 0;
        delete[] array_ptr;
        array_ptr = new T[0];
    }

    // Check if vector has no objects
    template<class T>
    bool Vector<T>::is_empty()
    {
        if(size != 0)
            return false;
            
        for(T object : array_ref)
        {
            if(object != 0)
                return false;
        }

        return true;
    }

    // Check if vector has no allocated memory
    template<class T>
    bool Vector<T>::is_clear()
    {
        if(capacity == 0)
            return true;
            
        return false;
    }

    // Resize capacity of vector to the size of vector
    template<class T>
    void Vector<T>::auto_size()
    {
        if(capacity == size)
            return;

        size_t no_objects = 0;
        for(T object : array_ref)
        {
            if(object)
                no_objects++;
        }

        if(size == no_objects)
            return;
           
        size_t i = 0;
        T objects[no_objects];

        for(T object : array_ref)
        {
            if(object)
            {
                objects[i] = std::move(object);
                i++;
            }
        }
            
        size -= no_objects;
        delete[] array_ptr;

        array_ptr = new T[size];
        for(size_t i = 0; i < size; i++)
            array_ref[i] = std::move(objects[i]);
    }

    // Swap object in given placement of vector with given object and return swapped object from vector
    template<class T>
    T& Vector<T>::swap(const size_t place, const T object)
    {
        if(place < 0 || place > capacity - 1)
            return;
            
        T return_object = array_ref[place];
        array_ref[place] = std::move(object);
        return *return_object;
    }

    // Print out each object of vector, each on a new line
    template<class T>
    void Vector<T>::print() const
    {
        for(T object : array_ref)
            std::cout << object << std::endl;
    }

    // Operator overloads of Vector class

    // Overload + operator to add objects of two vectors together
    template<class T>
    Vector<T> Vector<T>::operator + (const Vector<T> vector)
    {
        Vector<T> new_vector;
        new_vector.resize(size + vector.get_size());

        for(size_t i = 0; i < size; i++)
            new_vector.set(i, array_ref[i]);
           
        for(size_t i = size; i < size + vector.get_size(); i++)
            new_vector.set(i, vector.get(i - size));
            
        return new_vector;
    }

    // Overload - operator to set obejcts from vector to null which are in the range of second vector's number of objects
    template<class T>
    Vector<T> Vector<T>::operator - (const Vector<T> vector)
    {
        if(size - vector.get_size() < 0)
            return;
            
        Vector<T> new_vector;
        new_vector.resize(size - vector.get_size());

        for(size_t i = 0; i < size - vector.get_size(); i++)
            new_vector.set(i, array_ref[i]);
            
        return new_vector;
    }

    // Overload = operator to copy contents from one vector to another, setting the old contents to null
    template<class T>
    void Vector<T>::operator = (Vector<T> vector)
    {
        size = vector.get_size();
        capacity = vector.get_capacity();

        delete[] array_ptr;
        array_ptr = new T[capacity];

        for(size_t i = 0; i < vector.get_size(); i++)
            array_ref[i] = vector.get(i);
    }

    // Overload == operator to check if the size and objects from two vectors are equal
    template<class T>
    bool Vector<T>::operator == (const Vector<T> vector)
    {
        if(size != vector.get_size())
            return false;

        for(size_t i = 0; i < size; i++)
        {
            if(vector.get(i) != array_ref[i])
                return false;
        }
            
        return true;
    }

    // Overload != operator to check if the size and objects from two vectors are different
    template<class T>
    bool Vector<T>::operator != (const Vector<T> vector)
    {
        if(size != vector.get_size())
            return true;

        for(size_t i = 0; i < size; i++)
        {
            if(vector.get(i) != array_ref[i])
                return true;
        }
            
        return false;
    }

    // Overload < operator to check if the size of one vector is smaller than another's
    template<class T>
    bool Vector<T>::operator < (const Vector<T> vector)
    {
        if(size < vector.get_size())
            return true;
            
        return false;
    }

    // Overload <= operator to check if the size of one vector is smaller than or equal to another's
    template<class T>
    bool Vector<T>::operator <= (const Vector<T> vector)
    {
        if(size <= vector.get_size())
            return true;
            
        return false;
    }

    // Overload > operator to check if the size of one vector is larger than another's
    template<class T>
    bool Vector<T>::operator > (const Vector<T> vector)
    {
        if(size > vector.get_size())
            return true;
            
        return false;
    }

    // Overload >= operator to check if the size of one vector is larger than or equal to another's
    template<class T>
    bool Vector<T>::operator >= (const Vector<T> vector)
    {
        if(size >= vector.get_size())
            return true;
            
        return false;
    }

    // Overload ++vector operator to allocate space for one more object
    template<class T>
    void Vector<T>::operator ++ ()
    {
        T temp_array[size - 1] = array_ref;
        delete[] array_ptr;

        capacity++;
        array_ptr = new T[capacity];
        for(size_t i = 0; i < size - 1; i++)
            array_ref[i] = temp_array[i];
    }

    // Overload vector++ operator to allocate space for one more object
    template<class T>
    void Vector<T>::operator ++ (int)
    {
        T temp_array[size - 1] = array_ref;
        delete[] array_ptr;

        capacity++;
        array_ptr = new T[capacity];
        for(size_t i = 0; i < size - 1; i++)
            array_ref[i] = temp_array[i];
    }

    // Overload --vector operator to deallocate space allocated for one object and set the object in that space to null
    template<class T>
    void Vector<T>::operator -- ()
    {
        T temp_array[size] = array_ref;
        delete[] array_ptr;

        if(size == capacity)
            size--;

        capacity--;
        array_ptr = new T[capacity];

        for(size_t i = 0; i < size; i++)
            array_ref[i] = temp_array[i];
    }

    // Overload vector-- operator to deallocate space allocated for one object and set the object in that space to null
    template<class T>
    void Vector<T>::operator -- (int)
    {
        T temp_array[size] = array_ref;
        delete[] array_ptr;

        if(size == capacity)
            size--;

        capacity--;
        array_ptr = new T[capacity];

        for(size_t i = 0; i < size; i++)
            array_ref[i] = temp_array[i];
    }

    // Overload operator [] to access object in given place of vector
    template<class T>
    T& Vector<T>::operator [] (const size_t place)
    {
        if(place < 0 || place > size - 1)
            return;
            
        return array_ref[place];
    }

    // Overload operator [] to access object in given place of vector (read only)
    template<class T>
    const T& Vector<T>::operator [] (const size_t place) const
    {
        if(place < 0 || place > size - 1)
            return;
            
        return array_ref[place];
    }


    // Friend functions of Vector class

    // Overload (std::cout) << operator to output each object in the vector
    template<class T>
    std::ostream &operator << (std::ostream &output, const Vector<T> &vector)
    {
        for(T object : vector)
            std::cout << object << std::endl;
            
        return output;
    }

    // Overload (std::cin) >> operator to move given input into back of vector
    template<class T>
    std::istream &operator >> (std::istream &input, Vector<T> &vector)
    {
        vector.push_back(input);

        return input;
    }

    // Overload (std::ofstream) << to copy object from vector into file
    template<class T>
    std::ofstream &operator << (std::ofstream &output, const Vector<T> &vector)
    {
        for(T object : vector)
            output << object;
            
        return output;
    }

    // Overload (std::ifstream) >> to move object from file to back of vector
    template<class T>
    std::ifstream &operator >> (std::ifstream &input, Vector<T> &vector)
    {
        vector.push_back(input);

        return input;
    }
}