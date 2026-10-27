#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "Array.h"
#include "BST.h"
#include "LinkedList.h"
#include "Table.h"
#include "Admin.h"

using namespace std;

// ==========================================
// Text Data File Names
// ==========================================
const string FILE_DRINKS = "menu_drinks.txt";
const string FILE_DISHES = "menu_dishes.txt";
const string FILE_SOUPS  = "menu_soups.txt";
const string FILE_FRIED  = "menu_fried.txt";
const string FILE_TABLES = "tables.txt";
const string FILE_ADMINS = "admins.txt";
const string FILE_STORE  = "store.txt";

// ==========================================
// Global System State
// ==========================================
BinarySearchTree drinkTree;       // BST Catalog for Drinks
BinarySearchTree singleDishTree;  // BST Catalog for Single Dishes
BinarySearchTree soupTree;        // BST Catalog for Soups & Curries
BinarySearchTree friedTree;       // BST Catalog for Fried & Snacks

LinkedList kitchenQueue;          // Singly Linked List for Kitchen Queue
Array<Table> tables;              // Dynamic Array for Table Layout & Billing
Array<Admin> admins;              // Dynamic Array for Admin Credentials

int nextOrderID = 1001;           // Auto-increment Order ID generator
double totalDailyRevenue = 0.0;   // Accumulated daily revenue
bool isStoreOpen = true;          // Store Status: true = OPEN, false = CLOSED

// ==========================================
// Helper Utilities
// ==========================================
void clearScreen()
{
    system("cls");
}

void clearInput()
{
    cin.clear();
    cin.ignore(10000, '\n');
}

void pauseConsole()
{
    cout << "\n";
    system("pause");
}

// Helper to select BST tree based on category choice
BinarySearchTree* getCategoryTree(int catChoice, string &catName)
{
    switch (catChoice)
    {
    case 1:
        catName = "Drinks";
        return &drinkTree;
    case 2:
        catName = "Single Dishes";
        return &singleDishTree;
    case 3:
        catName = "Soups & Curries";
        return &soupTree;
    case 4:
        catName = "Fried & Snacks";
        return &friedTree;
    default:
        catName = "";
        return nullptr;
    }
}

// Find table index by table number (returns index or -1)
int findTableIndex(int tableNum)
{
    for (int i = 0; i < tables.getSize(); i++)
    {
        if (tables.getAt(i).tableNumber == tableNum)
            return i;
    }
    return -1;
}

// Seed default initial data if files are not present
void seedInitialData()
{
    // 1. Initial Menu Catalog (BST)
    drinkTree.insertMenu(40.0, "Iced Green Tea");
    drinkTree.insertMenu(40.0, "Iced Thai Tea");
    drinkTree.insertMenu(25.0, "Cold Cola");
    drinkTree.insertMenu(15.0, "Mineral Water");

    singleDishTree.insertMenu(55.0, "Basil Minced Pork Rice");
    singleDishTree.insertMenu(50.0, "Pork Fried Rice");
    singleDishTree.insertMenu(40.0, "Minced Pork Omelette Rice");
    singleDishTree.insertMenu(65.0, "Tom Yum Seafood Rice");

    soupTree.insertMenu(120.0, "Tom Yum Kung");
    soupTree.insertMenu(80.0, "Tofu Minced Pork Soup");
    soupTree.insertMenu(90.0, "Spicy Pork Rib Soup");

    friedTree.insertMenu(75.0, "Fried Chicken Wings");
    friedTree.insertMenu(50.0, "French Fries");
    friedTree.insertMenu(60.0, "Crispy Gyoza");

    // 2. Initial Tables: 5 tables (Dynamic Array)
    for (int i = 1; i <= 5; i++)
    {
        tables.insertAtEnd(Table(i));
    }

    // 3. Initial Admin Credentials (Dynamic Array)
    admins.insertAtEnd(Admin("admin", "1234"));
    admins.insertAtEnd(Admin("manager", "8888"));
}

// Save all persistent data to text files
void saveAllData()
{
    // 1. Save 4 Menu BSTs
    drinkTree.saveToFile(FILE_DRINKS);
    singleDishTree.saveToFile(FILE_DISHES);
    soupTree.saveToFile(FILE_SOUPS);
    friedTree.saveToFile(FILE_FRIED);

    // 2. Save Tables
    ofstream outTables(FILE_TABLES);
    if (outTables.is_open())
    {
        for (int i = 0; i < tables.getSize(); i++)
        {
            outTables << tables.getAt(i).tableNumber << "\n";
        }
        outTables.close();
    }

    // 3. Save Admins
    ofstream outAdmins(FILE_ADMINS);
    if (outAdmins.is_open())
    {
        for (int i = 0; i < admins.getSize(); i++)
        {
            outAdmins << admins.getAt(i).username << "|" << admins.getAt(i).password << "\n";
        }
        outAdmins.close();
    }

    // 4. Save Store Status and Revenue
    ofstream outStore(FILE_STORE);
    if (outStore.is_open())
    {
        outStore << (isStoreOpen ? 1 : 0) << "|" << fixed << setprecision(2) << totalDailyRevenue << "\n";
        outStore.close();
    }
}

// Load all persistent data from text files at startup
void loadAllData()
{
    ifstream checkFile(FILE_DRINKS);
    if (!checkFile.is_open())
    {
        seedInitialData();
        saveAllData();
        return;
    }
    checkFile.close();

    // 1. Load Menu BSTs
    drinkTree.loadFromFile(FILE_DRINKS);
    singleDishTree.loadFromFile(FILE_DISHES);
    soupTree.loadFromFile(FILE_SOUPS);
    friedTree.loadFromFile(FILE_FRIED);

    // 2. Load Tables
    tables.clear();
    ifstream inTables(FILE_TABLES);
    if (inTables.is_open())
    {
        string line;
        while (getline(inTables, line))
        {
            if (line.empty()) continue;
            try
            {
                int tNum = stoi(line);
                tables.insertAtEnd(Table(tNum));
            }
            catch (...) {}
        }
        inTables.close();
    }
    if (tables.getSize() == 0)
    {
        for (int i = 1; i <= 5; i++)
            tables.insertAtEnd(Table(i));
    }

    // 3. Load Admins
    admins.clear();
    ifstream inAdmins(FILE_ADMINS);
    if (inAdmins.is_open())
    {
        string line;
        while (getline(inAdmins, line))
        {
            if (line.empty()) continue;
            size_t sep = line.find('|');
            if (sep != string::npos)
            {
                string u = line.substr(0, sep);
                string p = line.substr(sep + 1);
                admins.insertAtEnd(Admin(u, p));
            }
        }
        inAdmins.close();
    }
    if (admins.getSize() == 0)
    {
        admins.insertAtEnd(Admin("admin", "1234"));
    }

    // 4. Load Store Status and Revenue
    ifstream inStore(FILE_STORE);
    if (inStore.is_open())
    {
        string line;
        if (getline(inStore, line))
        {
            size_t sep = line.find('|');
            if (sep != string::npos)
            {
                string statusStr = line.substr(0, sep);
                string revStr = line.substr(sep + 1);
                isStoreOpen = (statusStr == "1");
                try
                {
                    totalDailyRevenue = stod(revStr);
                }
                catch (...) {}
            }
        }
        inStore.close();
    }
}

// ==========================================
// Customer Modules
// ==========================================

// 1.1 View Menu Catalog (Reverse Inorder: High to Low)
void customerViewMenu()
{
    while (true)
    {
        clearScreen();
        cout << "============================================\n";
        cout << "           FOOD MENU CATALOG                \n";
        cout << "============================================\n";
        cout << "1. Drinks\n";
        cout << "2. Single Dishes\n";
        cout << "3. Soups & Curries\n";
        cout << "4. Fried & Snacks\n";
        cout << "5. View All Categories\n";
        cout << "0. Back to Customer Menu\n";
        cout << "--------------------------------------------\n";
        cout << "Select category to view [0-5]: ";

        int choice;
        if (!(cin >> choice))
        {
            clearInput();
            cout << "Invalid input! Please enter a number.\n";
            pauseConsole();
            continue;
        }

        if (choice == 0)
            break;

        if (choice >= 1 && choice <= 4)
        {
            clearScreen();
            string catName;
            BinarySearchTree *tree = getCategoryTree(choice, catName);
            cout << "============================================\n";
            cout << "Category: " << catName << "\n";
            cout << "(Sorted from Highest to Lowest Price via Reverse In-order)\n";
            cout << "--------------------------------------------\n";
            tree->displayPriceHighToLow();
            pauseConsole();
        }
        else if (choice == 5)
        {
            clearScreen();
            cout << "============================================\n";
            cout << "         COMPLETE MENU CATALOG              \n";
            cout << "============================================\n";
            for (int i = 1; i <= 4; i++)
            {
                string catName;
                BinarySearchTree *tree = getCategoryTree(i, catName);
                cout << "\n[" << catName << "]\n";
                tree->displayPriceHighToLow();
            }
            pauseConsole();
        }
        else
        {
            cout << "Invalid choice! Please try again.\n";
            pauseConsole();
        }
    }
}

// 1.2 Order Food to Kitchen Queue (Enqueue into Linked List)
void customerOrderFood(int tableNum)
{
    clearScreen();
    if (!isStoreOpen)
    {
        cout << "============================================\n";
        cout << "       [NOTICE] Store is CLOSED             \n";
        cout << "============================================\n";
        cout << "The restaurant is not currently taking orders.\n";
        cout << "[INFO] You are welcome to browse the menu catalog.\n";
        clearInput();
        pauseConsole();
        return;
    }

    int tableIdx = findTableIndex(tableNum);
    if (tableIdx == -1)
    {
        cout << "[ERROR] Table #" << tableNum << " not found in system!\n";
        pauseConsole();
        return;
    }

    while (true)
    {
        clearScreen();
        cout << "============================================\n";
        cout << "       Order Food for [Table #" << tableNum << "] \n";
        cout << "============================================\n";
        cout << "1. Order Drinks\n";
        cout << "2. Order Single Dishes\n";
        cout << "3. Order Soups & Curries\n";
        cout << "4. Order Fried & Snacks\n";
        cout << "0. Finish Ordering (Back to Customer Menu)\n";
        cout << "--------------------------------------------\n";
        cout << "Select category [0-4]: ";

        int catChoice;
        if (!(cin >> catChoice))
        {
            clearInput();
            cout << "Invalid input! Please enter a number.\n";
            pauseConsole();
            continue;
        }

        if (catChoice == 0)
            break;

        string catName;
        BinarySearchTree *tree = getCategoryTree(catChoice, catName);
        if (tree == nullptr)
        {
            cout << "Invalid category!\n";
            pauseConsole();
            continue;
        }

        Array<string> names;
        Array<double> prices;
        tree->getOrderedMenus(names, prices);

        if (names.getSize() == 0)
        {
            cout << "No items available in this category.\n";
            pauseConsole();
            continue;
        }

        clearScreen();
        cout << "============================================\n";
        cout << "Items in " << catName << " - [Table #" << tableNum << "]:\n";
        cout << "============================================\n";
        for (int i = 0; i < names.getSize(); i++)
        {
            cout << "  " << (i + 1) << ". " << left << setw(28) << names.getAt(i)
                 << " Price: $" << right << setw(6) << fixed << setprecision(2) << prices.getAt(i) << "\n";
        }
        cout << "  0. Back\n";
        cout << "--------------------------------------------\n";
        cout << "Select item number to order [1-" << names.getSize() << "]: ";

        int itemChoice;
        if (!(cin >> itemChoice) || itemChoice <= 0 || itemChoice > names.getSize())
        {
            clearInput();
            continue;
        }

        string chosenName = names.getAt(itemChoice - 1);
        double chosenPrice = prices.getAt(itemChoice - 1);

        cout << "Enter quantity [1-10]: ";
        int qty;
        if (!(cin >> qty) || qty <= 0)
        {
            qty = 1;
        }

        // Add to Kitchen Queue (Linked List)
        for (int q = 0; q < qty; q++)
        {
            int currentID = nextOrderID++;
            kitchenQueue.enqueueOrder(currentID, tableNum, chosenName, chosenPrice);
            cout << "  -> Order #" << currentID << " [" << chosenName << "] sent to kitchen queue!\n";
        }

        tables.getAt(tableIdx).isOccupied = true;
        cout << "\n[SUCCESS] Successfully queued " << qty << " item(s) to the kitchen for [Table #" << tableNum << "]!\n";
        pauseConsole();
    }
}

// 1.3 Check Table Order Status in Kitchen (My Table vs. Overall Restaurant Queue)
void customerCheckQueue(int tableNum)
{
    while (true)
    {
        clearScreen();
        cout << "============================================\n";
        cout << "       Kitchen Queue Status Monitor         \n";
        cout << "============================================\n";
        cout << "Current Table: [Table #" << tableNum << "]\n";
        cout << "--------------------------------------------\n";
        cout << "1. View My Table Queue (Table #" << tableNum << " Only)\n";
        cout << "2. View Overall Kitchen Queue (All Tables in Restaurant)\n";
        cout << "0. Back to Customer Menu\n";
        cout << "--------------------------------------------\n";
        cout << "Select option [0-2]: ";

        int choice;
        if (!(cin >> choice))
        {
            clearInput();
            cout << "Invalid input! Please enter a number.\n";
            pauseConsole();
            continue;
        }

        if (choice == 0)
            break;

        if (choice == 1)
        {
            clearScreen();
            kitchenQueue.displayTableOrders(tableNum);
            pauseConsole();
        }
        else if (choice == 2)
        {
            clearScreen();
            kitchenQueue.displayKitchenQueue();
            pauseConsole();
        }
        else
        {
            cout << "Invalid choice! Please try again.\n";
            pauseConsole();
        }
    }
}

// Customer Main Menu
void customerMenu()
{
    clearScreen();
    cout << "============================================\n";
    cout << "           Customer Table Selection         \n";
    cout << "============================================\n";
    cout << "Active Tables in Restaurant: ";
    for (int i = 0; i < tables.getSize(); i++)
    {
        cout << "[" << tables.getAt(i).tableNumber << "] ";
    }
    cout << "\n--------------------------------------------\n";
    cout << "Please enter your Table Number (or 0 to cancel): ";

    int tableNum;
    if (!(cin >> tableNum) || tableNum == 0)
    {
        clearInput();
        return;
    }

    int tableIdx = findTableIndex(tableNum);
    if (tableIdx == -1)
    {
        cout << "\n[ERROR] Table #" << tableNum << " not found in system!\n";
        clearInput();
        pauseConsole();
        return;
    }

    while (true)
    {
        clearScreen();
        cout << "============================================\n";
        cout << "     Customer Service Menu - [Table #" << tableNum << "]\n";
        cout << " Store Status: " << (isStoreOpen ? "[ OPEN ]" : "[ CLOSED (Orders Paused) ]") << "\n";
        cout << "============================================\n";
        cout << "1. View Menu Catalog (View Menu)\n";
        cout << "2. Order Food to Kitchen (Order Food)\n";
        cout << "3. Check Table Queue Status (Check Queue)\n";
        cout << "0. Back to Main Screen (Exit Table)\n";
        cout << "--------------------------------------------\n";
        cout << "Select option [0-3]: ";

        int choice;
        if (!(cin >> choice))
        {
            clearInput();
            cout << "Invalid input! Please enter a number.\n";
            pauseConsole();
            continue;
        }

        switch (choice)
        {
        case 1:
            customerViewMenu();
            break;
        case 2:
            customerOrderFood(tableNum);
            break;
        case 3:
            customerCheckQueue(tableNum);
            break;
        case 0:
            return;
        default:
            cout << "Invalid choice! Please try again.\n";
            pauseConsole();
        }
    }
}

// ==========================================
// Admin Modules
// ==========================================

// Admin Authentication Login
bool adminLogin()
{
    clearScreen();
    cout << "============================================\n";
    cout << "             Admin Staff Login              \n";
    cout << "============================================\n";
    cout << "Username: ";
    string username, password;
    cin >> username;
    cout << "Password: ";
    cin >> password;

    for (int i = 0; i < admins.getSize(); i++)
    {
        if (admins.getAt(i).username == username && admins.getAt(i).password == password)
        {
            cout << "\n[SUCCESS] Welcome, " << username << ", to the Admin Back-Office!\n";
            pauseConsole();
            return true;
        }
    }

    cout << "\n[ERROR] Invalid username or password!\n";
    clearInput();
    pauseConsole();
    return false;
}

// Admin: Menu Catalog Management (BST) - Accessible when store is CLOSED
void adminManageMenu()
{
    while (true)
    {
        clearScreen();
        cout << "============================================\n";
        cout << "        Menu Catalog Management (BST)       \n";
        cout << "============================================\n";
        cout << "1. Display Items (Highest to Lowest Price - Reverse Inorder)\n";
        cout << "2. Display Items (Lowest to Highest Price - Inorder)\n";
        cout << "3. Inspect BST Structure (Rotated 90 deg)\n";
        cout << "4. Add New Menu Item (Insert)\n";
        cout << "5. Edit Item Price (Relocate BST Node Key)\n";
        cout << "6. Delete Menu Item (Delete)\n";
        cout << "7. Search Item by Name (Search)\n";
        cout << "0. Back to Admin Panel\n";
        cout << "--------------------------------------------\n";
        cout << "Select operation [0-7]: ";

        int choice;
        if (!(cin >> choice))
        {
            clearInput();
            cout << "Invalid input! Please enter a number.\n";
            pauseConsole();
            continue;
        }

        if (choice == 0)
            break;

        if (choice >= 1 && choice <= 3)
        {
            cout << "Select Category [1=Drinks, 2=Dishes, 3=Soups, 4=Fried]: ";
            int cat;
            cin >> cat;
            string name;
            BinarySearchTree *tree = getCategoryTree(cat, name);
            if (tree == nullptr)
            {
                cout << "Invalid category!\n";
                pauseConsole();
                continue;
            }

            clearScreen();
            cout << "============================================\n";
            cout << "Category: " << name << " (Total " << tree->getTotalMenuCount() << " item(s), "
                 << tree->getNodeCount() << " price level(s))\n";
            cout << "============================================\n";

            if (choice == 1)
                tree->displayPriceHighToLow();
            else if (choice == 2)
                tree->displayPriceLowToHigh();
            else
                tree->printTree();

            pauseConsole();
        }
        else if (choice == 4)
        {
            cout << "Select Category to add item [1=Drinks, 2=Dishes, 3=Soups, 4=Fried]: ";
            int cat;
            cin >> cat;
            string catName;
            BinarySearchTree *tree = getCategoryTree(cat, catName);
            if (tree == nullptr)
            {
                cout << "Invalid category!\n";
                pauseConsole();
                continue;
            }

            cout << "Enter new item name: ";
            clearInput();
            string newMenu;
            getline(cin, newMenu);

            cout << "Enter price ($): ";
            double price;
            if (!(cin >> price) || price <= 0)
            {
                cout << "Invalid price!\n";
                clearInput();
                pauseConsole();
                continue;
            }

            tree->insertMenu(price, newMenu);
            saveAllData();
            cout << "[SUCCESS] Item \"" << newMenu << "\" ($" << fixed << setprecision(2) << price 
                 << ") added to " << catName << " successfully!\n";
            pauseConsole();
        }
        else if (choice == 5)
        {
            cout << "Select Category to edit price [1=Drinks, 2=Dishes, 3=Soups, 4=Fried]: ";
            int cat;
            cin >> cat;
            string catName;
            BinarySearchTree *tree = getCategoryTree(cat, catName);
            if (tree == nullptr)
            {
                cout << "Invalid category!\n";
                pauseConsole();
                continue;
            }

            cout << "Enter item name to change price: ";
            clearInput();
            string editMenu;
            getline(cin, editMenu);

            double currentPrice = 0.0;
            if (tree->searchMenu(editMenu, currentPrice))
            {
                cout << "[FOUND] \"" << editMenu << "\" Current Price: $" << fixed << setprecision(2) << currentPrice << "\n";
                cout << "Enter new price ($): ";
                double newPrice;
                if (cin >> newPrice && newPrice > 0)
                {
                    tree->deleteMenu(editMenu);
                    tree->insertMenu(newPrice, editMenu);
                    saveAllData();
                    cout << "[SUCCESS] Updated price of \"" << editMenu << "\" to $" 
                         << fixed << setprecision(2) << newPrice << " in BST successfully!\n";
                }
                else
                {
                    cout << "[ERROR] Invalid new price!\n";
                }
            }
            else
            {
                cout << "[ERROR] Item \"" << editMenu << "\" not found in this category!\n";
            }
            clearInput();
            pauseConsole();
        }
        else if (choice == 6)
        {
            cout << "Select Category to delete item [1=Drinks, 2=Dishes, 3=Soups, 4=Fried]: ";
            int cat;
            cin >> cat;
            string catName;
            BinarySearchTree *tree = getCategoryTree(cat, catName);
            if (tree == nullptr)
            {
                cout << "Invalid category!\n";
                pauseConsole();
                continue;
            }

            cout << "Enter item name to delete: ";
            clearInput();
            string delMenu;
            getline(cin, delMenu);

            if (tree->deleteMenu(delMenu))
            {
                saveAllData();
                cout << "[SUCCESS] Deleted \"" << delMenu << "\" from BST successfully!\n";
            }
            else
            {
                cout << "[ERROR] Item \"" << delMenu << "\" not found!\n";
            }
            pauseConsole();
        }
        else if (choice == 7)
        {
            cout << "Select Category to search [1=Drinks, 2=Dishes, 3=Soups, 4=Fried]: ";
            int cat;
            cin >> cat;
            string catName;
            BinarySearchTree *tree = getCategoryTree(cat, catName);
            if (tree == nullptr)
            {
                cout << "Invalid category!\n";
                pauseConsole();
                continue;
            }

            cout << "Enter item name to search: ";
            clearInput();
            string sMenu;
            getline(cin, sMenu);

            double foundPrice = 0.0;
            if (tree->searchMenu(sMenu, foundPrice))
            {
                cout << "[FOUND] \"" << sMenu << "\" in " << catName 
                     << " | Price: $" << fixed << setprecision(2) << foundPrice << "\n";
            }
            else
            {
                cout << "[ERROR] Item \"" << sMenu << "\" not found in system.\n";
            }
            pauseConsole();
        }
        else
        {
            cout << "Invalid choice! Please try again.\n";
            pauseConsole();
        }
    }
}

// Admin: Kitchen Dashboard Management (During OPEN hours)
void adminManageKitchen()
{
    while (true)
    {
        clearScreen();
        kitchenQueue.displayKitchenQueue();

        cout << "\n--- Kitchen Queue Operations (Linked List) ---\n";
        cout << "1. Serve Next Order (Dequeue Head -> Deliver & Bill to Table)\n";
        cout << "2. Serve Specific Order by Order ID (Deliver Ready Item Early)\n";
        cout << "3. Move Order to New Queue Position (Reposition / Rush Order)\n";
        cout << "4. Cancel Order by Order ID (Cancel Anywhere in Queue)\n";
        cout << "0. Back to Admin Panel\n";
        cout << "----------------------------------------------\n";
        cout << "Select operation [0-4]: ";

        int choice;
        if (!(cin >> choice))
        {
            clearInput();
            cout << "Invalid input! Please enter a number.\n";
            pauseConsole();
            continue;
        }

        if (choice == 0)
            break;

        if (choice == 1)
        {
            OrderNode served;
            if (kitchenQueue.serveNextOrder(served))
            {
                int tIdx = findTableIndex(served.tableNumber);
                if (tIdx != -1)
                {
                    tables.getAt(tIdx).addServedFood(served.menuName, served.price);
                }

                cout << "\n[KITCHEN] Next Order Cooked & Ready!\n";
                cout << "   -> Order ID: #" << served.orderID << "\n";
                cout << "   -> Item: " << served.menuName << " ($" << fixed << setprecision(2) << served.price << ")\n";
                cout << "   -> Delivered to: [Table #" << served.tableNumber << "] and recorded to bill!\n";
            }
            else
            {
                cout << "\n[NOTICE] No pending orders in the kitchen right now!\n";
            }
            pauseConsole();
        }
        else if (choice == 2)
        {
            if (kitchenQueue.isEmpty())
            {
                cout << "\n[NOTICE] No pending orders in the kitchen right now!\n";
                pauseConsole();
                continue;
            }

            cout << "Enter Order ID to serve: ";
            int serveID;
            if (cin >> serveID)
            {
                OrderNode served;
                if (kitchenQueue.serveOrderByID(serveID, served))
                {
                    int tIdx = findTableIndex(served.tableNumber);
                    if (tIdx != -1)
                    {
                        tables.getAt(tIdx).addServedFood(served.menuName, served.price);
                    }

                    cout << "\n[KITCHEN] Specific Order Cooked & Ready!\n";
                    cout << "   -> Order ID: #" << served.orderID << "\n";
                    cout << "   -> Item: " << served.menuName << " ($" << fixed << setprecision(2) << served.price << ")\n";
                    cout << "   -> Delivered to: [Table #" << served.tableNumber << "] and recorded to bill!\n";
                }
                else
                {
                    cout << "[ERROR] Order #" << serveID << " not found in kitchen queue!\n";
                }
            }
            clearInput();
            pauseConsole();
        }
        else if (choice == 3)
        {
            if (kitchenQueue.isEmpty())
            {
                cout << "\n[NOTICE] No pending orders in the kitchen right now!\n";
                pauseConsole();
                continue;
            }

            cout << "Enter Order ID to move: ";
            int moveID;
            if (cin >> moveID)
            {
                cout << "Enter new queue position [1-" << kitchenQueue.getSize() << "]: ";
                int newPos;
                if (cin >> newPos)
                {
                    if (kitchenQueue.moveOrderPosition(moveID, newPos))
                    {
                        cout << "\n[SUCCESS] Order #" << moveID << " successfully moved to queue position " << newPos << "!\n";
                    }
                    else
                    {
                        cout << "\n[ERROR] Failed to move order! Please check Order ID and position range (1-" << kitchenQueue.getSize() << ").\n";
                    }
                }
                else
                {
                    cout << "[ERROR] Invalid position number!\n";
                }
            }
            clearInput();
            pauseConsole();
        }
        else if (choice == 4)
        {
            cout << "Enter Order ID to cancel: ";
            int cancelID;
            if (cin >> cancelID)
            {
                OrderNode canceled;
                if (kitchenQueue.cancelOrder(cancelID, canceled))
                {
                    cout << "[SUCCESS] Order #" << canceled.orderID << " (" << canceled.menuName 
                         << " for Table #" << canceled.tableNumber << ") removed from kitchen queue!\n";
                }
                else
                {
                    cout << "[ERROR] Order #" << cancelID << " not found in kitchen queue!\n";
                }
            }
            clearInput();
            pauseConsole();
        }
        else
        {
            cout << "Invalid choice! Please try again.\n";
            pauseConsole();
        }
    }
}

// Admin: Billing & Checkout System (During OPEN hours)
void adminBillingSystem()
{
    while (true)
    {
        clearScreen();
        cout << "======================================================================\n";
        cout << "                    Table Billing System                              \n";
        cout << "======================================================================\n";
        cout << left << setw(10) << "Table #" 
             << setw(16) << "Status" 
             << setw(20) << "Served Items" 
             << setw(18) << "Kitchen Queue" 
             << right << setw(12) << "Bill Amount\n";
        cout << "----------------------------------------------------------------------\n";

        for (int i = 0; i < tables.getSize(); i++)
        {
            Table &t = tables.getAt(i);
            int pending = kitchenQueue.countOrdersByTable(t.tableNumber);
            cout << left << setw(10) << ("Table " + to_string(t.tableNumber))
                 << setw(16) << (t.isOccupied ? "Occupied" : "Available")
                 << setw(20) << (to_string(t.servedItems.getSize()) + " item(s)")
                 << setw(18) << (to_string(pending) + " item(s)")
                 << right << setw(10) << fixed << setprecision(2) << "$" << t.currentBill << "\n";
        }
        cout << "======================================================================\n";
        cout << "1. Select Table to Print Invoice & Checkout\n";
        cout << "0. Back to Admin Panel\n";
        cout << "----------------------------------------------------------------------\n";
        cout << "Select operation: ";

        int choice;
        if (!(cin >> choice))
        {
            clearInput();
            cout << "Invalid input! Please enter a number.\n";
            pauseConsole();
            continue;
        }

        if (choice == 0)
            break;

        if (choice == 1)
        {
            cout << "Enter Table Number to checkout: ";
            int tNum;
            if (!(cin >> tNum))
            {
                clearInput();
                continue;
            }

            int tIdx = findTableIndex(tNum);
            if (tIdx == -1)
            {
                cout << "[ERROR] Table not found!\n";
                pauseConsole();
                continue;
            }

            Table &t = tables.getAt(tIdx);

            // Safety Check: Any pending orders still in kitchen?
            int pending = kitchenQueue.countOrdersByTable(t.tableNumber);
            if (pending > 0)
            {
                cout << "\n[WARNING]: Table #" << tNum << " still has " << pending << " order(s) cooking in kitchen!\n";
                cout << "Proceed with checkout anyway? (y/n): ";
                char confirm;
                cin >> confirm;
                if (confirm != 'y' && confirm != 'Y')
                {
                    cout << "Checkout cancelled.\n";
                    pauseConsole();
                    continue;
                }
            }

            if (t.servedItems.getSize() == 0 && t.currentBill == 0.0)
            {
                cout << "Table has no served items and current bill is $0.00.\n";
                t.isOccupied = false;
                pauseConsole();
                continue;
            }

            clearScreen();
            // Print Receipt
            cout << "============================================\n";
            cout << "               SALES RECEIPT                \n";
            cout << "           Data Bistro Restaurant           \n";
            cout << "============================================\n";
            cout << "Table: #" << t.tableNumber << "\n";
            cout << "--------------------------------------------\n";
            cout << left << setw(6) << "No." << setw(28) << "Item" << right << setw(10) << "Price ($)\n";
            cout << "--------------------------------------------\n";

            for (int s = 0; s < t.servedItems.getSize(); s++)
            {
                cout << left << setw(6) << (s + 1)
                     << setw(28) << t.servedItems.getAt(s)
                     << right << setw(10) << fixed << setprecision(2) << t.itemPrices.getAt(s) << "\n";
            }
            cout << "--------------------------------------------\n";
            cout << "Total Amount: $" << right << setw(30) << fixed << setprecision(2) << t.currentBill << "\n";
            cout << "============================================\n";

            cout << "Cash Received ($): ";
            double pay;
            while (!(cin >> pay) || pay < t.currentBill)
            {
                cout << "[ERROR] Insufficient amount! Please re-enter (Min $" 
                     << fixed << setprecision(2) << t.currentBill << "): ";
                clearInput();
            }

            double change = pay - t.currentBill;
            cout << "Change: $" << fixed << setprecision(2) << change << "\n";
            cout << "[SUCCESS] Payment successful! Thank you!\n";

            totalDailyRevenue += t.currentBill;
            t.clearBill();
            saveAllData();

            pauseConsole();
        }
        else
        {
            cout << "Invalid choice! Please try again.\n";
            pauseConsole();
        }
    }
}

// Admin: Table Layout Management (Accessible when CLOSED)
void adminManageTables()
{
    while (true)
    {
        clearScreen();
        cout << "============================================\n";
        cout << "          Table Layout Management           \n";
        cout << "============================================\n";
        cout << "Current Total Tables: " << tables.getSize() << "\n";
        cout << "Layout: ";
        for (int i = 0; i < tables.getSize(); i++)
        {
            Table &t = tables.getAt(i);
            cout << "[T" << t.tableNumber << ": " << (t.isOccupied ? "Occupied" : "Free") << "] ";
        }
        cout << "\n--------------------------------------------\n";
        cout << "1. Add New Table (+1 Table)\n";
        cout << "2. Reduce Table (Remove Last Table)\n";
        cout << "3. Remove Table by Table Number\n";
        cout << "0. Back to Admin Panel\n";
        cout << "--------------------------------------------\n";
        cout << "Select operation [0-3]: ";

        int choice;
        if (!(cin >> choice))
        {
            clearInput();
            cout << "Invalid input! Please enter a number.\n";
            pauseConsole();
            continue;
        }

        if (choice == 0)
            break;

        if (choice == 1)
        {
            int maxNum = 0;
            for (int i = 0; i < tables.getSize(); i++)
            {
                if (tables.getAt(i).tableNumber > maxNum)
                    maxNum = tables.getAt(i).tableNumber;
            }
            int newTableNum = maxNum + 1;
            tables.insertAtEnd(Table(newTableNum));
            saveAllData();
            cout << "[SUCCESS] Added [Table #" << newTableNum << "] to layout successfully!\n";
            pauseConsole();
        }
        else if (choice == 2 || choice == 3)
        {
            int targetIdx = -1;
            if (choice == 2)
            {
                if (tables.getSize() <= 1)
                {
                    cout << "[ERROR] Cannot reduce tables: Restaurant must have at least 1 table!\n";
                    pauseConsole();
                    continue;
                }
                targetIdx = tables.getSize() - 1;
            }
            else
            {
                cout << "Enter Table Number to remove: ";
                int tNum;
                cin >> tNum;
                targetIdx = findTableIndex(tNum);
                if (targetIdx == -1)
                {
                    cout << "[ERROR] Table not found!\n";
                    pauseConsole();
                    continue;
                }
            }

            Table &target = tables.getAt(targetIdx);
            int targetNum = target.tableNumber;

            // Safety Check
            bool hasPendingKitchen = kitchenQueue.hasOrdersForTable(targetNum);
            bool hasServedItems = target.servedItems.getSize() > 0;
            bool hasBill = target.currentBill > 0.0;
            bool isOccupied = target.isOccupied;

            if (hasPendingKitchen || hasServedItems || hasBill || isOccupied)
            {
                cout << "\n[CANNOT REMOVE TABLE] Table #" << targetNum << " cannot be deleted because:\n";
                if (hasPendingKitchen)
                    cout << "   - Has pending cooking orders in kitchen queue!\n";
                if (hasServedItems || hasBill)
                    cout << "   - Has served items with unpaid bill ($" << fixed << setprecision(2) << target.currentBill << ")!\n";
                if (isOccupied)
                    cout << "   - Table currently has customers seated!\n";
                cout << "[INFO] Please clear the table and check out before deleting.\n";
            }
            else
            {
                tables.deleteAtIndex(targetIdx);
                saveAllData();
                cout << "[SUCCESS] Removed [Table #" << targetNum << "] from layout successfully!\n";
            }
            pauseConsole();
        }
        else
        {
            cout << "Invalid choice! Please try again.\n";
            pauseConsole();
        }
    }
}

// Admin: Open Store for Business
void adminOpenShop()
{
    clearScreen();
    if (isStoreOpen)
    {
        cout << "[INFO] Store is already OPEN!\n";
        pauseConsole();
        return;
    }

    if (tables.getSize() == 0)
    {
        cout << "[ERROR] Cannot open store: No tables in layout. Please add tables first!\n";
        pauseConsole();
        return;
    }

    isStoreOpen = true;
    saveAllData();
    cout << "============================================\n";
    cout << "      Store is now OPEN for Business!       \n";
    cout << "============================================\n";
    cout << "Status changed to: [ OPEN ]\n";
    cout << "Customers can now view menus and place orders.\n";
    pauseConsole();
}

// Admin: Close Store Daily & Summarize Revenue (Auto-serve & Auto-bill)
void adminCloseShop()
{
    clearScreen();
    cout << "============================================\n";
    cout << "             Daily Store Closing            \n";
    cout << "============================================\n";

    int pendingQueue = kitchenQueue.getSize();
    int activeTables = 0;
    for (int i = 0; i < tables.getSize(); i++)
    {
        if (tables.getAt(i).isOccupied || tables.getAt(i).currentBill > 0.0)
        {
            activeTables++;
        }
    }

    if (activeTables > 0 || pendingQueue > 0)
    {
        cout << "[NOTICE] Pre-closing pending activities detected:\n";
        if (pendingQueue > 0)
            cout << "   - " << pendingQueue << " item(s) pending in kitchen queue\n";
        if (activeTables > 0)
            cout << "   - " << activeTables << " table(s) currently active / seated\n";
        cout << "\nClosing store will automatically:\n";
        cout << "   1. Auto-serve all pending kitchen orders to their tables\n";
        cout << "   2. Auto-bill and checkout all active tables\n";
        cout << "   3. Finalize and record all sales to Total Daily Revenue\n";
        cout << "--------------------------------------------\n";
        cout << "Proceed with store closing? (y/n): ";
        char confirm;
        cin >> confirm;
        if (confirm != 'y' && confirm != 'Y')
        {
            cout << "Store closing cancelled.\n";
            clearInput();
            pauseConsole();
            return;
        }
    }

    // 1. Auto-serve all pending orders in kitchen queue to their respective tables
    int autoServedCount = 0;
    OrderNode served;
    while (kitchenQueue.serveNextOrder(served))
    {
        int tIdx = findTableIndex(served.tableNumber);
        if (tIdx != -1)
        {
            tables.getAt(tIdx).addServedFood(served.menuName, served.price);
            tables.getAt(tIdx).isOccupied = true;
            autoServedCount++;
        }
    }

    // 2. Auto-bill all active tables and accumulate into total daily revenue
    int autoBilledTables = 0;
    double autoBilledRevenue = 0.0;
    for (int i = 0; i < tables.getSize(); i++)
    {
        Table &t = tables.getAt(i);
        if (t.isOccupied || t.currentBill > 0.0)
        {
            autoBilledTables++;
            autoBilledRevenue += t.currentBill;
            totalDailyRevenue += t.currentBill;
            t.clearBill();
        }
    }

    // 3. Mark store as CLOSED and save persistent data
    isStoreOpen = false;
    saveAllData();

    // 4. Print final daily sales & closing report
    cout << "\n======================================================\n";
    cout << "         Daily Sales Summary & Closing Report         \n";
    cout << "======================================================\n";
    cout << "Auto-served from Kitchen:  " << autoServedCount << " item(s) -> ALL SERVED\n";
    cout << "Auto-billed Active Tables: " << autoBilledTables << " table(s) (+$" 
         << fixed << setprecision(2) << autoBilledRevenue << ")\n";
    cout << "Reset All Tables:          " << tables.getSize() << " table(s) -> ALL FREE\n";
    cout << "------------------------------------------------------\n";
    cout << "FINAL TOTAL DAILY REVENUE: $" << fixed << setprecision(2) << totalDailyRevenue << "\n";
    cout << "======================================================\n";
    cout << "Store status changed to:   [ CLOSED ]\n";
    cout << "[INFO] Entered Back-office Maintenance Mode (Admins can configure menu & tables).\n";
    clearInput();
    pauseConsole();
}

// Admin: View Accumulated Revenue Report (When CLOSED)
void adminViewDailyReport()
{
    clearScreen();
    cout << "============================================\n";
    cout << "         Daily Revenue & Store Report       \n";
    cout << "============================================\n";
    cout << "Current Accumulated Revenue: $" << fixed << setprecision(2) << totalDailyRevenue << "\n";
    cout << "Total Tables in Restaurant: " << tables.getSize() << " tables\n";
    cout << "Kitchen Queue Status: Empty (No pending orders)\n";
    cout << "============================================\n";
    pauseConsole();
}

// Admin: Credentials Management (When CLOSED)
void adminManageAccounts()
{
    while (true)
    {
        clearScreen();
        cout << "============================================\n";
        cout << "        Admin Credentials Management        \n";
        cout << "============================================\n";
        cout << "Active Admin Accounts:\n";
        for (int i = 0; i < admins.getSize(); i++)
        {
            cout << "  " << (i + 1) << ". " << admins.getAt(i).username << "\n";
        }
        cout << "--------------------------------------------\n";
        cout << "1. Add New Admin Account\n";
        cout << "2. Change Account Password\n";
        cout << "0. Back to Admin Panel\n";
        cout << "--------------------------------------------\n";
        cout << "Select operation [0-2]: ";

        int choice;
        if (!(cin >> choice))
        {
            clearInput();
            cout << "Invalid input! Please enter a number.\n";
            pauseConsole();
            continue;
        }

        if (choice == 0)
            break;

        if (choice == 1)
        {
            cout << "Enter new Username: ";
            string u, p;
            cin >> u;
            cout << "Enter new Password: ";
            cin >> p;
            admins.insertAtEnd(Admin(u, p));
            saveAllData();
            cout << "[SUCCESS] Admin account \"" << u << "\" added successfully!\n";
            pauseConsole();
        }
        else if (choice == 2)
        {
            cout << "Enter Username to change password: ";
            string u;
            cin >> u;
            int foundIdx = -1;
            for (int i = 0; i < admins.getSize(); i++)
            {
                if (admins.getAt(i).username == u)
                {
                    foundIdx = i;
                    break;
                }
            }

            if (foundIdx != -1)
            {
                cout << "Enter new password: ";
                string newPass;
                cin >> newPass;
                admins.getAt(foundIdx).password = newPass;
                saveAllData();
                cout << "[SUCCESS] Password for \"" << u << "\" updated successfully!\n";
            }
            else
            {
                cout << "[ERROR] Account not found!\n";
            }
            pauseConsole();
        }
        else
        {
            cout << "Invalid choice! Please try again.\n";
            pauseConsole();
        }
    }
}

// Admin Panel Main Router
void adminMenu()
{
    if (!adminLogin())
        return;

    while (true)
    {
        clearScreen();
        cout << "======================================================\n";
        cout << "                Admin Staff Panel                     \n";
        if (isStoreOpen)
        {
            cout << "   Current Status: [ OPEN for Business ]\n";
            cout << "   (Active Service Hours: Kitchen & Billing Management)\n";
            cout << "======================================================\n";
            cout << "1. Kitchen Queue Dashboard (Kitchen Dashboard)\n";
            cout << "2. Table Billing & Invoicing (Billing System)\n";
            cout << "3. Close Store & Sales Summary (Close Shop)\n";
            cout << "0. Logout Admin\n";
            cout << "------------------------------------------------------\n";
            cout << "Select option [0-3]: ";

            int choice;
            if (!(cin >> choice))
            {
                clearInput();
                cout << "Invalid input! Please enter a number.\n";
                pauseConsole();
                continue;
            }

            switch (choice)
            {
            case 1:
                adminManageKitchen();
                break;
            case 2:
                adminBillingSystem();
                break;
            case 3:
                adminCloseShop();
                break;
            case 0:
                cout << "Logged out from Admin Panel.\n";
                pauseConsole();
                return;
            default:
                cout << "Invalid choice! Please try again.\n";
                pauseConsole();
            }
        }
        else
        {
            cout << "   Current Status: [ CLOSED - Maintenance Mode ]\n";
            cout << "   (Back-Office Hours: Menu, Table & Account Configuration)\n";
            cout << "======================================================\n";
            cout << "1. Open Store for Business (Open Shop)\n";
            cout << "2. Manage Menu Catalog (BST: Add / Delete / Edit Price)\n";
            cout << "3. Manage Table Layout (Add / Reduce Tables)\n";
            cout << "4. Manage Admin Credentials (Admins)\n";
            cout << "5. View Daily Revenue Report (Revenue)\n";
            cout << "0. Logout Admin\n";
            cout << "------------------------------------------------------\n";
            cout << "Select option [0-5]: ";

            int choice;
            if (!(cin >> choice))
            {
                clearInput();
                cout << "Invalid input! Please enter a number.\n";
                pauseConsole();
                continue;
            }

            switch (choice)
            {
            case 1:
                adminOpenShop();
                break;
            case 2:
                adminManageMenu();
                break;
            case 3:
                adminManageTables();
                break;
            case 4:
                adminManageAccounts();
                break;
            case 5:
                adminViewDailyReport();
                break;
            case 0:
                cout << "Logged out from Admin Panel.\n";
                pauseConsole();
                return;
            default:
                cout << "Invalid choice! Please try again.\n";
                pauseConsole();
            }
        }
    }
}

// ==========================================
// Application Entry Point (Main)
// ==========================================
int main()
{
    // Load data from text files (or seed and create files if first run)
    loadAllData();

    while (true)
    {
        clearScreen();
        cout << "============================================\n";
        cout << "       Welcome to Data Bistro Restaurant    \n";
        cout << "      (Smart POS & Kitchen Queue System)    \n";
        cout << "   Store Status: " << (isStoreOpen ? "[ OPEN ]" : "[ CLOSED ]") << "\n";
        cout << "============================================\n";
        cout << "1. Customer Mode (Browse & Order)\n";
        cout << "2. Admin Mode (Staff Login)\n";
        cout << "0. Exit & Save\n";
        cout << "--------------------------------------------\n";
        cout << "Please select mode [0-2]: ";

        int choice;
        if (!(cin >> choice))
        {
            clearInput();
            cout << "Please enter numbers only!\n";
            pauseConsole();
            continue;
        }

        switch (choice)
        {
        case 1:
            customerMenu();
            break;
        case 2:
            adminMenu();
            break;
        case 0:
            clearScreen();
            saveAllData();
            cout << "============================================\n";
            cout << "[SAVED] All data and catalogs successfully saved to text files!\n";
            cout << "Thank you for using Data Bistro Restaurant. Have a great day!\n";
            cout << "============================================\n";
            return 0;
        default:
            cout << "Invalid choice! Please try again.\n";
            pauseConsole();
        }
    }

    return 0;
}
