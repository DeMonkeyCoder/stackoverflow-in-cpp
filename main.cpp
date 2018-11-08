#include <iostream>
#include <vector>
#include <functional>
#include <sstream>
#include <unistd.h>
#include "myHash.h"
#include <string>
#include <unistd.h>
#include "md5.h"
#include <stdlib.h>
#include <string>
#include "AbstractUser.h"
#include "Exceptions.h"
#include "User.h"

using namespace std;
using namespace Exceptions;




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


int main() {
    User * loggedInUser = nullptr;
    AppDatabase appDatabase;
    MenuState menuState = MenuState::START;
    string last_message = "";
    //User users(&appDatabase.appUsers);
    char choice;
    while(menuState != MenuState::END) {
        system("clear");
        if (last_message != "")
            cout << last_message << endl;
        last_message = "";
        switch (menuState) {
            case MenuState::START: {
                cout << "1. login\n2. signup\ne. exit\n";
                cin >> choice;
                switch (choice) {
                    case '1': { // login
                        string username;
                        cout << "Enter Username: ";
                        cin >> username;
                        //char *pass = getpass("Enter password: ");
                        string password;
                        cout << "Enter Password: ";
                        cin >> password;
                        try {
                            loggedInUser = User::login(&appDatabase.appUsers, username, password);
                            menuState = MenuState::LOGGED_IN;
                        } catch (WrongUsernameOrPasswordException e) {
                            last_message = e.what();
                            loggedInUser = nullptr;
                        }
                        break;
                    }
                    case '2': { // signup
                        string username;
                        cout << "Enter Username: ";
                        cin >> username;
                        //char *pass = getpass("Enter password: ");
                        string password;
                        cout << "Enter Password: ";
                        cin >> password;
                        try {
                            User::signup(&appDatabase.appUsers, username, password);
                            last_message = "\n\nUser signed up!\n\n";
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
                        last_message = "\n\nUnknown Input\n\n";
                        break;
                    }
                }
                break;
            }
            case MenuState::LOGGED_IN: {
                cout << "d.delete account\nl. logout\ne. exit\n";
                cin >> choice;
                switch (choice) {
                    case 'd': {
                        try {
                            loggedInUser->deleteAccount(&appDatabase.appUsers);
                            cout << "\n\nAccount successfully deleted\n\n";
                            loggedInUser = nullptr;
                            menuState = MenuState::START;
                        }
                        catch (DeleteAdminException e) {
                            last_message = e.what();
                        }
                        break;
                    }
                    case 'l': { // logout
                        loggedInUser = nullptr;
                        menuState = MenuState::START;
                        last_message = "\n\nGOOD BYE\n\n";
                        break;
                    }
                    case 'e': { // exit
                        menuState = MenuState::END;
                        last_message = "\n\nGoodbye\n\n";
                        break;
                    }
                    default: { // unknown input
                        last_message = "\n\nUnknown Input\n\n";
                        break;
                    }

                }
            }
        }
    }
    return 0;
}
