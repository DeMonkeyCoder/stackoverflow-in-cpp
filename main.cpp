#include <iostream>
#include <vector>
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
	md5 hash;
	hash.update(data.begin(), data.end());
    hash.hex_digest(data_hex_digest);
	return data_hex_digest;
}

enum UserType {
    ADMIN,
    MEMBER
/**
* In the name of God
* Homework 2
**/


enum UserType {
	ADMIN,
	MEMBER
};
class AdminDeleteException{
public:
    string msg;
    AdminDeleteException(string text=string("ADMIN CANT BE DELETED!!!\n"))
    {
        msg=text;
    }
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

class DeleteAdminException : public exception {
private:
    const string message = "Error: can't delete admin account!";
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
class UserAlreadyExistsException:public exception{
    string msg;
    public:
        UserAlreadyExistsException(const string msg):msg(msg){}
        virtual const void what(){
            std:cout<<endl<<msg<<endl;
        }
};

class UnknownInput : public exception {
public:
	virtual const char* what() const throw()
	{
		return "Unknown Input\n";
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
private:
    const string salt = "E1F53135E559C253";
public:
    User(string username, string password, UserType type) {
        lower(username);
        this->username = username;
        this->password = md5_hash(username + password + this->salt);
        this->type = type;
    }

    bool authenticate(string username, string password) {
        lower(username);
        string hashed_password = md5_hash(username + password + this->salt);
        return this->username == username and this->password == hashed_password;
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

class User : public AbstractUser {
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

	User(string username, string password, UserType type) {
		this->username = username;
		this->password = password;
		this->type = type;
	}

	bool authenticate(string username, string password) {
		return this->username == username && this->password == password;
	}

    bool CheckPass(string pass){
        return this->password == password;
    }

    static User* login(vector<AbstractUser*> *users, string username, string password){
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
        for(auto user = users->begin(); user != users->end(); user++) { 
            if ((*user)->username == username) {
                throw UserAlreadyExistsException(username);
                UserAlreadyExistsException ex("Error: username already exists");
                throw ex;
            }
        }
    }
	static User* login(vector<AbstractUser*> *users, string username, string password) {
		for (auto user = users->begin(); user != users->end(); user++) {
			if ((*user)->authenticate(username, password)) {
				return (User*)*user;
			}
		}
		UserNotExistException ex;
		throw ex;
	}

    bool deleteAccount(vector<AbstractUser*> *users)
    {
        if(this ==*(users->begin()))
            throw AdminDeleteException();
        for(auto user = users->begin(); user!= users->end();user++)
            if((*user) == this)
                {
                    users->erase(user);
                    delete this;
                    return 1;
                }
	void deleteAccount(vector<AbstractUser*> *users)
	{
		//Check if user with this username exists to erase or not. if not throw an exception.
		//It only checks the username because password is a private field in AbstractUser class
		//and it is said not to change AbstractUser class AT ALL!
		for (auto user = users->begin(); user != users->end(); user++) {
			if ((*user)->username == username) {
				user = find(users->begin(), users->end(), this);
				users->erase(user);										//Delete this user from the list of users
				return;
			}
		}
		UserAlreadyExistsException ex;
		throw ex;
	}

	static void signup(vector<AbstractUser*> *users, string username, string password) {

    void deleteAccount(vector<AbstractUser*> *users) {
        if(this->type == UserType::ADMIN) {
            DeleteAdminException ex;
            throw ex;
        }
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
                                    "admin", /* password is unsafe! for test only */
    }
    
};

int main() {
    User * loggedInUser = nullptr;
    AppDatabase appDatabase;
    MenuState menuState = MenuState::START;
    string last_message = "";
    Users users(&appDatabase.appUsers);
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
                cout << "1. login\n2. signup\ne. exit\n";
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
                            UserType userType = users.login(username, password);
                            loggedInUser = new User(username, password, userType);
                            menuState = MenuState::LOGGED_IN;
                            cout << "\nWelcome! please choose:\n";
                        }catch(UserNotFoundException e){
                            cout << e.getMessage() << "\n\n";
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
                    case '2': {
                        string username, password;
                        cout << "Enter Username :" << endl;
                        cin >> username;
                        cout << "Enter Password :" << endl;
                        cin >> password;
                        try{
                            User::signup(&appDatabase.appUsers, username, password);
                        } catch (UserSignUpException& e) {
                            cout << e.what() <<endl;
                        }
                        break;
                    }
                    case 'e': {
                        cout << "\nGoodbye\n";
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
                system("clear");
                if(last_message != "")
                    cout << last_message << endl;
                last_message = "";
                cout << "d.delete account\nl. logout\ne. exit\n";
                cin >> choice;
                switch(choice) {
                    case 'd': {try{
                        loggedInUser->deleteAccount(&appDatabase.appUsers);
                        cout << "Account successfully deleted!\n";
                        loggedInUser = nullptr;
                        menuState = MenuState::START;}
                        catch(AdminDeleteException e)
                        {cout << e.msg << endl;}
                    case 'd': { // delete account
                        try {
                            loggedInUser->deleteAccount(&appDatabase.appUsers);
                        } catch(DeleteAdminException e) {
                            last_message = e.what();
                            break;
                        }
                        last_message = "Account successfully deleted";
                        loggedInUser = nullptr;
                        menuState = MenuState::START;
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
                            loggedInUser->deleteAccount(&users);
                            loggedInUser = nullptr;
                            menuState = MenuState::START;
                            cout << "Account successfully deleted\n\n";
                        }catch(DeleteAccountException e){
                            cout << e.getMessage() << "\n\n";
                        }
                        break;
                    }
                    case 'l': { // logout
                        loggedInUser = nullptr;
                        menuState = MenuState::START;
                        cout << "\nGoodbye\n\n";
                        break;
                    }
                    case 'e': { // exit
                        menuState = MenuState::END;
                        cout << "\nGoodbye\n";
                        break;
                    }
                    default: { // unknown input
                        last_message = "Unknown Input";
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
