#include <iostream>
#include <vector>
#include<stdlib.h>
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


class UserAlreadyExistsException:public exception{
    string msg;
    public:
        UserAlreadyExistsException(const string msg):msg(msg){}
        virtual const void what(){
            std:cout<<endl<<msg<<endl;
        }

}; //TODO: Give exceptions a better structure. search google (optional)

class AbstractUser{ // User structure
public:
    virtual bool authenticate(string username, string password) = 0;
    virtual bool deleteAccount(vector<AbstractUser*> *users) = 0;
    string username;
protected:
    string password;
    UserType type;
};


class User : public AbstractUser{
public:
    
    bool deleteAccount(vector<AbstractUser*> *users){
        for(auto user = users->begin(); user != users->end(); user++) {
            if(this->type==ADMIN){break;}
            if((*user)->authenticate(this->username, this->password)){
                users->erase(user);
                return 1;
                break;
            }
        }
        UserAlreadyExistsException ex("This user is not successfully deleted");
        throw ex;                                                         
        return 0;
    }

    User(string username, string password, UserType type){
        this->username = username;
        this->password = password;
        this->type = type;
    }

    bool authenticate(string username, string password){
        return this->username == username && this->password == password;
    }

    bool CheckPass(string pass){
        return this->password == password;
    }

    static User* login(vector<AbstractUser*> *users, string username, string password){ //TODO: 2. handle user login errors with exceptions
        for(auto user = users->begin(); user != users->end(); user++){
            if((*user)->authenticate(username, password)){
                return (User*) *user;
            }
        }
        UserAlreadyExistsException ex("Your username or Your password is incorrect");
        throw ex;
        return nullptr;
    }

    static void signup(vector<AbstractUser*> *users, string username, string password){
        
        //Check if user with that username exists and throw UserAlreadyExistsException in that case
        for(auto user = users->begin(); user != users->end(); user++) { //TODO: 3. this doesn't work. fix it!!
            if ((*user)->username == username) {
                UserAlreadyExistsException ex("Error: username already exists");
                throw ex;
            }
        }

        //Create user and add it to vector
        users->push_back(new User(username, password, UserType::MEMBER));
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
                                    "admin" /* password is unsafe! for test only */,
                                    UserType::ADMIN)
        );
    }
};

int main(){
    User * loggedInUser = nullptr;
    AppDatabase appDatabase;
    MenuState menuState = MenuState::START;

    char choice;
    cout << "Welcome!" << endl;

    while(menuState != MenuState::END){
        switch (menuState){
            case MenuState::START: {

                cout << "\n1. login\n2. signup\ne. exit\n";
                cin >> choice;
                switch(choice) {
                    case '1': {
                        string username, password;
                        cout << "\nEnter Username" << endl;
                        cin >> username;
                        cout << "\nEnter Password" << endl;
                        cin >> password;
                        try{
                            loggedInUser = User::login(&appDatabase.appUsers, username, password);
                        }
                        catch(UserAlreadyExistsException e){
                            e.what();
                            loggedInUser = nullptr;
                        }
                        if (loggedInUser != nullptr) {
                            menuState = MenuState::LOGGED_IN;
                        }
                        break;
                    }
                    case '2': {
                        string username, password;
                        cout << "\nEnter Username" << endl;
                        cin >> username;
                        cout << "\nEnter Password" << endl;
                        cin >> password;
                        try{
                            User::signup(&appDatabase.appUsers, username, password);
                        } catch (UserAlreadyExistsException e) {
                            e.what();
                        }
                        break;
                    }
                    case 'e': {
                        menuState = MenuState::END;
                        break;
                    }
                    default: {
                        cout << "\nUnknown Input" << endl;
                    }
                }
                break;
            }
            case MenuState::LOGGED_IN: {
                cout << "\nd.delete account\nl. logout\ne. exit\n";
                cin >> choice;
                switch(choice) {
                    case 'd': {
                        try{
                            loggedInUser->deleteAccount(&appDatabase.appUsers);
                            cout << "\nAccount successfully deleted";
                            loggedInUser = nullptr;
                            menuState = MenuState::START;
                            break;
                        }
                        catch(UserAlreadyExistsException e){
                            e.what();
                        }
                    }
                    case 'l': {
                        loggedInUser = nullptr;
                        menuState = MenuState::START;
                        break;
                    }
                    case 'e': {
                        menuState = MenuState::END;
                        break;
                    }
                    default: {
                        cout << "\nUnknown Input" << endl;
                    }
                }
                break;
            }
        }
    }

    return 0;

}

