#include <iostream>
#include <fstream>
#include <string>
#include "AbstractUser.h"
#include "Exceptions.h"
#include "User.h"
#include "Logger.h"
#include "Content.h"

#ifdef _WIN32
#define CLEAR "cls"
#else //In any other OS
#define CLEAR "clear"
#endif

using namespace std;


enum MenuState {
    START,
    LOGGED_IN,
    QUESTIONS,
    END
};

int main() {
    Logger::getInstance();
    User::init("SECRET_KEY");
    User * loggedInUser = nullptr;
    MenuState menuState = MenuState::START;
    string last_message;
    bool show_logs = false;
    char choice;
    int content_num = 0;
    while(menuState != MenuState::END) {
        if(not show_logs)
            system(CLEAR);
        show_logs = false;
        if (not last_message.empty()) {
            cout << last_message << endl;
            last_message = "";
        }
        switch (menuState) {
            case MenuState::START: {
                cout << "1. login\n2. signup\ne. exit\n";
                cin >> choice;
                switch (choice) {
                    case '1': { // login
                        try {
                            string username, password;
                            cout << "Enter Username: ";
                            cin >> username;
                            cout << "Enter Password: ";
                            cin >> password;
                            loggedInUser = &User::login(username, password);
                            menuState = MenuState::LOGGED_IN;
                            _Log(*loggedInUser);
                        } catch (WrongUsernameOrPasswordException &e) {
                            last_message = e.what();
                        }
                        break;
                    }
                    case '2': { // signup
                        try {
                            string username, password, email;
                            cout << "Enter Email: ";
                            cin >> email;
                            cout << "Enter Username: ";
                            cin >> username;
                            cout << "Enter Password: ";
                            cin >> password;
                            loggedInUser = &User::signup(username, password, email);
                            menuState = MenuState::LOGGED_IN;
                            last_message = "User signed up!\n";
                        } catch (exception &e) {
                            last_message = e.what();
                        }
                        break;
                    }
                    case 'e': { // exit
                        menuState = MenuState::END;
                        break;
                    }
                    default: { // unknown input
                        last_message = "Unknown Input\n";
                        break;
                    }
                }
                break;
            }
            case MenuState::LOGGED_IN: {
                if(loggedInUser->is_admin())
                    cout << "s. show logs\n";
                cout << "a. add question\nq. all questions\nd. delete account\nl. logout\ne. exit\n";
                cin >> choice;
                switch (choice) {
                    case 'a': { // add question
                        cout << "Enter your question: ";
                        string question;
                        cin.ignore(1);
                        getline(cin, question);
                        loggedInUser->create(question, ContentType::QUESTION);
                        break;
                    }
                    case 'q': { // all questions
                        content_num = 0;
                        menuState = MenuState::QUESTIONS;
                        break;
                    }
                    case 'd': { // delete account
                        try {
                            loggedInUser->deleteAccount();
                            cout << "Account successfully deleted\n";
                            loggedInUser = nullptr;
                            menuState = MenuState::START;
                        }
                        catch (DeleteAdminException &e) {
                            last_message = e.what();
                        }
                        break;
                    }
                    case 's': { // show logs
                        if(loggedInUser->is_admin()) {
                            system(CLEAR);
                            Logger::getInstance().printLogs();
                            show_logs = true;
                        }
                        else
                            last_message = "Unknown Input\n";
                        break;
                    }
                    case 'l': { // logout
                        loggedInUser = nullptr;
                        menuState = MenuState::START;
                        break;
                    }
                    case 'e': { // exit
                        menuState = MenuState::END;
                        break;
                    }
                    default: { // unknown input
                        last_message = "Unknown Input\n";
                        break;
                    }

                }
                break;
            }
            case MenuState::QUESTIONS: {
                loggedInUser->print_content(content_num);
                cout << "p. previous question\nn. next question\ne. edit question\nb. back to main menu\n";
                cin >> choice;
                switch (choice) {
                    case 'p': { // previous question
                        content_num--;
                        break;
                    }
                    case 'n': { // next question
                        content_num++;
                        break;
                    }
                    case 'e': { // edit question
                        cout << "Enter your question: ";
                        string question;
                        cin.ignore(1);
                        getline(cin, question);
                        loggedInUser->edit_content(content_num, question);
                        break;
                    }
                    case 'b': { // back to main menu
                        menuState = MenuState::LOGGED_IN;
                        break;
                    }
                    default: { // unknown input
                        last_message = "Unknown Input\n";
                        break;
                    }
                }
                break;
            }
        }
    }
    system(CLEAR);
    cout << "GOODBYE" << endl;
    return 0;
}
