#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <string>

using namespace std;

// Admin user credentials
struct Admin
{
    string username; // User name
    string password; // Password

    Admin()
    {
        username = "";
        password = "";
    }

    Admin(string u, string p)
    {
        username = u;
        password = p;
    }

    bool operator==(const Admin &other) const
    {
        return username == other.username && password == other.password;
    }
};

inline ostream& operator<<(ostream& os, const Admin& a)
{
    os << "[Admin: " << a.username << "]";
    return os;
}

#endif
