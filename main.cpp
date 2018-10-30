#include <iostream>
#include <vector>
#include <exception>
#define SUCCESS true
#define ERROR	false


using namespace std;


enum UserType{
    ADMIN,
    MEMBER
};


class UserAlreadyExistsException : public exception {  // extending stdexception
	public :
		const char * what (){
			return "UserAlreadyExists";
		}

}; 


class LoginException : public exception {  // extending stdexception
	public :
		const char * what (){
			return "Couldn't login with given credentials";
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
public:

    User(string username, string password, UserType type){
        this->username = username;
        this->password = password;
        this->type = type;
    }

    bool authenticate(string username, string password){
        return this->username == username && this->password == password;
    }

    void deleteAccount(vector<AbstractUser *> *users){
        for (auto user = users->begin(); user != users->end(); user++){
            if (this->username == (*user)->username ){  // We want to delete just the current user's account ...
                users->erase(user);    
            }
        }
        
    }

    static User* login(vector<AbstractUser*> *users, string username, string password){ 
        for(auto user = users->begin(); user != users->end(); user++){
            if((*user)->authenticate(username, password)){
                return (User*) *user;
            }

        }
        LoginException LE;
        throw LE;

        return nullptr;
    }



    static void signup(vector<AbstractUser*> *users, string username, string password){

       
        for( auto user = users->begin(); user != users->end(); user++) { 
            
            	if((*user)->authenticate(username, password)) {    //Fixing the function
                	UserAlreadyExistsException ex;
                	throw ex;
            	}
            
        }

       
        users->push_back(new User(username, password, UserType::MEMBER));
    }

    string username;
};

enum MenuState{
    START,
    LOGGED_IN,
    END
};

class AppDatabase { 
public:
    vector<AbstractUser *> appUsers;

    AppDatabase() { 
        appUsers.push_back(new User("admin",
                                    "admin" ,
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

                cout << "1. login\n2. signup\ne. exit\n";
                cin >> choice;
                switch(choice) {
                    case '1': {
                        string username, password;
                        cout << "Enter Username" << endl;
                        cin >> username;
                        cout << "Enter Password" << endl;
                        cin >> password;
                        try{                                //Handling user login errors with exceptions
                        	loggedInUser = User::login(&appDatabase.appUsers, username, password);
                        	menuState = MenuState::LOGGED_IN;
                        }catch(LoginException login_exception){
                        	cout << login_exception.what() << endl;
                            
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
                        } catch (UserAlreadyExistsException exist_exception) {
                            cout << exist_exception.what() << endl;
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
                        cout << "Account successfully deleted";
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
            case MenuState::END:{
                ;
            }


        }
    }

    return 0;

}


