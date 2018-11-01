#include <iostream>
#include <vector>
#include<stdlib.h>
#include <string>
#include <exception>
using namespace std;

/**
* In the name of God
* Homework 2
**/


enum UserType {
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
                        break;
                    }
                    case '2': {
                        string username, password;
                        cout << "\nEnter Username" << endl;
                        cin >> username;
                        cout << "\nEnter Password" << endl;
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
                        cout << "\nUnknown Input" << endl;
                    }
                }
                break;
            }
        }
    }
    
    return 0;
    
}