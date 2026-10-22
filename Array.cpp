#include <iostream>
#include <stdexcept>
#include "Array.h"
#include "Table.h"
#include "Admin.h"

using namespace std;

// Constructor: Initializes array and reserves capacity of 5
template <typename T>
Array<T>::Array()
{
    arr = nullptr;
    capacity = 0;
    size = 0;
    create(5);
}

// Deep copy constructor
template <typename T>
Array<T>::Array(const Array<T>& other)
{
    capacity = other.capacity;
    size = other.size;
    if (capacity > 0)
    {
        arr = new T[capacity];
        for (int i = 0; i < size; i++)
        {
            arr[i] = other.arr[i];
        }
    }
    else
    {
        arr = nullptr;
    }
}

// Deep assignment operator
template <typename T>
Array<T>& Array<T>::operator=(const Array<T>& other)
{
    if (this == &other)
        return *this;

    delete[] arr;
    capacity = other.capacity;
    size = other.size;
    if (capacity > 0)
    {
        arr = new T[capacity];
        for (int i = 0; i < size; i++)
        {
            arr[i] = other.arr[i];
        }
    }
    else
    {
        arr = nullptr;
    }
    return *this;
}

// Destructor: Releases array memory
template <typename T>
Array<T>::~Array()
{
    delete[] arr;
}

// Allocates memory for the given capacity and resets size to 0
template <typename T>
void Array<T>::create(int cap)
{
    delete[] arr;

    capacity = cap;
    size = 0;

    arr = new T[capacity];
}

// Doubles array capacity when full
template <typename T>
void Array<T>::resize()
{
    int newCapacity = (capacity == 0) ? 1 : capacity * 2;
    T *newArr = new T[newCapacity];

    for (int i = 0; i < size; i++)
    {
        newArr[i] = arr[i];
    }

    delete[] arr;

    arr = newArr;
    capacity = newCapacity;
}

// Inserts value at the beginning
template <typename T>
void Array<T>::insertAtFirst(T value)
{
    insertAtIndex(0, value);
}

// Inserts value at the end
template <typename T>
void Array<T>::insertAtEnd(T value)
{
    insertAtIndex(size, value);
}

// Inserts value at the given index, shifts subsequent elements right
template <typename T>
void Array<T>::insertAtIndex(int index, T value)
{
    if (index < 0 || index > size)
    {
        cout << "Invalid index\n";
        return;
    }

    if (size == capacity)
        resize();

    for (int i = size; i > index; i--)
    {
        arr[i] = arr[i - 1];
    }

    arr[index] = value;
    size++;
}

// Searches for index of value, returns -1 if not found
template <typename T>
int Array<T>::findElement(T value)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == value)
            return i;
    }

    return -1;
}

// Finds and deletes first occurrence of value
template <typename T>
void Array<T>::deleteElement(T value)
{
    int index = findElement(value);

    if (index == -1)
    {
        cout << "Value not found\n";
        return;
    }

    deleteAtIndex(index);
}

// Removes element at index and returns it
template <typename T>
T Array<T>::deleteAtIndex(int index)
{
    if (index < 0 || index >= size)
    {
        cout << "Invalid index\n";
        return T();
    }

    T value = arr[index];

    for (int i = index; i < size - 1; i++)
    {
        arr[i] = arr[i + 1];
    }

    size--;

    return value;
}

// Returns current size
template <typename T>
int Array<T>::getSize() const
{
    return size;
}

// Checks if array is empty
template <typename T>
bool Array<T>::isEmpty() const
{
    return size == 0;
}

// Accesses element at index (mutable)
template <typename T>
T& Array<T>::getAt(int index)
{
    if (index < 0 || index >= size)
    {
        throw std::out_of_range("Array index out of bounds");
    }
    return arr[index];
}

// Accesses element at index (read-only)
template <typename T>
const T& Array<T>::getAt(int index) const
{
    if (index < 0 || index >= size)
    {
        throw std::out_of_range("Array index out of bounds");
    }
    return arr[index];
}

// Sets value at index
template <typename T>
void Array<T>::setAt(int index, T value)
{
    if (index >= 0 && index < size)
    {
        arr[index] = value;
    }
}

// Clears array elements
template <typename T>
void Array<T>::clear()
{
    size = 0;
}

// Displays array elements
template <typename T>
void Array<T>::display() const
{
    cout << "\nSize = " << size << endl;
    cout << "Capacity = " << capacity << endl;
    cout << "Array : ";
    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Explicit Instantiations
template class Array<int>;
template class Array<string>;
template class Array<double>;
template class Array<Table>;
template class Array<Admin>;
