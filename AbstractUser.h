//
// Created by spsina on 11/8/18.
//

#ifndef STACKOVERFLOW_IN_CPP1_ABSTRACTUSER_H
#define STACKOVERFLOW_IN_CPP1_ABSTRACTUSER_H
#include <algorithm>
#include <string>
#include <vector>
#define lower(str) transform(str.begin(), str.end(), str.begin(), ::tolower)
using namespace std;

enum UserType {
    ADMIN,
    MEMBER
};

class AbstractUser {
public:
    hash<string> pass_hash;
    virtual bool authenticate(string username, string password) = 0;
    virtual void deleteAccount() = 0;
    string username;
protected:
    string password;
    UserType type;
};

#endif //STACKOVERFLOW_IN_CPP1_ABSTRACTUSER_H
