#include <iostream>
#include <fstream>
#include <iomanip>
#include "BST.h"

using namespace std;

// Constructor: Initializes empty tree
BinarySearchTree::BinarySearchTree()
{
    root = nullptr;
}

// Destructor: Deallocates all nodes
BinarySearchTree::~BinarySearchTree()
{
    destroy(root);
    root = nullptr;
}

// Allocates new price node with initial menu name
PriceNode *BinarySearchTree::newNode(double price, string menuName)
{
    PriceNode *n = new PriceNode();
    n->price = price;
    n->menuNames.insertAtEnd(menuName);
    n->left = nullptr;
    n->right = nullptr;
    return n;
}

// Post-order deallocation
void BinarySearchTree::destroy(PriceNode *node)
{
    if (node == nullptr)
        return;

    destroy(node->left);
    destroy(node->right);
    delete node;
}

// Inserts menu item by price
PriceNode *BinarySearchTree::insert(PriceNode *node, double price, string menuName)
{
    if (node == nullptr)
        return newNode(price, menuName);

    if (price < node->price)
    {
        node->left = insert(node->left, price, menuName);
    }
    else if (price > node->price)
    {
        node->right = insert(node->right, price, menuName);
    }
    else
    {
        // Duplicate price: append to internal array
        if (node->menuNames.findElement(menuName) == -1)
        {
            node->menuNames.insertAtEnd(menuName);
        }
    }

    return node;
}

void BinarySearchTree::insertMenu(double price, string menuName)
{
    root = insert(root, price, menuName);
}

// Searches for price node
PriceNode *BinarySearchTree::searchPrice(PriceNode *node, double price)
{
    if (node == nullptr || node->price == price)
        return node;

    if (price < node->price)
        return searchPrice(node->left, price);

    return searchPrice(node->right, price);
}

bool BinarySearchTree::hasPrice(double price)
{
    return searchPrice(root, price) != nullptr;
}

// Searches for menu item by name across all nodes
bool BinarySearchTree::searchMenuHelper(PriceNode *node, string menuName, double &foundPrice)
{
    if (node == nullptr)
        return false;

    if (node->menuNames.findElement(menuName) != -1)
    {
        foundPrice = node->price;
        return true;
    }

    if (searchMenuHelper(node->left, menuName, foundPrice))
        return true;

    return searchMenuHelper(node->right, menuName, foundPrice);
}

bool BinarySearchTree::searchMenu(string menuName, double &foundPrice)
{
    return searchMenuHelper(root, menuName, foundPrice);
}

// Finds minimum price node
PriceNode *BinarySearchTree::findMin(PriceNode *node)
{
    if (node == nullptr)
        return nullptr;

    if (node->left == nullptr)
        return node;

    return findMin(node->left);
}

// Finds maximum price node
PriceNode *BinarySearchTree::findMax(PriceNode *node)
{
    if (node == nullptr)
        return nullptr;

    if (node->right == nullptr)
        return node;

    return findMax(node->right);
}

double BinarySearchTree::getMinPrice()
{
    PriceNode *min = findMin(root);
    if (min == nullptr)
        return -1.0;
    return min->price;
}

double BinarySearchTree::getMaxPrice()
{
    PriceNode *max = findMax(root);
    if (max == nullptr)
        return -1.0;
    return max->price;
}

// Deletes price node handling all 4 BST cases
PriceNode *BinarySearchTree::deletePriceNode(PriceNode *node, double price)
{
    if (node == nullptr)
        return nullptr;

    if (price < node->price)
    {
        node->left = deletePriceNode(node->left, price);
        return node;
    }

    if (price > node->price)
    {
        node->right = deletePriceNode(node->right, price);
        return node;
    }

    // Leaf node
    if (node->left == nullptr && node->right == nullptr)
    {
        delete node;
        return nullptr;
    }

    // One child
    if (node->left == nullptr)
    {
        PriceNode *temp = node->right;
        delete node;
        return temp;
    }
    if (node->right == nullptr)
    {
        PriceNode *temp = node->left;
        delete node;
        return temp;
    }

    // Two children: In-order successor
    PriceNode *successor = findMin(node->right);
    node->price = successor->price;
    node->menuNames = successor->menuNames;
    node->right = deletePriceNode(node->right, successor->price);

    return node;
}

void BinarySearchTree::deletePrice(double price)
{
    root = deletePriceNode(root, price);
}

// Deletes single menu item from array; deletes node if array becomes empty
bool BinarySearchTree::deleteMenu(string menuName)
{
    double foundPrice = 0.0;
    if (!searchMenu(menuName, foundPrice))
    {
        return false;
    }

    PriceNode *targetNode = searchPrice(root, foundPrice);
    if (targetNode != nullptr)
    {
        targetNode->menuNames.deleteElement(menuName);
        if (targetNode->menuNames.isEmpty())
        {
            root = deletePriceNode(root, foundPrice);
        }
        return true;
    }

    return false;
}

bool BinarySearchTree::isEmpty()
{
    return root == nullptr;
}

// Inorder traversal: Lowest to Highest price
void BinarySearchTree::inorder(PriceNode *node)
{
    if (node == nullptr)
        return;

    inorder(node->left);

    for (int i = 0; i < node->menuNames.getSize(); i++)
    {
        cout << "  - " << left << setw(28) << node->menuNames.getAt(i)
             << " Price: $" << right << setw(6) << fixed << setprecision(2) << node->price << "\n";
    }

    inorder(node->right);
}

void BinarySearchTree::displayPriceLowToHigh()
{
    if (root == nullptr)
    {
        cout << "  (No items in this category)\n";
        return;
    }
    inorder(root);
}

// Reverse Inorder traversal: Highest to Lowest price
void BinarySearchTree::reverseInorder(PriceNode *node, int &itemIndex)
{
    if (node == nullptr)
        return;

    reverseInorder(node->right, itemIndex);

    for (int i = 0; i < node->menuNames.getSize(); i++)
    {
        cout << "  " << setw(2) << itemIndex++ << ". " 
             << left << setw(28) << node->menuNames.getAt(i)
             << " [ $" << fixed << setprecision(2) << node->price << " ]\n";
    }

    reverseInorder(node->left, itemIndex);
}

void BinarySearchTree::displayPriceHighToLow()
{
    if (root == nullptr)
    {
        cout << "  (No items in this category)\n";
        return;
    }
    int itemIndex = 1;
    reverseInorder(root, itemIndex);
}

// Collects menus in reverse in-order for quick numeric selection
void BinarySearchTree::collectReverseHelper(PriceNode *node, Array<string> &names, Array<double> &prices)
{
    if (node == nullptr)
        return;

    collectReverseHelper(node->right, names, prices);

    for (int i = 0; i < node->menuNames.getSize(); i++)
    {
        names.insertAtEnd(node->menuNames.getAt(i));
        prices.insertAtEnd(node->price);
    }

    collectReverseHelper(node->left, names, prices);
}

void BinarySearchTree::getOrderedMenus(Array<string> &names, Array<double> &prices)
{
    names.clear();
    prices.clear();
    collectReverseHelper(root, names, prices);
}

int BinarySearchTree::countNodes(PriceNode *node)
{
    if (node == nullptr)
        return 0;

    return 1 + countNodes(node->left) + countNodes(node->right);
}

int BinarySearchTree::getNodeCount()
{
    return countNodes(root);
}

int BinarySearchTree::countTotalMenus(PriceNode *node)
{
    if (node == nullptr)
        return 0;

    return node->menuNames.getSize() + countTotalMenus(node->left) + countTotalMenus(node->right);
}

int BinarySearchTree::getTotalMenuCount()
{
    return countTotalMenus(root);
}

void BinarySearchTree::print(PriceNode *node, int depth)
{
    if (node == nullptr)
        return;

    print(node->right, depth + 1);

    for (int i = 0; i < depth; i++)
        cout << "        ";

    cout << "$" << fixed << setprecision(2) << node->price << " (" << node->menuNames.getSize() << " items)\n";

    print(node->left, depth + 1);
}

void BinarySearchTree::clear()
{
    destroy(root);
    root = nullptr;
}

void BinarySearchTree::saveNodeToFile(PriceNode *node, ofstream &outFile)
{
    if (node == nullptr)
        return;

    saveNodeToFile(node->left, outFile);

    for (int i = 0; i < node->menuNames.getSize(); i++)
    {
        outFile << fixed << setprecision(2) << node->price << "|" 
                << node->menuNames.getAt(i) << "\n";
    }

    saveNodeToFile(node->right, outFile);
}

bool BinarySearchTree::saveToFile(const string &filename)
{
    ofstream outFile(filename);
    if (!outFile.is_open())
        return false;

    saveNodeToFile(root, outFile);
    outFile.close();
    return true;
}

bool BinarySearchTree::loadFromFile(const string &filename)
{
    ifstream inFile(filename);
    if (!inFile.is_open())
        return false;

    clear();

    string line;
    while (getline(inFile, line))
    {
        if (line.empty())
            continue;

        size_t sep = line.find('|');
        if (sep != string::npos)
        {
            string priceStr = line.substr(0, sep);
            string menuName = line.substr(sep + 1);
            try
            {
                double price = stod(priceStr);
                insertMenu(price, menuName);
            }
            catch (...)
            {
            }
        }
    }

    inFile.close();
    return true;
}

void BinarySearchTree::printTree()
{
    if (root == nullptr)
    {
        cout << "Tree is empty\n";
        return;
    }
    cout << "--- BST Price Structure (Root on the left) ---\n";
    print(root, 0);
    cout << "------------------------------------------------\n";
}
