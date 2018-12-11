#include <iostream>
#include <string>
#include "AbstractUser.h"
#include "Exceptions.h"
#include "User.h"

#ifdef _WIN32
#define CLEAR "cls"
#else //In any other OS
#define CLEAR "clear"
#endif

using namespace std;


enum MenuState {
	START,
	LOGGED_IN,
	END
};

int main() {
	User::init("SECRET_KEY");
	User * loggedInUser = nullptr;
	MenuState menuState = MenuState::START;
	string last_message;

	char choice;
	while (menuState != MenuState::END) {
		// system(CLEAR);
		if (!last_message.empty())
			cout << last_message << endl;
		last_message = "";
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
				}
				catch (WrongUsernameOrPasswordException &e) {
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
				}
				catch (UsernameAlreadyExistsException &e) {
					last_message = e.what();
					break;

				}
				catch (EmailAlreadyExistsException &e) {
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
			cout << "d. delete account\nl. logout\ne. exit"
				"\na. add question\nu. update/edit question\nh. handle questions edit requests(Admin only)\no. omit/delete question"
				"\ng. good/upvote question\nb. bad/downvote question"
				"\ni. include/add question to your favorits\nr. remove question from your favorits\nf. see your favorit questions"
				"\nv. view question\ns. change status(Admin only)"
				"\nA. answer question\nU. update/edit answer\nH. handle answers edit requests(Admin only)"
				"\nO. omit/delete answer\nG. good/upvote answer\nB. bad/downvote answer\n";
			cin >> choice;
			switch (choice) {
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
			case 'l': { // logout
				loggedInUser = nullptr;
				menuState = MenuState::START;
				last_message = "GOOD BYE\n";
				break;
			}
			case 'a': { // add question
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clean cin
				string title, content, t;
				vector<string> tags;
				cout << "about the question you want to add enter the followings:" << endl;
				cout << "title: ";
				getline(cin, title);
				cout << "content: ";
				getline(cin, content);
				cout << "tags(seperated by space/newline) and then press EOF or press t to see allowed tags: ";
				while (cin >> t) {
					if (t == "t") {
						cout << "C,C++,Python,Django,MySQL,Java,JavaScript,Go,Socket" << endl;
						continue;
					}
					if (t == "C" || t == "C++" || t == "Python" || t == "Django" || t == "MySQL" ||
						t == "Java" || t == "JavaScript" || t == "Go" || t == "Socket")
						tags.emplace_back(t);
					else
						cout << t << "is not allowed as a tag" << endl;
				}
				cin.clear(); // cleaning cin after EOF in C++ is needed
				try {
					loggedInUser->createQuestion(title, content, tags);
				}
				catch (RepetitiveQuestionException e) {
					last_message = e.what();
				}
				break;
			}
			case 'u': { // update/edit question
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				string questionTitle, questionAuthor, newTitle, newContent, description, t;
				vector<string> newTags;
				cout << "about the question you want to update enter the followings:" << endl;
				cout << "title: ";
				getline(cin, questionTitle);
				cout << "author: ";
				getline(cin, questionAuthor);
				cout << "new title: ";
				getline(cin, newTitle);
				cout << "new content: ";
				getline(cin, newContent);
				cout << "new tags(seperated by space/newline) and the press EOF or press t to see allowed tags: ";
				while (cin >> t) {
					if (t == "t") {
						cout << "C,C++,Python,Django,MySQL,Java,JavaScript,Go,Socket" << endl;
						continue;
					}
					if (t == "C" || t == "C++" || t == "Python" || t == "Django" || t == "MySQL" ||
						t == "Java" || t == "JavaScript" || t == "Go" || t == "Socket")
						newTags.emplace_back(t);
					else
						cout << t << "is not allowed as a tag" << endl;
				}
				cin.clear();
				cout << "description: ";
				getline(cin, description);
				try {
					loggedInUser->editQuestion(questionTitle, questionAuthor, newTitle, newContent, newTags, description);
				}
				catch (NoSuchQuestionException e) {
					last_message = e.what();
				}
				break;
			}
			case 'h': { // handle questions edit requests
				try {
					loggedInUser->handleQuestionEditRequests();
				}
				catch (NotUserPropertyException e) {
					last_message = e.what();
				}
				break;
			}
			case'g': { // good/upvote question
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				string questionTitle, questionAuthor;
				cout << "about the question you want to upvote enter the followings:" << endl;
				cout << "title: ";
				getline(cin, questionTitle);
				cout << "author: ";
				getline(cin, questionAuthor);
				try {
					loggedInUser->upvoteQuestion(questionTitle,questionAuthor);
				}
				catch (NoSuchQuestionException e) {
					last_message = e.what();
				}
				break;
			}
			case'b': { // bad/downvote question
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				string questionTitle, questionAuthor;
				cout << "about the question you wnt to downvote enter the followings:" << endl;
				cout << "title: ";
				getline(cin, questionTitle);
				cout << "author: ";
				getline(cin, questionAuthor);
				try {
					loggedInUser->downvoteQuestion(questionTitle,questionAuthor);
				}
				catch (NoSuchQuestionException e) {
					last_message = e.what();
				}
				break;
			}
			case 'o': { // omit/delete question
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				string questionTitle, questionAuthor;
				cout << "about the question you want to delete enter the followings:" << endl;
				cout << "title: ";
				getline(cin, questionTitle);
				cout << "author: ";
				getline(cin, questionAuthor);
				try {
					loggedInUser->deleteQuestion(questionTitle,questionAuthor);
				}
				catch (NoSuchQuestionException e) {
					last_message = e.what();
				}
				break;
			}
			case'i': { // include/add question to favorits
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				string questionTitle, questionAuthor;
				cout << "about the question you want add to your favorite list enter the followings: " << endl;
				cout << "title: ";
				getline(cin, questionTitle);
				cout << "author: ";
				getline(cin, questionAuthor);
				try {
					loggedInUser->addQuestionToFavs(questionTitle,questionAuthor);
				}
				catch (NoSuchQuestionException e) {
					last_message = e.what();
				}
				break;
			}
			case'r': { // remove question from favorits
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				string questionTitle, questionAuthor;
				cout << "about the question you want to delete it from your favorite list enter the followings:" << endl;
				cout << "title: ";
				getline(cin, questionTitle);
				cout << "author: ";
				getline(cin, questionAuthor);
				try {
					loggedInUser->deleteQuestionFromFavs(questionTitle,questionAuthor);
				}
				catch (NoSuchQuestionException e) {
					last_message = e.what();
				}
				break;
			}
			case'f': { // see favorit list
				loggedInUser->viewFavQuestions();
				break;
			}
			case 'v': { // view question
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				string questionTitle, questionAuthor;
				cout << "about the question you want to see enter the followings:" << endl;
				cout << "title: ";
				getline(cin, questionTitle);
				cout << "author: ";
				getline(cin, questionAuthor);
				try {
					loggedInUser->viewQuestions(questionTitle,questionAuthor);
				}
				catch (NoSuchQuestionException e) {
					last_message = e.what();
				}
				break;
			}
			case 's': { // change status of question
				string copyTitle, copyAuthor, originalTitle, originalAuthor;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "about copy question enter the followings:" << endl;
				cout << "title: ";
				getline(cin, copyTitle);
				cout << "author: ";
				getline(cin, copyAuthor);

				cout << "about original question enter the followings:" << endl;
				cout << "title: ";
				getline(cin, originalTitle);
				cout << "author: ";
				getline(cin, originalAuthor);
				try {
					loggedInUser->changeStatus(copyTitle,copyAuthor,originalTitle,originalAuthor);
				}
				catch (NoSuchQuestionException e1) {
					last_message = e1.what();
				}
				catch (NotUserPropertyException e2) {
					last_message = e2.what();
				}
				break;
			}
			case 'A': { // answer question
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				string questionTitle, questionAuthor, answerContent;
				cout << "about the question you want to answer enter the followings:" << endl;
				cout << "question title: ";
				getline(cin, questionTitle);
				cout << "question author: ";
				getline(cin, questionAuthor);
				cout << "answer body: ";
				getline(cin, answerContent);
				try {
					loggedInUser->addAnswer(questionTitle, questionAuthor, answerContent);
				}
				catch (NoSuchQuestionException e) {
					last_message = e.what();
				}
				break;
			}
			case 'U': { // update/edit answer
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "about the answer you want to edit enter the followings:" << endl;
				string questionTitle, questionAuthor, answerAuthor, answerNewContent, description;
				vector<string> newTags;
				cout << "question title: ";
				getline(cin, questionTitle);
				cout << "question author: ";
				getline(cin, questionAuthor);
				cout << "answer author: ";
				getline(cin, answerAuthor);
				cout << "answer new content: ";
				getline(cin, answerNewContent);
				cout << "description: ";
				getline(cin, description);
				try {
					loggedInUser->editAnswer(questionTitle, questionAuthor, answerAuthor, answerNewContent, description);
				}
				catch (NoSuchQuestionException e1) {
					last_message = e1.what();
				}
				catch (NoSuchAnswerException e2) {
					last_message = e2.what();
				}
				break;
			}
			case 'H': { // handle answers edit requests
				try {
					loggedInUser->handleAnswerEditRequest();
				}
				catch (NotUserPropertyException e) {
					last_message = e.what();
				}
				break;
			}
			case 'O': { // omit/delete answer
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "about the answer you want to delete enter the followings:" << endl;
				string questionTitle, questionAuthor, answerAuthor;
				cout << "question title: ";
				getline(cin, questionTitle);
				cout << "question author: ";
				getline(cin, questionAuthor);
				cout << "answer author: ";
				getline(cin, answerAuthor);
				try {
					loggedInUser->deleteAnswer(questionTitle, questionAuthor, answerAuthor);
				}
				catch (NoSuchQuestionException e1) {
					last_message = e1.what();
				}
				catch (NoSuchAnswerException e2) {
					last_message = e2.what();
				}
				break;
			}
			case'G': { // good/upvote answer
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				string questionTitle, questionAuthor, answerAuthor;
				cout << "about the answer you want to upvote enter the followings:" << endl;
				cout << "question title: ";
				getline(cin, questionTitle);
				cout << "question author: ";
				getline(cin, questionAuthor);
				cout << "answer author: ";
				getline(cin, answerAuthor);
				try {
					loggedInUser->upvoteAnswer(questionTitle, questionAuthor, answerAuthor);
				}
				catch (NoSuchQuestionException e1) {
					last_message = e1.what();
				}
				catch (NoSuchAnswerException e2) {
					last_message = e2.what();
				}
				break;
			}
			case'B': { // bad/downvote answer
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				string questionTitle, questionAuthor, answerAuthor;
				cout << "about the answer you want to downvote enter the followings:" << endl;
				cout << "question title: ";
				getline(cin, questionTitle);
				cout << "question author: ";
				getline(cin, questionAuthor);
				cout << "answer author: ";
				getline(cin, answerAuthor);
				try {
					loggedInUser->downvoteAnswer(questionTitle, questionAuthor, answerAuthor);
				}
				catch (NoSuchQuestionException e1) {
					last_message = e1.what();
				}
				catch (NoSuchAnswerException e2) {
					last_message = e2.what();
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
		}
		}
	}
	system(CLEAR);
	cout << "GOODBYE" << endl;
	return 0;
}