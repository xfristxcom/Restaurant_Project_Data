#include <iostream>
#include <iomanip>
#include "LinkedList.h"

using namespace std;

// Constructor: Initializes empty queue
LinkedList::LinkedList()
{
    head = nullptr;
    size = 0;
}

// Destructor: Cleans up all nodes
LinkedList::~LinkedList()
{
    clear();
}

// Clears all nodes in queue
void LinkedList::clear()
{
    OrderNode *current = head;
    while (current != nullptr)
    {
        OrderNode *next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    size = 0;
}

// Allocates new order node
OrderNode *LinkedList::newNode(int orderID, int tableNumber, string menuName, double price)
{
    OrderNode *n = new OrderNode();
    n->orderID = orderID;
    n->tableNumber = tableNumber;
    n->menuName = menuName;
    n->price = price;
    n->next = nullptr;
    return n;
}

// Recursive helper to insert at tail
void LinkedList::insertAtEnd(OrderNode *node, int orderID, int tableNumber, string menuName, double price)
{
    if (node->next == nullptr)
    {
        node->next = newNode(orderID, tableNumber, menuName, price);
    }
    else
    {
        insertAtEnd(node->next, orderID, tableNumber, menuName, price);
    }
}

// Enqueue order to tail of queue
void LinkedList::enqueueOrder(int orderID, int tableNumber, string menuName, double price)
{
    if (head == nullptr)
    {
        head = newNode(orderID, tableNumber, menuName, price);
    }
    else
    {
        insertAtEnd(head, orderID, tableNumber, menuName, price);
    }
    size++;
}

// Dequeue head order when kitchen finishes cooking
bool LinkedList::serveNextOrder(OrderNode &servedOrder)
{
    if (head == nullptr)
        return false;

    OrderNode *temp = head;
    servedOrder = *temp;
    head = head->next;
    delete temp;
    size--;

    return true;
}

// Serve a specific order by Order ID anywhere in the queue (e.g. quick drinks/snacks ready first)
bool LinkedList::serveOrderByID(int orderID, OrderNode &servedOrder)
{
    if (head == nullptr)
        return false;

    OrderNode *curr = head;
    OrderNode *prev = nullptr;

    while (curr != nullptr && curr->orderID != orderID)
    {
        prev = curr;
        curr = curr->next;
    }

    if (curr == nullptr)
        return false; // Order ID not found

    servedOrder = *curr;

    if (prev == nullptr)
    {
        head = head->next;
    }
    else
    {
        prev->next = curr->next;
    }

    delete curr;
    size--;
    return true;
}

// Move an existing order to a new queue position (1-based index)
bool LinkedList::moveOrderPosition(int orderID, int newPosition)
{
    if (head == nullptr || newPosition < 1 || newPosition > size)
        return false;

    // 1. Locate the target node and its previous node
    OrderNode *curr = head;
    OrderNode *prev = nullptr;
    int currentPos = 1;

    while (curr != nullptr && curr->orderID != orderID)
    {
        prev = curr;
        curr = curr->next;
        currentPos++;
    }

    if (curr == nullptr)
        return false; // Order ID not found

    if (currentPos == newPosition)
        return true; // Already at the desired position

    // 2. Detach target node from its current position
    if (prev == nullptr)
    {
        head = curr->next;
    }
    else
    {
        prev->next = curr->next;
    }
    curr->next = nullptr;

    // 3. Re-insert target node at newPosition
    if (newPosition == 1)
    {
        curr->next = head;
        head = curr;
    }
    else
    {
        // Traverse to (newPosition - 1)-th node in the list
        OrderNode *targetPrev = head;
        for (int i = 1; i < newPosition - 1 && targetPrev != nullptr; i++)
        {
            targetPrev = targetPrev->next;
        }

        if (targetPrev != nullptr)
        {
            curr->next = targetPrev->next;
            targetPrev->next = curr;
        }
    }

    return true;
}

// Recursive helper to cancel order by Order ID anywhere in queue
bool LinkedList::cancelOrderHelper(OrderNode *&node, int orderID, OrderNode &canceledOrder)
{
    if (node == nullptr)
        return false;

    if (node->orderID == orderID)
    {
        OrderNode *temp = node;
        canceledOrder = *temp;
        node = node->next;
        delete temp;
        return true;
    }

    return cancelOrderHelper(node->next, orderID, canceledOrder);
}

// Cancel order by Order ID
bool LinkedList::cancelOrder(int orderID, OrderNode &canceledOrder)
{
    if (cancelOrderHelper(head, orderID, canceledOrder))
    {
        size--;
        return true;
    }
    return false;
}

// Counts pending orders for a specific table
int LinkedList::countOrdersByTable(int tableNumber)
{
    int count = 0;
    OrderNode *curr = head;
    while (curr != nullptr)
    {
        if (curr->tableNumber == tableNumber)
        {
            count++;
        }
        curr = curr->next;
    }
    return count;
}

// Checks if table has pending orders
bool LinkedList::hasOrdersForTable(int tableNumber)
{
    OrderNode *curr = head;
    while (curr != nullptr)
    {
        if (curr->tableNumber == tableNumber)
            return true;
        curr = curr->next;
    }
    return false;
}

// Display orders waiting in the kitchen for a specific table
void LinkedList::displayTableOrders(int tableNumber)
{
    int count = 0;
    OrderNode *curr = head;
    cout << "\n============================================\n";
    cout << "   Kitchen Pending Orders for Table #" << tableNumber << "\n";
    cout << "============================================\n";
    cout << left << setw(10) << "Order ID" << setw(26) << "Item Name" << right << setw(10) << "Price ($)" << "\n";
    cout << "--------------------------------------------\n";

    while (curr != nullptr)
    {
        if (curr->tableNumber == tableNumber)
        {
            count++;
            cout << left << setw(10) << ("#" + to_string(curr->orderID))
                 << setw(26) << curr->menuName
                 << right << setw(10) << fixed << setprecision(2) << curr->price << "\n";
        }
        curr = curr->next;
    }

    if (count == 0)
    {
        cout << "   (No pending orders for this table)\n";
    }
    cout << "--------------------------------------------\n";
    cout << "Total " << count << " item(s) waiting to be served.\n";
    cout << "============================================\n";
}

// Display full kitchen cooking queue
void LinkedList::displayKitchenQueue()
{
    cout << "\n======================================================================\n";
    cout << "                    Kitchen Order Queue (Live)                        \n";
    cout << "======================================================================\n";
    cout << left << setw(6) << "No." 
         << setw(12) << "Order ID" 
         << setw(10) << "Table #" 
         << setw(30) << "Item Name" 
         << right << setw(10) << "Price ($)\n";
    cout << "----------------------------------------------------------------------\n";

    if (head == nullptr)
    {
        cout << "              (No pending orders in the kitchen)\n";
    }
    else
    {
        int index = 1;
        OrderNode *curr = head;
        while (curr != nullptr)
        {
            cout << left << setw(6) << index++
                 << setw(12) << ("#" + to_string(curr->orderID))
                 << setw(10) << curr->tableNumber
                 << setw(30) << curr->menuName
                 << right << setw(10) << fixed << setprecision(2) << curr->price << "\n";
            curr = curr->next;
        }
    }
    cout << "======================================================================\n";
    cout << "Total pending orders in kitchen: " << size << " item(s)\n";
}

int LinkedList::getSize()
{
    return size;
}

bool LinkedList::isEmpty()
{
    return size == 0;
}
