//
// Created by spsina on 11/8/18.
//

#ifndef STACKOVERFLOW_IN_CPP1_USER_H
#define STACKOVERFLOW_IN_CPP1_USER_H

#include "AbstractUser.h"

class User : public AbstractUser {
public:
    User(string username, string password, UserType type);
public:
    void set_password(string password);
    bool check_password(string password);

public:
    bool authenticate(string username, string password);
    void deleteAccount(vector<AbstractUser*> *users);

public:
    static User* login(vector<AbstractUser*> *users, string username, string password);
    static void signup(vector<AbstractUser*> *users, string username, string password);
private:
    const string salt = "E1F53135E559C253";
};

#endif //STACKOVERFLOW_IN_CPP1_USER_H
