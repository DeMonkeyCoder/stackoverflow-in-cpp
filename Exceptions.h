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

class InvalidUsernameException : public std::exception {
public:
    const char *what() const throw() {
        return message.c_str();
    }

private:
    const std::string message = "Error: invalid username\n"
                                "You can use a-z, 0-9 and underscores\n"
                                "Username must be at least 5 and at most 32 characters";
};

class InvalidEmailException : public std::exception {
public:
    const char *what() const throw() {
        return message.c_str();
    }

private:
    const std::string message = "Error: invalid email address";
};

class WrongUsernameOrPasswordException : public std::exception {
public:
    const char *what() const throw() {
        return message.c_str();
    }

private:
    const std::string message = "Error: wrong username or password!";
};

class DeleteAdminException : public std::exception {
public:
    const char *what() throw() {
        return message.c_str();
    }

private:
    const std::string message = "Error: can't delete admin account!";
};
