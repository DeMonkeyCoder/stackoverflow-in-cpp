//
// Created by spsina on 11/8/18.
//

#ifndef STACKOVERFLOW_IN_CPP1_USER_H
#define STACKOVERFLOW_IN_CPP1_USER_H

#include <iostream>
#include "AbstractUser.h"

class User : public AbstractUser {
public:
    User(string username, string password, UserType type);

    static void init(const string &salt);

public:
    void set_password(string password);
    bool check_password(string password);

public:
    bool authenticate(string username, string password);
    void deleteAccount();

public:
    static User& login(string username, string password);
    static User& signup(string username, string password);

private:
    static string salt;
    static vector<User> appDatabase;

};

#endif //STACKOVERFLOW_IN_CPP1_USER_H
