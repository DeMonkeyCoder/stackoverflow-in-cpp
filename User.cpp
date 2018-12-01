#include <utility>

#include <sstream>
#include "User.h"
#include "Exceptions.h"
#include <iostream>
#include <regex>

bool is_username_valid(const string& username) {
    const std::regex pattern("\\w{5,32}");
    return std::regex_match(username, pattern);
}

bool is_email_valid(const string& email) {
    const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return std::regex_match(email, pattern);
}

vector<User> User::users;
string User::salt;


User::User(string username, string password, string email, UserType type){
    lower(username);
    this->username = username;
    set_password(std::move(password));
    this->email = email;
    this->type = type;
}

void User::set_password(string password){
    size_t ps = pass_hash(password + salt);
    stringstream out;
    out << ps;
    this->password = out.str();
}

bool User::check_password(string password){
    size_t check = pass_hash(password + salt);
    stringstream out;
    out << check;
    return (this->password == out.str());
}

bool User::authenticate(string username, string password){
    lower(username);
    return this->username == username and check_password(password);
}
void User::deleteAccount(){
    if (this->type == UserType::ADMIN) {
        throw DeleteAdminException();
    }

    for (auto user = users.begin(); user != users.end();user++){
        if ( user->username == this->username  ) {
            users.erase(user);
            break;
        }
    }
}

User& User::login(string username, string password){
    for (auto &user : users) {
        if(user.authenticate(username, password)) {
            return user;
        }
    }
    throw WrongUsernameOrPasswordException();
}

User& User::signup(string username, string password, string email){
    for (auto &user : users) {
        if (user.username == username) {
            throw UsernameAlreadyExistsException();
        }
        else if (user.email == email) {
            throw EmailAlreadyExistsException();
        }
    }
    // Check username validatin
    if(not is_username_valid(username))
        throw InvalidUsernameException();

    // Check email validatin
    if(not is_email_valid(email))
        throw InvalidEmailException();

    // Create user
    users.emplace_back(username, password, email, UserType::MEMBER);
    return users[users.size() - 1];
}

void User::init(const string &salt) {
    User::salt = salt;
    users.reserve(20);
    users.emplace_back("admin", "admin", "admin@stackoverflow.com", UserType::ADMIN);
}

bool User::is_admin() {
    return this->type == UserType::ADMIN;
}

string User::toString() {
    return email + " | " + username;
}

std::ostream& operator<<(std::ostream &os, User &user) {
    os << user.toString();
    return os;
}

void User::create(std::string &body, ContentType type) {
    contents.emplace_back( body, type);
}

void User::print_questions() {
    for(const auto &user : users) {
        for(const auto &content : user.contents) {
            if(content.type == ContentType::QUESTION) {
                std::cout << user.username << ": " << content.body << std::endl << std::endl;
            }
        }
    }
}
