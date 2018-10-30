#include <iostream>
#include <vector>
#include <exception>

using namespace std;

/**
 * In the name of God
 * Homework 2
 * TODO: Complete this code using given TODO comments :). then remove TODOs
 * feel free to edit code as you want and make it better
 * Any questions? ask @devcom_support on telegram
 * Good luck!
 **/

enum UserType{
    ADMIN,
    MEMBER
};


class UserAlreadyExistsException: public exception{
    const char* exceptionMessage = "The user exists!";
public:
    virtual const char* getMessage() const throw()
    {
        return exceptionMessage;
    }
};

class UserNotFoundException: public exception{
    const char* exceptionMessage = "couldn't login with given credentials.";
public:
    virtual const char* getMessage() const throw()
    {
        return exceptionMessage;
    }
};

class DeleteAccountException: public exception{
    const char* exceptionMessage = "couldn't delete your account. sry!";
public:
    virtual const char* getMessage() const throw()
    {
        return exceptionMessage;
    }
};

// we do the hash things here. This can be refined later!
class PHash{
    string salt = "";
public:
    PHash(string salt):salt(salt){}
    
    string doHash(string str){
        hash <string> hash;
        return to_string(hash(str));
    }
};

PHash phash("");

class AbstractUsers{
public:
    virtual UserType login(string username, string password) = 0;
    virtual void deleteAccount(string username) = 0;
    virtual void signup(string username, string password) = 0;
};

class AbstractUser{ // User structure
public:
    virtual bool authenticate(string username, string password) = 0;
    virtual void deleteAccount(AbstractUsers *users) = 0;
    string username;
    UserType type;
protected:
    string password;
};

class User:public AbstractUser{
public:
    
    User(string username, string password, UserType type){
        this->username = username;
        this->password = phash.doHash(password);
        this->type = type;
    }
    
    bool authenticate(string username, string password){
        return this->username == username && this->password == phash.doHash(password);
    }
    
    void deleteAccount(AbstractUsers *users){
        users->deleteAccount(this->username);
    }
    
};

class Users:public AbstractUsers{
protected:
    vector<AbstractUser *> users;
public:
    Users(vector<AbstractUser *> *users){
        this->users = *users;
    }
    
    UserType login(string username, string password){
        for(auto user = users.begin(); user != users.end(); user++){
            if((*user)->authenticate(username, password)){
                return (*user)->type;
            }
        }
        UserNotFoundException ex;
        throw ex;
    }
    
    void deleteAccount(string username){
        for(auto user = users.begin(); user != users.end(); user++){
            if(username == (*user)->username){
                users.erase(user);
                return;
            }
        }
        DeleteAccountException ex;
        throw ex;
    }
    
    void signup(string username, string password){
        //Check if user with that username exists and throw UserAlreadyExistsException in that case
        for(auto user = users.begin(); user != users.end(); user++) {
            if ((*user)->username == username) {
                UserAlreadyExistsException ex;
                throw ex;
            }
        }
        //Create user and add it to vector
        users.push_back(new User(username, password, UserType::MEMBER));
    }
    
};



enum MenuState{
    START,
    LOGGED_IN,
    END
};

class AppDatabase { //Just holds runtime data. doesn't save anything
public:
    vector<AbstractUser *> appUsers;
    
    AppDatabase() { //Load initial data
        appUsers.push_back(new User("admin",
                                    phash.doHash("admin") /* password is unsafe! for test only */,
                                    UserType::ADMIN)
                           );
    }
};

int main(){
    User * loggedInUser = nullptr;
    AppDatabase appDatabase;
    MenuState menuState = MenuState::START;
    Users users(&appDatabase.appUsers);
    char choice;
    cout << "Welcome!" << endl;

    while(menuState != MenuState::END){
        switch (menuState){
            case MenuState::START: {
                
                cout << "1. login\n2. signup\ne. exit\n";
                cin >> choice;
                switch(choice) {
                    case '1': {
                        string username, password;
                        cout << "Enter Username" << endl;
                        cin >> username;
                        cout << "Enter Password" << endl;
                        cin >> password;
                        try{
                            UserType userType = users.login(username, password);
                            loggedInUser = new User(username, password, userType);
                            menuState = MenuState::LOGGED_IN;
                            cout << "\nWelcome! please choose:\n";
                        }catch(UserNotFoundException e){
                            cout << e.getMessage() << "\n\n";
                        }
                        break;
                    }
                    case '2': {
                        string username, password;
                        cout << "Enter Username" << endl;
                        cin >> username;
                        cout << "Enter Password" << endl;
                        cin >> password;
                        try{
                            users.signup(username, password);
                            cout << "you successfully registered. please login!\n\n";
                        } catch (UserAlreadyExistsException e) {
                            cout << e.getMessage() << "\n\n";
                        }
                        break;
                    }
                    case 'e': {
                        cout << "\nGoodbye\n";
                        menuState = MenuState::END;
                        break;
                    }
                    default: {
                        cout << "Unknown Input" << endl;
                    }
                }
                break;
            }
            case MenuState::LOGGED_IN: {
                cout << "d.delete account\nl. logout\ne. exit\n";
                cin >> choice;
                switch(choice) {
                    case 'd': {
                        try{
                            loggedInUser->deleteAccount(&users);
                            loggedInUser = nullptr;
                            menuState = MenuState::START;
                            cout << "Account successfully deleted\n\n";
                        }catch(DeleteAccountException e){
                            cout << e.getMessage() << "\n\n";
                        }
                        break;
                    }
                    case 'l': {
                        loggedInUser = nullptr;
                        menuState = MenuState::START;
                        cout << "\nGoodbye\n\n";
                        break;
                    }
                    case 'e': {
                        menuState = MenuState::END;
                        cout << "\nGoodbye\n";
                        break;
                    }
                    default: {
                        cout << "Unknown Input" << endl;
                    }
                }
                break;
            }
        }
    }
    
    return 0;
    
}


