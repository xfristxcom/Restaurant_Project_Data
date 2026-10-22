#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <string>
#include "Array.h"

using namespace std;

// Table structure for layout and billing
struct Table
{
    int tableNumber;            // Table Number (1, 2, 3, ...)
    bool isOccupied;            // Seating status: occupied or free
    Array<string> servedItems;  // Food items that have been served
    Array<double> itemPrices;   // Prices of served items
    double currentBill;         // Current total bill amount

    Table()
    {
        tableNumber = 0;
        isOccupied = false;
        currentBill = 0.0;
    }

    Table(int num)
    {
        tableNumber = num;
        isOccupied = false;
        currentBill = 0.0;
    }

    bool operator==(const Table &other) const
    {
        return tableNumber == other.tableNumber;
    }

    // Add served food item to table bill
    void addServedFood(string foodName, double price)
    {
        isOccupied = true;
        servedItems.insertAtEnd(foodName);
        itemPrices.insertAtEnd(price);
        currentBill += price;
    }

    // Clear bill upon checkout
    void clearBill()
    {
        isOccupied = false;
        servedItems.clear();
        itemPrices.clear();
        currentBill = 0.0;
    }
};

inline ostream& operator<<(ostream& os, const Table& t)
{
    os << "[Table " << t.tableNumber << ": " << (t.isOccupied ? "Occupied" : "Available") 
       << " | $" << t.currentBill << "]";
    return os;
}

#endif
