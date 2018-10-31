#include <iostream>
#include <string>
#include <vector>
#include <windows.h>


#define Clear system("cls ");
#define Pause cout<< "Press any key to continue . . ." << endl; clean();

using namespace std;

/**
 * In the name of God
 * Homework 2
 * TODO: Complete this code using given TODO comments :). then remove TODOs
 * feel free to edit code as you want and make it better
 * Any questions? ask @devcom_support on telegram
 * Good luck!
 **/
// As all guys use hash, so i don't! ( As I be different... ;) )
// Warning the code must compile with windows!

enum UserType{
    ADMIN,
    MEMBER
};


class UserAlreadyExistsException:public exception{
private:
	char UserAlreadyExistsException_msg[50];
public:
	UserAlreadyExistsException()
		:UserAlreadyExistsException_msg("This user is already exists, Please try again\n") {
	}
	const char * what() const throw() {
		return UserAlreadyExistsException_msg;
	}
}; //TODO: Give exceptions a better structure. search google (optional)


class AbstractUser{ // User structure
public:
    virtual bool userExistence(string username) = 0;
	virtual bool UserCorrectPassword(string password) = 0;
    virtual void deleteAccount(vector < AbstractUser*> *users) = 0;
	//TODO: 1. implement this in User class. (You can't compile code and create instance of User until then). DON'T TOUCH ABSTRACT USER!
    string username;
protected:
    string password;
    UserType type; 
};

enum LoginException {
	USER_NOT_EXIST,
	WRONG_PASSWORD
};

class User : public AbstractUser{
public:
	
    User(string username, string password, UserType type){


		this->username = username;
		this->password = password;
		this->type = type;

    }

    bool userExistence(string username){
        return this->username == username;
    }

	bool UserCorrectPassword(string password) {
		return this->password == password;
	}

    void deleteAccount(vector<AbstractUser*> *users){

		vector<AbstractUser*> ::iterator itor;

		for (itor = users->begin(); itor != users->end(); itor++) {

			if ((*itor)->username == this->username) {
				users->erase(itor);
				return;
			}
		}
    }

    static User* login(vector<AbstractUser*> *users, string username, string password){ //TODO: 2. handle user login errors with exceptions

		
        for(auto user = users->begin() ; user != users->end(); user++){

            if((*user)->userExistence(username)){

				if ((*user)->UserCorrectPassword(password)) {
					return (User*)*user;
				}
				else {
					throw LoginException::WRONG_PASSWORD;
				}
            }
        }

		throw LoginException::USER_NOT_EXIST;
    }

    static void signup(vector< AbstractUser* > *users, string username, string password){

        //Check if user with that username exists and throw UserAlreadyExistsException in that case
        for(auto user = users->begin(); user != users->end(); user++) { //TODO: 3. this doesn't work. fix it!!
            if ((*user)->username == username) {
                UserAlreadyExistsException ex;
                throw ex;
            }
        }

        //Create user and add it to vector
        users->push_back(new User(username, password, UserType::MEMBER));
    }

};


class AppDatabase { //Just holds runtime data. doesn't save anything
public:
	vector < AbstractUser *> appUsers;

    AppDatabase() { //Load initial data
		appUsers.push_back(new User("admin", "admin" , UserType::ADMIN));
		// password is unsafe! for test only (however, It's none of my business! I'm just doing my homework)
    }
};

void clean(){
    while((getchar())!= '\n');
}

enum MenuState {
	START,
	LOGGED_IN,
	END
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

void menu() {

	User * loggedInUser = nullptr;
	AppDatabase appDatabase;
	MenuState menuState = MenuState::START;
	string loggedin_user;

	char choice;
	

	while (menuState != MenuState::END) {

		Clear;
		
		if (menuState == MenuState::START) {

			cout << "Welcome to system!" << endl;
			cout << "1. login" << endl << "2. signup" << endl << "e. exit" << endl;
			cin >> choice;
			clean();


			if (choice == '1') {

				string username, password;

				Clear;
				cout << "***** Welcome to login menu *****" << endl;

				cout << "Please enter Username: ";
				getline(cin, username);

				cout << "Please enter Password for " << "\"" << username << "\"" << ": ";
				password = getpass();


				try {

					loggedInUser = User::login(&appDatabase.appUsers, username, password);
					menuState = MenuState::LOGGED_IN;
					loggedin_user = username;
				}
				catch(LoginException loginException){

					if (loginException == LoginException::WRONG_PASSWORD) {

						Clear;
						cout << "Wrong password" << endl;
						Pause;

					}
					else if (loginException == LoginException::USER_NOT_EXIST) {

						Clear;
						cout << "User not exist" << endl;
						Pause;
					}
				}
				
			}

			else if (choice == '2') {

				string username, password;

				Clear;
				cout << "***** Welcome to signup menu *****" << endl;
				cout << "Please enter Username: ";
				getline(cin, username);

				cout << "Please enter Password for " << "\"" << username << "\"" << ": ";
				password = getpass();

				try {

					User::signup(&appDatabase.appUsers, username, password);
				}
				catch (UserAlreadyExistsException err) {
					Clear;
					cout << "Error: this username already exists";
					Pause;
				}
				
			}

			else if (choice == 'e') {

				menuState = MenuState::END;
			}

			else {

				Clear;
				cout << "Unknown Input" << endl;
				Pause;
			}
			
		}


		else if (menuState == MenuState::LOGGED_IN) {

			cout << "Welcome " << loggedin_user << "!" <<  endl;
			cout << "d. delete account" << endl << "l. logout" << endl << "e. exit" << endl;
			cin >> choice;
			clean();

			if (choice == 'd'){
				loggedInUser->deleteAccount(&appDatabase.appUsers);
				cout << "Account successfully deleted";
				loggedInUser = nullptr;
				menuState = MenuState::START;
			}

			else if (choice == 'l') {

				loggedInUser = NULL;
				menuState = MenuState::START;
			}

			else if (choice == 'e') {

				menuState = MenuState::END;
			}

			else {

				Clear;
				cout << "Unknown Input" << endl;
				Pause;
			}

		}


	}

}



int main(){

	menu();
    return 0;

}

