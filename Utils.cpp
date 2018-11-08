//
// Created by spsina on 11/8/18.
//

#include "Utils.h"
#include <iostream>
#include <string>
#include "User.h"

User *Utils::loginUser() {
    std::string username;
    std::cout << "Enter Username: ";
    std::cin >> username;
    //char *pass = getpass("Enter password: ");
    std::string password;
    std::cout << "Enter Password: ";
    std::cin >> password;
    return User::login(username,password);
}

void Utils::signUp() {
    std::string username;
    std::cout << "Enter Username: ";
    std::cin >> username;
    //char *pass = getpass("Enter password: ");
    std::string password;
    std::cout << "Enter Password: ";
    std::cin >> password;
    User::signup(username, password);
}
