//
// Created by spsina on 11/8/18.
//

#ifndef STACKOVERFLOW_IN_CPP1_EXCEPTIONS_H
#define STACKOVERFLOW_IN_CPP1_EXCEPTIONS_H

#include <exception>
#include <string>

class UserAlreadyExistsException : public std::exception {
public:
    const char *what() const throw() {
        return message.c_str();
    }

private:
    const std::string message = "Error: User already exists";

};

class WrongUsernameOrPasswordException : public std::exception {
private:
    const std::string message = "Error: wrong username or password!";
public:
    const char *what() const throw() {
        return message.c_str();
    }
};

class DeleteAdminException : public std::exception {
public:
    const char *what() throw() {
        return message.c_str();
    }

private:
    const std::string message = "Error: can't delete admin account!";

};

#endif //STACKOVERFLOW_IN_CPP1_EXCEPTIONS_H
