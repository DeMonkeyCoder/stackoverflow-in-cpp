#include <iostream>
#include <vector>
#include <functional>
#include <sstream>
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
    const char* message;
public:
    UserAlreadyExistsException (char* message){    
        this->message = message;
    } 
    const char* what() const throw(){
        return message;
    }
}; 

class AbstractUser{ // User structure
public:
    virtual bool authenticate(string username, string password) = 0;
    virtual void deleteAccount(vector<AbstractUser*> *users) = 0; 
    string username;
protected:
    string password;
    UserType type;
};


class User : public AbstractUser{
    hash<string> pass_hash;
public:
    // string username;

    User(string username, string password, UserType type){
        this->username = username;
        set_password(password);
        this->type = type;
    }
    int check_password(string password){
        hash<string> pass_hash_c;
        long long check = pass_hash(password);
        string a;
        stringstream out;
        out << check;
        a = out.str();
        return (this->password == a);
    }
    void set_password(string password){
        long long ps = pass_hash(password);
        string a;
        stringstream out;
        out << ps;
        a = out.str();
        this->password = a;
    }
    bool authenticate(string username, string password){
        int ath1 = 0;
        int ath2 = 0;
        if (this->username == username){ath1++;}
        if (check_password(password)){ath2++;}
        if (ath1 == 1 && ath2 == 1){return 1;}
        if (ath1 == 1 && ath2 == 0){throw 2;}  // we have username but the pass is incorrect  
        if (ath1 == 0 && ath2 == 0){return 0;} 
        return 0 ;
    }

    static User* login(vector<AbstractUser*> *users, string username, string password){ 
        for(auto user = users->begin(); user != users->end(); user++){
            if((*user)->authenticate(username, password)){
                return (User*) *user;
            }
        }
        return nullptr;
    }

    static void signup(vector<AbstractUser*> *users, string username, string password){
        for(auto user = users->begin(); user != users->end(); user++) {
            if ((*user)->username == username) {
                char* message="Username already exists";
                UserAlreadyExistsException ex(message); 
                throw ex;
            }
        }

        //Create user and add it to vector
        users->push_back(new User(username, password, UserType::MEMBER));
    }

    void deleteAccount(vector<AbstractUser*> *users){
        for(auto user = users->begin(); user != users->end(); user++) {
            if (*user == this) {
                users->erase(user);
                break;
            }
        }
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

    while(menuState != MenuState::END){
        switch (menuState){
            case MenuState::START: {
                cout << "\n1. login\n2. signup\ne. exit\n";
                cin >> choice;
                switch(choice) {
                    case '1': {
                        string username, password;
                        cout << "Enter Username" << endl;
                        cin >> username;
                        cout << "Enter Password" << endl;
                        cin >> password;
                        try{
                            loggedInUser = User::login(&appDatabase.appUsers, username, password);
                             if (loggedInUser == nullptr) {
                            cout << "couldn't login with given credentials." << endl;
                            } else {
                            menuState = MenuState::LOGGED_IN;
                            }
                        }
                        catch(int e){
                             cout << "Given password is incorrect" << endl;
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
                            User::signup(&appDatabase.appUsers, username, password);
                        } catch (UserAlreadyExistsException e) {
                            cout << "Error: username already exists" << endl;
                        }
                        break;
                    }
                    case 'e': {
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
                        loggedInUser->deleteAccount(&appDatabase.appUsers);
                        cout << "Account successfully deleted" << endl;
                        loggedInUser = nullptr;
                        menuState = MenuState::START;
                        break;
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
                        cout << "Unknown Input" << endl;
                    }
                }
                break;
            }
        }
    }

    return 0;

}
