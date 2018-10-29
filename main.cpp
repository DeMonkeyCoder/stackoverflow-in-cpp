#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;


enum UserType{
  ADMIN,
  MEMBER
};


class UserDoesntExistException : public exception {
private:
  string message;
public:
  UserDoesntExistException(string message) {
    this -> message = message;
  }
  string what() {
    return message;
  }
};

class UserAlreadyExistsException : public exception {
private:
  string message;
public:
  UserAlreadyExistsException(string message) {
    this -> message = message;
  }
  string what() {
    return message;
  }
};

class AbstractUser { // User structure
public:
  string username;
  virtual bool authenticate(string username, string password) = 0;
  virtual void deleteAccount(vector <AbstractUser*> *users) = 0;
protected:
  string password;
  UserType type;
};


class User : public AbstractUser {
public:

  User(string username, string password, UserType type){
      this -> username = username;
      this -> password = password;
      this -> type = type;
  }

  void deleteAccount(vector <AbstractUser*> *users) {
    for (auto user = users -> begin(); user != users -> end(); user ++) {
       if ((*user) -> username == this -> username) {
         users -> erase(user);
         break;
       }
    }
  }

  bool authenticate(string username, string password) {
      return this -> username == username && this -> password == password;
  }

  static User* login(vector<AbstractUser*> *users, string username, string password){ //TODO: 2. handle user login errors with exceptions
      for (auto user = users -> begin(); user != users -> end(); user++){
          if((*user) -> authenticate(username, password)){
              return (User*) *user;
          }
      }
      UserDoesntExistException ex("couldn't login with given credentials.");
      throw ex;
  }

  static void signup(vector <AbstractUser*> *users, string username, string password) {

      //Check if user with that username exists and throw UserAlreadyExistsException in that case
      for(auto user = users -> begin(); user != users -> end(); user++) {
          if ((*user) -> username == username) {
              UserAlreadyExistsException ex("Error: username already exists");
              throw ex;
          }
      }

      //Create user and add it to vector
      users -> push_back(new User(username, password, UserType::MEMBER));
  }

};

enum MenuState{
  START,
  LOGGED_IN,
  END
};

class AppDatabase { //Just holds runtime data. doesn't save anything
public:
  vector <AbstractUser*> appUsers;

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
                      try {
                        loggedInUser = User::login(&appDatabase.appUsers, username, password);
                        menuState = MenuState::LOGGED_IN;
                      } catch (UserDoesntExistException e) {
                        cout << e.what() << endl;
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
                      } catch (UserAlreadyExistsException e) {
                          cout << e.what() << endl;
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
              switch(choice) {
                  case 'd': {
                      loggedInUser -> deleteAccount(&appDatabase.appUsers);
                      cout << "Account successfully deleted" << endl;
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
