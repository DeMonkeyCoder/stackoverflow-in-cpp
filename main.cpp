#include <iostream>
#include <vector>
#include <string>
#include <exception>
using namespace std;

/**
* In the name of God
* Homework 2
* Sara Limooee
* std no : 9632421
* homework done with visual studio! ^^)
* Optinal : better structure for exception classes is done.
**/


enum UserType {
	ADMIN,
	MEMBER
};


class UserAlreadyExistsException : public exception {
public:
	virtual const char* what() const throw()
	{
		return "Error: username already exists\n";
	}
};

class UserNotExistException : public exception {
public:
	virtual const char* what() const throw()
	{
		return "couldn't login with given credentials.\n";
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

	User(string username, string password, UserType type) {
		this->username = username;
		this->password = password;
		this->type = type;
	}

	bool authenticate(string username, string password) {
		return this->username == username && this->password == password;
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
			"admin" /* password is unsafe! for test only */,
			UserType::ADMIN)
			);
	}
};

int main() {
	User * loggedInUser = nullptr;
	AppDatabase appDatabase;
	MenuState menuState = MenuState::START;

	char choice;
	cout << "Welcome!" << endl;

	while (menuState != MenuState::END) {
		switch (menuState) {
		case MenuState::START: {

			cout << "1. login\n2. signup\ne. exit\n";
			cin >> choice;
			switch (choice) {
			case '1': {
				string username, password;
				cout << "Enter Username" << endl;
				cin >> username;
				cout << "Enter Password" << endl;
				cin >> password;
				try
				{
					loggedInUser = User::login(&appDatabase.appUsers, username, password);
					cout << "You logged in successfully.\n";
					menuState = MenuState::LOGGED_IN;
				}
				catch (UserNotExistException ex)
				{
					cout << ex.what();
				}
				break;
			}
			case '2': {
				string username, password;
				cout << "Enter Username" << endl;
				cin >> username;
				cout << "Enter Password" << endl;
				cin >> password;
				try {
					User::signup(&appDatabase.appUsers, username, password);
					cout << "You signed up successfully.\n";
				}
				catch (UserAlreadyExistsException e) {
					cout << e.what();
				}
				break;
			}
			case 'e': {
				menuState = MenuState::END;
				break;
			}
			default: {
				UnknownInput ui;

				cout << ui.what();
			}
			}
			break;
		}
		case MenuState::LOGGED_IN: {
			cout << "d.delete account\nl. logout\ne. exit\n";
			cin >> choice;
			switch (choice) {
			case 'd': {
				loggedInUser->deleteAccount(&appDatabase.appUsers);
				cout << "Account successfully deleted\n";
				loggedInUser = nullptr;
				menuState = MenuState::START;
				break;
			}
			case 'l': {
				loggedInUser = nullptr;
				menuState = MenuState::START;
				cout << "You logged out successfullt.\n";
				break;
			}
			case 'e': {
				menuState = MenuState::END;
				break;
			}
			default: {
				UnknownInput ui;
				cout << ui.what();
			}
			}
			break;
		}
		}
		system("pause");
		system("cls");
	}

	return 0;

}