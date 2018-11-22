#pragma once

#include <iostream>
#include "AbstractUser.h"

class User : public AbstractUser {
public:
    User(string username, string password, string email, UserType type);

    static void init(const string &salt);

public:
    void set_password(string password);
    bool check_password(string password);

public:
    bool authenticate(string username, string password);
    void deleteAccount();

public:
    static User& login(string username, string password);
    static User& signup(string username, string password, string email);

public:
    bool is_admin();

public:
    string toString();
    friend std::ostream& operator<<(std::ostream& os, User& user);
private:
    static string salt;
    static vector<User> users;

};
