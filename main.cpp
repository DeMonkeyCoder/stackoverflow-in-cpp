#include <iostream>
#include <vector>
#include <functional>
#include <sstream>
#include <unistd.h>
#include "myHash.h"
#include <string>
#include <algorithm>
#include <unistd.h>
#include "md5.h"
#include<stdlib.h>
#include <string>
#include <exception>


using namespace std;
#define lower(str) transform(str.begin(), str.end(), str.begin(), ::tolower)

string md5_hash(string data) {
	string data_hex_digest;
	class md5 hash;
	hash.update(data.begin(), data.end());
    hash.hex_digest(data_hex_digest);
	return data_hex_digest;
}

enum UserType {
    ADMIN,
    MEMBER
};
/**
* In the name of God
* Homework 2
**/


class UserAlreadyExistsException:public exception{
public:
    const char* what() const throw(){
        return message.c_str();
    }
private:
    const string message = "Error: User already exists";
};

class WrongUsernameOrPasswordException : public exception {
private:
    const string message = "Error: wrong username or password!";
public:
    const char*  what() const throw (){
        return message.c_str();
    }
};

class DeleteAdminException : public exception {
public:
    const char*  what() throw(){
        return message.c_str();
    }
private:
    const string message = "Error: can't delete admin account!";

};


class AbstractUser { // User structure
public:
    hash<string> pass_hash;
	virtual bool authenticate(string username, string password) = 0;
	virtual void deleteAccount(vector<AbstractUser*> *users) = 0;
	string username;
protected:
	string password;
	UserType type;
};

class User : public AbstractUser {
private:
    const string salt = "E1F53135E559C253";
public:
    User(string username, string password, UserType type) {
        lower(username);
        this->username = username;
        set_password(password);
        this->type = type;
    }
    void set_password(string password){
        long long ps = pass_hash(password);
        string a;
        stringstream out;
        out << ps;
        a = out.str();
        this->password = a;
    }

    bool check_password(string password){
        hash<string> pass_hash_c;
        long long check = pass_hash(password);
        string a;
        stringstream out;
        out << check;
        a = out.str();
        return (this->password == a);
    }
    bool authenticate(string username, string password) {
        lower(username);
        return this->username == username and check_password(password);
    }
    void deleteAccount(vector<AbstractUser*> *users){
        if (this->type == UserType::ADMIN) {
            DeleteAdminException ex;
            throw ex;
        }

        for (auto user = users->begin(); user != users->end();user++){
            if ((*user)->username == this->username) {
                users->erase(user);
                break;
            }
        }
    }

    static User* login(vector<AbstractUser*> *users, string username, string password) {
        for(auto user = users->begin(); user != users->end(); user++) {
            if((*user)->authenticate(username, password)) {
                return (User*) *user;
            }
        }
        WrongUsernameOrPasswordException ex;
        throw ex;
    }

    static void signup(vector<AbstractUser*> *users, string username, string password) {
        for (auto user = users->begin(); user != users->end(); user++) {
            if ((*user)->username == username) {
                UserAlreadyExistsException ex;
                throw ex;
            }
        }
        //Create user and add it to vector
        users->push_back(new User(username, password, UserType::MEMBER));
    }
};

enum MenuState {
    START,
    LOGGED_IN,
    END
};


class AppDatabase { //Just holds runtime data. doesn't save anything
public:
    vector<AbstractUser*> appUsers;

    AppDatabase() { //Load initial data
        appUsers.push_back(new User("admin",
                                    "admin" /* password is unsafe! for test only */,
                                    UserType::ADMIN)
        );
    }
    
};


int main() {
    User * loggedInUser = nullptr;
    AppDatabase appDatabase;
    MenuState menuState = MenuState::START;
    string last_message = "";
    //User users(&appDatabase.appUsers);
    char choice;
    while(menuState != MenuState::END) {
        system("clear");
        if (last_message != "")
            cout << last_message << endl;
        last_message = "";
        switch (menuState) {
            case MenuState::START: {
                cout << "1. login\n2. signup\ne. exit\n";
                cin >> choice;
                switch (choice) {
                    case '1': { // login
                        string username;
                        cout << "Enter Username: ";
                        cin >> username;
                        //char *pass = getpass("Enter password: ");
                        string password;
                        cout << "Enter Password: ";
                        cin >> password;
                        try {
                            loggedInUser = User::login(&appDatabase.appUsers, username, password);
                            menuState = MenuState::LOGGED_IN;
                        } catch (WrongUsernameOrPasswordException e) {
                            last_message = e.what();
                            loggedInUser = nullptr;
                        }
                        break;
                    }
                    case '2': { // signup
                        string username;
                        cout << "Enter Username: ";
                        cin >> username;
                        //char *pass = getpass("Enter password: ");
                        string password;
                        cout << "Enter Password: ";
                        cin >> password;
                        try {
                            User::signup(&appDatabase.appUsers, username, password);
                            last_message = "\n\nUser signed up!\n\n";
                        } catch (UserAlreadyExistsException e) {
                            last_message = e.what();
                        }
                        break;
                    }
                    case 'e': { // exit
                        menuState = MenuState::END;
                        break;
                    }
                    default: { // unknown input
                        last_message = "\n\nUnknown Input\n\n";
                        break;
                    }
                }
                break;
            }
            case MenuState::LOGGED_IN: {
                cout << "d.delete account\nl. logout\ne. exit\n";
                cin >> choice;
                switch (choice) {
                    case 'd': {
                        try {
                            loggedInUser->deleteAccount(&appDatabase.appUsers);
                            cout << "\n\nAccount successfully deleted\n\n";
                            loggedInUser = nullptr;
                            menuState = MenuState::START;
                        }
                        catch (DeleteAdminException e) {
                            last_message = e.what();
                        }
                        break;
                    }
                    case 'l': { // logout
                        loggedInUser = nullptr;
                        menuState = MenuState::START;
                        last_message = "\n\nGOOD BYE\n\n";
                        break;
                    }
                    case 'e': { // exit
                        menuState = MenuState::END;
                        last_message = "\n\nGoodbye\n\n";
                        break;
                    }
                    default: { // unknown input
                        last_message = "\n\nUnknown Input\n\n";
                        break;
                    }

                }
            }
        }
    }
    return 0;
}
