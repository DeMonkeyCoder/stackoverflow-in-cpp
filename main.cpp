#include <iostream>
#include <vector>
#include<string>
#include<exception>
#include<Windows.h>
using namespace std;

enum UserType {
	ADMIN,
	MEMBER
};


class UserAlreadyExistsException : public exception {
public:
	const char * what() const throw() {
		return "Error: user already exists\n";
	}
};

class UnmatchedUserException : public exception {
public:
	const char * what() const throw() {
		return "Error: unmatched user\n";
	}
};

class UnmatchedPasswordException : public exception {
public:
	const char * what() const throw() {
		return "Error: unmatched password\n";
	}
};

class UnmatchedUserOrPasswordException : public exception {
public:
	const char * what() const throw() {
		return "Error: unmatched user or password\n";
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
		for (auto user = users->begin(); user != users->end(); user++)
			if ((*user)->authenticate(username, password))
				return (User*)*user;
		UnmatchedUserOrPasswordException ex;
		throw ex;
		/* return nullptr; */
	}

	static void signup(vector<AbstractUser*> *users, string username, string password) {

		// Check if user with that username exists and throw UserAlreadyExistsException in that case
		for (auto user = users->begin(); user != users->end(); user++) {
			if ((*user)->username == username) {
				UserAlreadyExistsException ex;
				throw ex;
			}
		}

		// Create user and add it to vector
		users->push_back(new User(username, password, UserType::MEMBER));
	}

	void deleteAccount(vector<AbstractUser*> *users) {
		string user, pass;
		cout << "Please enter your username: ";
		cin >> user;
		if (username != user) {
			UnmatchedUserException ex;
			throw ex;
		}
		cout << "Please enter your password: ";
		HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
		DWORD mode = 0;
		GetConsoleMode(hStdin, &mode);
		SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
		cin >> pass;
		SetConsoleMode(hStdin, mode);
		if (password != pass) {
			UnmatchedPasswordException ex;
			throw ex;
		}
		for (auto user = users->begin(); user != users->end(); user++)
			if ((*user)->authenticate(username, password)) {
				users->erase(user);
			}
	}

	/* string username; */
};

enum MenuState {
	START,
	LOGGED_IN,
	END
};

class AppDatabase { // Just holds runtime data. doesn't save anything
public:
	vector<AbstractUser *> appUsers;

	AppDatabase() { // Load initial data
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
				HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
				DWORD mode = 0;
				GetConsoleMode(hStdin, &mode);
				SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
				cin >> password;
				SetConsoleMode(hStdin, mode);
				try {
					loggedInUser = User::login(&appDatabase.appUsers, username, password);
					menuState = MenuState::LOGGED_IN;
				}
				catch (UnmatchedUserOrPasswordException &e) {
					cout << e.what();
				}
				break;
				/*
				if (loggedInUser == nullptr) { ---> i changed here
				cout << "couldn't login with given credentials.";
				}
				else {
				menuState = MenuState::LOGGED_IN;
				}
				break;
				*/
			}
			case '2': {
				string username, password;
				cout << "Enter Username" << endl;
				cin >> username;
				cout << "Enter Password" << endl;
				HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
				DWORD mode = 0;
				GetConsoleMode(hStdin, &mode);
				SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
				cin >> password;
				SetConsoleMode(hStdin, mode);
				try {
					User::signup(&appDatabase.appUsers, username, password);
				}
				catch (UserAlreadyExistsException &e) {
					/* cout << "Error: username already exists"; */
					cout << e.what();
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
			cout << "d. delete account\nl. logout\ne. exit\n";
			cin >> choice;
			switch (choice) {
			case 'd': {
				try {
					loggedInUser->deleteAccount(&appDatabase.appUsers);
					cout << "Account successfully deleted" << endl;
					loggedInUser = nullptr;
					menuState = MenuState::START;
				}
				catch (UnmatchedUserException &e) {
					cout << e.what();
				}
				catch (UnmatchedPasswordException &e) {
					cout << e.what();
				}
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

