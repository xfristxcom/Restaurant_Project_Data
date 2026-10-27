#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <string>

using namespace std;

// Node structure for Kitchen Queue
struct OrderNode
{
    int orderID;        // Order ID (e.g. 1001, 1002, ...)
    int tableNumber;    // Table number
    string menuName;    // Name of ordered food
    double price;       // Price
    OrderNode *next;    // Pointer to next order in queue
};

// Singly Linked List class for managing Kitchen Queue
class LinkedList
{
private:
    OrderNode *head;    // Pointer to head order in queue
    int size;           // Number of pending orders in kitchen

    OrderNode *newNode(int orderID, int tableNumber, string menuName, double price);
    void insertAtEnd(OrderNode *node, int orderID, int tableNumber, string menuName, double price);
    bool cancelOrderHelper(OrderNode *&node, int orderID, OrderNode &canceledOrder);

public:
    LinkedList();
    ~LinkedList();

    LinkedList(const LinkedList &other) = delete;
    LinkedList &operator=(const LinkedList &other) = delete;

    void enqueueOrder(int orderID, int tableNumber, string menuName, double price);
    bool serveNextOrder(OrderNode &servedOrder);
    bool serveOrderByID(int orderID, OrderNode &servedOrder);
    bool moveOrderPosition(int orderID, int newPosition);
    bool cancelOrder(int orderID, OrderNode &canceledOrder);

    int countOrdersByTable(int tableNumber);
    bool hasOrdersForTable(int tableNumber);
    void displayTableOrders(int tableNumber);

    void displayKitchenQueue();
    void clear();

    int getSize();
    bool isEmpty();
};

#endif
