#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

// Generic Dynamic Array template class
template <typename T>
class Array
{
private:
    int capacity;       // Current allocated capacity
    void resize();      // Doubles capacity when full
    void create(int cap); // Allocates initial memory block

protected:
    T *arr;             // Pointer to heap array buffer
    int size;           // Number of active elements stored

public:
    Array();                             // Constructor: reserves initial capacity of 5
    Array(const Array<T>& other);        // Deep-copy constructor
    Array<T>& operator=(const Array<T>& other); // Safe assignment operator
    ~Array();                            // Destructor: releases heap memory

    void insertAtFirst(T value);         // Inserts element at index 0
    void insertAtEnd(T value);           // Inserts element at the end
    void insertAtIndex(int index, T value); // Inserts element at specified index

    int findElement(T value);            // Returns index of first match or -1
    void deleteElement(T value);         // Deletes first match
    T deleteAtIndex(int index);          // Removes and returns element at index

    int getSize() const;                 // Returns current size
    bool isEmpty() const;                // Returns true if size is 0
    T& getAt(int index);                 // Mutable element access
    const T& getAt(int index) const;     // Read-only element access
    void setAt(int index, T value);      // Sets element value at index
    void clear();                        // Resets size to 0

    void display() const;                // Prints all elements
};

#endif
