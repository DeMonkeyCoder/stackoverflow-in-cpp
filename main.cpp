#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>
using namespace std;
#define lower(str) transform(str.begin(), str.end(), str.begin(), ::tolower)

/**
 * In the name of God
 * Homework 2
 * TODO: Complete this code using given TODO comments :). then remove TODOs
 * feel free to edit code as you want and make it better
 * Any questions? ask @devcom_support on telegram
 * Good luck!
 **/

enum UserType {
    ADMIN,
    MEMBER
};

class UserAlreadyExistsException : public exception {
private:
    const string message = "Error: username already exists!";
public:
    const string what() {
        return message;
    }
};

class WrongUsernameOrPasswordException : public exception {
private:
    const string message = "Error: wrong username or password!";
public:
    const string what() {
        return message;
    }
};

class AbstractUser { // User structure
public:
    virtual bool authenticate(string username, string password) = 0;
    virtual void deleteAccount(vector<AbstractUser*> *users) = 0;
    string username;
protected:
    string password;
    UserType type;
};

class User : public AbstractUser {
public:
    User(string username, string password, UserType type) {
        lower(username);
        this->username = username;
        this->password = password;
        this->type = type;
    }

    bool authenticate(string username, string password) {
        lower(username);
        return this->username == username and this->password == password;
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
        for(auto user = users->begin(); user != users->end(); user++) {
            if((*user)->username == username) {
                UserAlreadyExistsException ex;
                throw ex;
            }
        }
        //Create user and add it to vector
        users->push_back(new User(username, password, UserType::MEMBER));
    }

    void deleteAccount(vector<AbstractUser*> *users) {
        for(auto user = users->begin(); user != users->end(); user++) {
            if((*user)->username == this->username) {
                users->erase(user);
                break;
            }
        }
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
                                    "admin", /* password is unsafe! for test only */
                                    UserType::ADMIN)
        );
    }
};

int main() {
    User * loggedInUser = nullptr;
    AppDatabase appDatabase;
    MenuState menuState = MenuState::START;
    string last_message = "";
    char choice;
    while(menuState != MenuState::END) {
        switch(menuState){
            case MenuState::START: {
                system("clear");
                if(last_message != "")
                    cout << last_message << endl;
                last_message = "";
                cout << "1. login\n2. signup\ne. exit\n";
                cin >> choice;
                switch(choice) {
                    case '1': { // login
                        string username;
                        cout << "Enter Username: ";
                        cin >> username;
                        char* pass = getpass("Enter password: ");
                        string password(pass);
                        try {
                            loggedInUser = User::login(&appDatabase.appUsers, username, password);
                        } catch(WrongUsernameOrPasswordException e) {
                            last_message = e.what();
                            break;
                        }
                        menuState = MenuState::LOGGED_IN;
                        break;
                    }
                    case '2': { // signup
                        string username;
                        cout << "Enter Username: ";
                        cin >> username;
                        char* pass = getpass("Enter password: ");
                        string password(pass);
                        try {
                            User::signup(&appDatabase.appUsers, username, password);
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
                        last_message = "Unknown Input";
                    }
                }
                break;
            }
            case MenuState::LOGGED_IN: {
                system("clear");
                cout << "d.delete account\nl. logout\ne. exit\n";
                cin >> choice;
                switch(choice) {
                    case 'd': { // delete account
                        loggedInUser->deleteAccount(&appDatabase.appUsers);
                        last_message = "Account successfully deleted";
                        loggedInUser = nullptr;
                        menuState = MenuState::START;
                        break;
                    }
                    case 'l': { // logout
                        loggedInUser = nullptr;
                        menuState = MenuState::START;
                        break;
                    }
                    case 'e': { // exit
                        menuState = MenuState::END;
                        break;
                    }
                    default: { // unknown input
                        last_message = "Unknown Input";
                    }
                }
                break;
            }
        }
    }

    return 0;

}
