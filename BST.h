#ifndef BST_H
#define BST_H

#include <iostream>
#include <fstream>
#include <string>
#include "Array.h"

using namespace std;

// Node of Binary Search Tree storing price and food items
struct PriceNode
{
    double price;            // Primary BST sorting key (Price level)
    Array<string> menuNames; // List of menu items having this price
    PriceNode *left;         // Left child pointer (Lower price)
    PriceNode *right;        // Right child pointer (Higher price)
};

// Binary Search Tree class for managing category food catalogs
class BinarySearchTree
{
private:
    PriceNode *root;         // Root pointer

    PriceNode *newNode(double price, string menuName);
    void destroy(PriceNode *node);

    PriceNode *insert(PriceNode *node, double price, string menuName);
    PriceNode *searchPrice(PriceNode *node, double price);
    bool searchMenuHelper(PriceNode *node, string menuName, double &foundPrice);
    
    PriceNode *findMin(PriceNode *node);
    PriceNode *findMax(PriceNode *node);
    PriceNode *deletePriceNode(PriceNode *node, double price);

    void inorder(PriceNode *node);
    void reverseInorder(PriceNode *node, int &itemIndex);
    void collectReverseHelper(PriceNode *node, Array<string> &names, Array<double> &prices);
    void saveNodeToFile(PriceNode *node, ofstream &outFile);
    int countNodes(PriceNode *node);
    int countTotalMenus(PriceNode *node);
    void print(PriceNode *node, int depth);

public:
    BinarySearchTree();
    ~BinarySearchTree();

    BinarySearchTree(const BinarySearchTree &other) = delete;
    BinarySearchTree &operator=(const BinarySearchTree &other) = delete;

    void insertMenu(double price, string menuName);
    bool searchMenu(string menuName, double &foundPrice);
    bool hasPrice(double price);
    bool deleteMenu(string menuName);
    void deletePrice(double price);

    double getMinPrice();
    double getMaxPrice();

    bool isEmpty();
    void displayPriceLowToHigh();
    void displayPriceHighToLow();
    void getOrderedMenus(Array<string> &names, Array<double> &prices);

    int getNodeCount();
    int getTotalMenuCount();
    void printTree();

    void clear();
    bool saveToFile(const string &filename);
    bool loadFromFile(const string &filename);
};

#endif
