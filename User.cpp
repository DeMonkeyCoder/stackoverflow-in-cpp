//
// Created by spsina on 11/8/18.
//

#include <sstream>
#include "User.h"
#include "Exceptions.h"
#include <iostream>

using namespace Exceptions;

vector<User*> User::appDatabase;

User::User(string username, string password, UserType type){
    lower(username);
    this->username = username;
    set_password(password);
    this->type = type;
}
void User::set_password(string password){
    long long ps = pass_hash(password);
    string a;
    stringstream out;
    out << ps;
    a = out.str();
    this->password = a;
}
bool User::check_password(string password){
    hash<string> pass_hash_c;
    long long check = pass_hash(password);
    string a;
    stringstream out;
    out << check;
    a = out.str();
    return (this->password == a);
}
bool User::authenticate(string username, string password){
    lower(username);
    return this->username == username and check_password(password);
}
void User::deleteAccount(){
    if (this->type == UserType::ADMIN) {
        DeleteAdminException ex;
        throw ex;
    }

    for (auto user = appDatabase.begin(); user != appDatabase.end();user++){
        if ( (*user)->username == this->username  ) {
            appDatabase.erase(user);
            break;
        }
    }
}
User* User::login(string username, string password){
    for(auto user = appDatabase.begin(); user != appDatabase.end(); user++) {
        if((*user)->authenticate(username, password)) {
            return (User*) *user;
        }
    }
    WrongUsernameOrPasswordException ex;
    throw ex;
}
void User::signup(string username, string password){
    for (auto user = appDatabase.begin(); user != appDatabase.end(); user++) {
        if ((*user)->username == username) {
            UserAlreadyExistsException ex;
            throw ex;
        }
    }
    //Create user and add it to vector
    appDatabase.push_back(new User(username, password, UserType::MEMBER));
}
