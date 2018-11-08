#include <iostream>
#include <string>
#include "AbstractUser.h"
#include "Exceptions.h"
#include "User.h"
#include "Utils.h"
using namespace std;
using namespace Exceptions;


enum MenuState {
    START,
    LOGGED_IN,
    END
};

int main() {
    User * loggedInUser = nullptr;
    MenuState menuState = MenuState::START;
    string last_message = "";

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
                        try {
                            loggedInUser = Utils::loginUser();
                            menuState = MenuState::LOGGED_IN;
                        } catch (WrongUsernameOrPasswordException e) {
                            last_message = e.what();
                            loggedInUser = nullptr;
                        }
                        break;
                    }
                    case '2': { // signup
                        try {
                            Utils::signUp();
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
                            loggedInUser->deleteAccount();
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
