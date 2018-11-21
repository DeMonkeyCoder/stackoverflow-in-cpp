#pragma once

#include <exception>
#include <string>

class UsernameAlreadyExistsException : public std::exception {
public:
    const char *what() const throw() {
        return message.c_str();
    }

private:
    const std::string message = "Error: username already exists";

};

class EmailAlreadyExistsException : public std::exception {
public:
    const char *what() const throw() {
        return message.c_str();
    }

private:
    const std::string message = "Error: email already exists";

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
