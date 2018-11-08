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



// todo: admin delete exception


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
                                    "admin" /* password is unsafe! for test only */,
                                    UserType::ADMIN)
        );
    }
    
};

string getpass(){
	const char BACKSPACE = 8;
	const char RETURN = 13;

	string password;
	unsigned char ch = 0;


	DWORD con_mode;
	DWORD dwRead;

	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

	GetConsoleMode(hIn, &con_mode);
	SetConsoleMode(hIn, con_mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

	while (ReadConsoleA(hIn, &ch, 1, &dwRead, NULL) && ch != RETURN){
		if (ch == BACKSPACE){
			if (password.length() != 0){
				cout << "\b \b";
				password.resize(password.length() - 1);
			}
		}
		else{
			password += ch;
			cout << '*';
		}
	}
	cout << endl;
	return password;
}

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
                        printf("\033[H\033[J");
                        cout << "Login Page\n" << endl;
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
                        printf("\033[H\033[J");
                        cout << "SignUp Page\n" << endl;
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
