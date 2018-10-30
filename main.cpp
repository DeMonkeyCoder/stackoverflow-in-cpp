#include <iostream>
#include <vector>
#include <time.h>
using namespace std;



#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"
#define RESET "\033[0m"

// a funciton for stop a program for a while

void sleepcp(int milliseconds) // Cross-platform sleep function
{
    clock_t time_end;
    time_end = clock() + milliseconds * CLOCKS_PER_SEC/1000;
    while (clock() < time_end)
    {
    }
}

enum UserType{
    ADMIN,
    MEMBER
};


class UserAlreadyExistsException{
private:
   string msg;
public:
   UserAlreadyExistsException(const string& msg) : msg(msg) {}


   string getMessage() const {return(msg);}

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

    void deleteAccount(vector<AbstractUser*> *users)
    {
        int index = 0;
        for (auto user=users->begin() ; user != users->end() ; user++)
        {

            if ((*user)->username == this->username)
            {
                users->erase(users->begin() + index);
                break;
            }
            index ++;


        }
    }
    bool authenticate(string username, string password){
        return this->username == username && this->password == password;
    }

    static User* login(vector<AbstractUser*> *users, string username, string password){
        for(auto user = users->begin(); user != users->end(); user++){
            if((*user)->authenticate(username, password)){
                return (User*) *user;
            }
        }
        string s = "Username or password is false";
        throw s;
    }

    static void signup(vector<AbstractUser*> *users, string username, string password){

        for(auto user = users->begin(); user != users->end(); user++) {
            if ((*user)->username == username){
                UserAlreadyExistsException ex("The username is Already Exists");
                throw ex;
            }
        }

        //Create user and add it to vector
        users->push_back(new User(username, password, UserType::MEMBER));
    }

  //  string username;
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
    char response;
    User * loggedInUser = nullptr;
    AppDatabase appDatabase;
    MenuState menuState = MenuState::START;
    int start = 0;
    char choice;
    while(menuState != MenuState::END){cout << KYEL << "choice: "<<RESET;
        system("clear");
        if (start++ == 0)
        cout << KMAG << "Welcome!" << RESET << endl;
        switch (menuState){
            case MenuState::START: {

                cout << KGRN << "1. login" <<endl << KBLU << "2. signup" << endl << KRED<< "e. exit"<< endl << RESET;
                cout << KYEL << "choice: "<<RESET;
                cin >> choice;
                switch(choice) {
                    case '1': {
                        string username, password;
                        cout << "Enter" << KYEL<<  " YOUR " << KRED << "Username:" <<RESET<< endl;
                        cin >> username;
                        cout << "Enter"<< KYEL<<  " YOUR "<< KRED << "Password:" << RESET << endl;
                        cin >> password;
                        try{
                            loggedInUser = User::login(&appDatabase.appUsers, username, password);
                            if (loggedInUser != nullptr)
                            {
                                menuState = MenuState::LOGGED_IN;
                            }
                        }catch(string& s)
                        {
                            cout << KRED << "ERROR: "<< s <<RESET <<endl;
                            cout << "Press any key to "<< KYEL << "retry" << RESET << " again";
                            cin.ignore().get(); 
                        }
                        
                        break;
                    }
                    case '2': {
                        string username, password;
                        cout << "Enter" << KYEL<<  " A " << KRED << "Username:" <<RESET<< endl;
                        cin >> username;
                        cout << "Enter" << KYEL<<  " A " << KRED << "Password:" <<RESET<< endl;
                        cin >> password;
                        try{
                            User::signup(&appDatabase.appUsers, username, password);
                        } catch (UserAlreadyExistsException& e) {
                            cout <<  KRED << "ERROR: "<< e.getMessage()<< RESET<<endl;
                            cout << "Press any key to "<< KYEL << "retry" << RESET << " again";
                            cin.ignore().get(); 
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
                cout << KRED <<"d."<<RESET<< "delete account\n" << KBLU <<"l." <<RESET<< " logout\n" << KCYN << "e." <<RESET<<" exit\n";
                cout << KYEL << "choice: "<<RESET;
                cin >> choice;
                switch(choice) {
                    case 'd': {
                        loggedInUser->deleteAccount(&appDatabase.appUsers);
                        cout << "Account successfully deleted (Please Wait!)" << endl;
                        sleepcp(3000);
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

