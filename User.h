#pragma once

#include <iostream>
#include <string>
#include <map>
#include "AbstractUser.h"

string currentDateTime();

class User : public AbstractUser {
public:
	// Login related
	User(string username, string password, string email, UserType type);
	static void init(const string &salt);
	void set_password(string password);
	bool check_password(string password);
	bool authenticate(string username, string password);
	void deleteAccount();
	static User& login(string username, string password);
	static User& signup(string username, string password, string email);
	
	// Question related
	void createQuestion(string title, string content, vector<string> tags);
	void editQuestion(string title, string author,
		string newTitle, string newContent, vector<string> newTags, string description);
	void handleQuestionEditRequests();
	void deleteQuestion(string title, string author);
	void upvoteQuestion(string title ,string author);
	void downvoteQuestion(string title, string author);
	void addQuestionToFavs(string title, string author);
	void deleteQuestionFromFavs(string title, string author);
	void viewFavQuestions();
	void viewQuestions(string title, string author);
	void changeStatus(string copyTitle, string copyAuthor, string originalTitle, string originalAuthor);

	// Answer related
	void addAnswer(string questionTitle, string questionAuthor, string answerContent);
	void editAnswer(string questionTitle, string questionAuthor, string answerAuthor, string answerNewContent, string description);
	void handleAnswerEditRequest();
	void deleteAnswer(string questionTitle, string questionAuthor, string answerAuthor);
	void upvoteAnswer(string questionTitle, string questionAuthor, string answerAuthor);
	void downvoteAnswer(string questionTitle, string questionAuthor, string answerAuthor);

protected:
	// Question related
	void searchAllQuestions(
		string title,
		string author,
		vector<User>::iterator& userIt,
		vector<Question>::iterator& questionIt);

	// Answer related
	void searchQuestionAnswers(
		string author,
		vector<Answer>::iterator& answerIt,
		vector<Question>::iterator& questionIt);

private:
	// Login related
	static string salt;
	static vector<User> users;
};