#include <iostream>
#include <utility>
#include <sstream>
#include <time.h>
#include "User.h"
#include "Exceptions.h"

#pragma warning(disable:4996)

#define questionsSize 20
#define upvotedQuestionSize 20
#define downvotedQuestionSize 20
#define thumbedUpSize 20
#define thumbedDownSize 20
#define RequestsSize 20

string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%b %d'%y at %H:%M", &tstruct);
	return buf;
}

vector<User> User::users;
string User::salt;
Logger User::log = *Logger::getInstance();

User::User(string username, string password, string email, UserType type) {
	lower(username);
	this->username = username;
	set_password(std::move(password));
	this->email = email;
	this->type = type;
	this->contents["Question"].reserve(questionsSize);
	this->thumbedUp.reserve(thumbedUpSize);
	this->thumbedDown.reserve(thumbedDownSize);
	this->upvoted.reserve(upvotedQuestionSize);
	this->downvoted.reserve(downvotedQuestionSize);
	this->upvotedAnswers.reserve(thumbedUpSize);
	this->downvotedAnswers.reserve(thumbedDownSize);
	if (type == UserType::ADMIN) {
		this->questionEditRequests.reserve(RequestsSize);
		this->answerEditRequests.reserve(RequestsSize);
	}
}

void User::set_password(string password) {
	size_t ps = pass_hash(password + salt);
	stringstream out;
	out << ps;
	this->password = out.str();
}

bool User::check_password(string password) {
	size_t check = pass_hash(password + salt);
	stringstream out;
	out << check;
	return (this->password == out.str());
}

bool User::authenticate(string username, string password) {
	lower(username);
	return this->username == username and check_password(password);
}
void User::deleteAccount() {
	if (this->type == UserType::ADMIN) {
		throw DeleteAdminException();
	}

	for (auto user = users.begin(); user != users.end(); user++) {
		if (user->username == this->username) {
			users.erase(user);
			break;
		}
	}
}

User& User::login(string username, string password) {
	log.inc();
	for (auto &user : users) {
		if (user.authenticate(username, password)) {
			log << user.email << user.username << currentDateTime() ;
			return user;
		}
	}
	throw WrongUsernameOrPasswordException();
}

User& User::signup(string username, string password, string email) {
	for (auto &user : users) {
		if (user.username == username) {
			throw UsernameAlreadyExistsException();
		}
		else if (user.email == email) {
			throw EmailAlreadyExistsException();
		}
	}
	//Create user
	users.emplace_back(username, password, email, UserType::MEMBER);
	return users[users.size() - 1];
}

void User::init(const string &salt) {
	User::salt = salt;
	users.reserve(20);
	users.emplace_back("admin", "admin", "admin@stackoverflow.com", UserType::ADMIN);
}

// point userIt to end of users which means there is no question with specified title
// or
// point userIt to related user and point questionIt to specified question
void User::searchAllQuestions(
	string title,
	string author,
	vector<User>::iterator& userIt,
	vector<Question>::iterator& questionIt) 
{
	vector<User>::iterator userSourceIt = users.begin();
	vector<User>::iterator userDestIt = users.end();
	for (; userSourceIt != userDestIt; userSourceIt++) {
		map<string, vector <Question> >::iterator contentsIt = userSourceIt->contents.find("Question");
		if (contentsIt != userSourceIt->contents.end())
			for (vector<Question>::iterator
				questionSourceIt = contentsIt->second.begin(), questionDestIt = contentsIt->second.end();
				questionSourceIt != questionDestIt;
				questionSourceIt++)
				if (questionSourceIt->title == title && questionSourceIt->author==author) {
					questionIt = questionSourceIt;
					userIt = userSourceIt;
					return;
				}
	}
	userIt = userSourceIt;
}

// point answerIt to end of answers which means there is no answer with specified question, author, and title
// or
// point answerIt to related answer
void User::searchQuestionAnswers(
	string author,
	vector<Answer>::iterator& answerIt,
	vector<Question>::iterator& questionIt)
{
	vector<Answer>::iterator answerSourceIt = questionIt->answers.begin();
	vector<Answer>::iterator answersDestIt = questionIt->answers.end();
	for (; answerSourceIt != answersDestIt; answerSourceIt++)
		if (answerSourceIt->author == author) {
			answerIt = answerSourceIt;
			return;
		}
	answerIt = answerSourceIt;
}

void User::createQuestion(string title, string content, vector<string> tags) {
	
	vector<User>::iterator userIt;
	vector<Question>::iterator questionIt;
	searchAllQuestions(title, this->username, userIt, questionIt);
	if (userIt != users.end())
		throw RepetitiveQuestionException();

	this->contents["Question"].emplace_back(this->username, title, content, currentDateTime(), tags, 0, 0, 0, 0);
}

void User::editQuestion(string title, string author,
	string newTitle, string newContent, vector<string> newTags, string description) {
	vector<User>::iterator usersIt;
	vector<Question>::iterator questionIt;
	searchAllQuestions(title, author, usersIt, questionIt);
	if (usersIt == users.end())
		throw NoSuchQuestionException();

	else if (usersIt->username != this->username) { // need to be accept by admin
		for (vector<User>::iterator
			usersSourceIt = users.begin(), usersDestIt = users.end();
			usersSourceIt != usersDestIt;
			usersSourceIt++)
			if (usersSourceIt->type == UserType::ADMIN) {
				cout << "your request is sended to ADMIN " << usersSourceIt->username << endl;
				usersSourceIt->questionEditRequests.emplace_back(questionIt->author, questionIt->title, this->username,
					newTitle, newContent, newTags, currentDateTime(), description);
			}
	}

	else if (usersIt->username == this->username) { // don't need admin acception
		questionIt->title = newTitle;
		questionIt->content = newContent;
		questionIt->tags.clear();
		questionIt->tags.insert(questionIt->tags.end(), newTags.begin(), newTags.end());
		questionIt->editors.emplace_back(this->username, currentDateTime(), description);
		cout << "done :)" << endl;
	}
}

void User::handleQuestionEditRequests() {
	if (this->type != UserType::ADMIN)
		throw NotUserPropertyException();

	vector<questionEditRequest>::iterator editRequestSourceIt = this->questionEditRequests.begin();
	while (editRequestSourceIt != this->questionEditRequests.end()) {

		vector<User>::iterator userIt;
		vector<Question>::iterator questionIt;
		searchAllQuestions(editRequestSourceIt->questionTitle, editRequestSourceIt->questionAuthor, userIt, questionIt);

		cout << "main question was:" << endl;
		if (questionIt->duplicate == true)
			cout << "duplicat to: " << questionIt->duplicateTo.title <<
			" from " << questionIt->duplicateTo.author << endl;
		cout << "title: " << questionIt->title << endl;
		cout << "content: " << questionIt->content << endl;
		cout << "tags: ";
		vector<string>::iterator previousTagsSourceIt = questionIt->tags.begin();
		vector<string>::iterator previousTagsDestIt = questionIt->tags.end();
		if (previousTagsSourceIt != previousTagsDestIt) { // prevent last excess comma such as in (a, b, )
			cout << *previousTagsSourceIt;
			previousTagsSourceIt++;
		}
		for (; previousTagsSourceIt != previousTagsDestIt; previousTagsSourceIt++)
			cout << ", " << *previousTagsSourceIt;
		cout << endl;
		cout << "asked " << questionIt->creationDate << endl;
		cout << "user: " << userIt->username << endl;
		for (vector<Edit>::iterator
			previousEditsSourceIt = questionIt->editors.begin(), previousEditsDestIt = questionIt->editors.end();
			previousEditsSourceIt != previousEditsDestIt;
			previousEditsSourceIt++)
			cout << "edited " << previousEditsSourceIt->editDate << endl << previousEditsSourceIt->editorName << endl;
		cout << "viewed " << questionIt->numViews << " times" << endl;
		cout << "reputation: " << questionIt->numReputation << endl;
		cout << "favorites: " << questionIt->numFavorite << endl;
		cout << endl;

		cout << "edited question is:" << endl;
		cout << "Edited: " << editRequestSourceIt->editDate << endl;
		cout << "By: " << editRequestSourceIt->editorName << endl;
		cout << "Title: " << editRequestSourceIt->editedTitle << endl;
		cout << "Body: " << editRequestSourceIt->editedContent << endl;
		cout << "Rev: " << editRequestSourceIt->description << endl;
		cout << "Tags: ";
		vector<string>::iterator tagsSourceIt = editRequestSourceIt->editedTags.begin();
		vector<string>::iterator tagsDestIt = editRequestSourceIt->editedTags.end();
		if (tagsSourceIt != tagsDestIt) {
			cout << *tagsSourceIt;
			tagsSourceIt++;
		}
		for (; tagsSourceIt != tagsDestIt; tagsSourceIt++)
			cout << ", " << *tagsSourceIt;
		cout << endl;

		string choice;
		while (true) {
			cout << "will you accept this edit?(y:yes, n:no, e:exit): ";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			getline(cin, choice);
			if (choice == "y" || choice == "Y") {
				questionIt->title = editRequestSourceIt->editedTitle;
				questionIt->content = editRequestSourceIt->editedContent;
				questionIt->tags.clear();
				questionIt->tags.insert(questionIt->tags.end(),
					editRequestSourceIt->editedTags.begin(), editRequestSourceIt->editedTags.end());
				questionIt->editors.emplace_back(editRequestSourceIt->editorName,
					editRequestSourceIt->editDate, editRequestSourceIt->description);
				editRequestSourceIt = this->questionEditRequests.erase(editRequestSourceIt);
				cout << "done :)" << endl;
				break;
			}
			else if (choice == "n" || choice == "N") {
				editRequestSourceIt++;
				break;
			}
			else if (choice == "e" || choice == "E") {
				return;
			}
			else {
				cout << "not an option!" << endl;
			}
		}
	}
}

void User::deleteQuestion(string title, string author) {
	vector<Question>::iterator questionIt;
	vector<User>::iterator userIt;
	searchAllQuestions(title, author, userIt, questionIt);
	if (userIt == users.end())
		throw NoSuchQuestionException();

	if (this->username != questionIt->author) {
		cout << "this isn't your question" << endl;
		return;
	}

	userIt->contents["Question"].erase(questionIt);
	cout << "done :)" << endl;
}

void User::upvoteQuestion(string title, string author) {
	vector<User>::iterator usersIt;
	vector<Question>::iterator questionIt;
	searchAllQuestions(title, author, usersIt, questionIt);
	if (usersIt == users.end())
		throw NoSuchQuestionException();

	vector<Question>::iterator questionSourceIt = this->upvoted.begin();
	vector<Question>::iterator questionDestIt = this->upvoted.end();
	for (questionSourceIt; questionSourceIt != questionDestIt; questionSourceIt++)
		if (questionSourceIt->title == title)
			break;
	if (questionSourceIt != questionDestIt)
		cout << "you upvoted this question before" << endl;
	else {
		vector<Question>::iterator downvotedSourceIt = this->downvoted.begin();
		vector<Question>::iterator downvotedDestIt = this->downvoted.end();
		for (downvotedSourceIt; downvotedSourceIt != downvotedDestIt; downvotedSourceIt++)
			if (downvotedSourceIt->title == title) {
				this->downvoted.erase(downvotedSourceIt);
				questionIt->numReputation++;
				break;
			}
		if (downvotedSourceIt == downvotedDestIt) {
			questionIt->numReputation++;
			this->upvoted.emplace_back(questionIt->author, questionIt->title,
				questionIt->content, questionIt->creationDate, questionIt->tags,
				questionIt->numReputation, questionIt->numViews, questionIt->numFavorite, questionIt->duplicate);
		}
	}
}

void User::downvoteQuestion(string title, string author) {
	vector<User>::iterator usersIt;
	vector<Question>::iterator questionIt;
	searchAllQuestions(title, author, usersIt, questionIt);
	if (usersIt == users.end())
		throw NoSuchQuestionException();

	vector<Question>::iterator questionSourceIt = this->downvoted.begin();
	vector<Question>::iterator questionDestIt = this->downvoted.end();
	for (questionSourceIt; questionSourceIt != questionDestIt; questionSourceIt++)
		if (questionSourceIt->title == title)
			break;
	if (questionSourceIt != questionDestIt)
		cout << "you downvoted this question before" << endl;
	else {
		vector<Question>::iterator upvotedSourceIt = this->upvoted.begin();
		vector<Question>::iterator upvotedDestIt = this->upvoted.end();
		for (upvotedSourceIt; upvotedSourceIt != upvotedDestIt; upvotedSourceIt++)
			if (upvotedSourceIt->title == title) {
				this->upvoted.erase(upvotedSourceIt);
				questionIt->numReputation--;
				break;
			}
		if (upvotedSourceIt == upvotedDestIt) {
			questionIt->numReputation--;
			this->downvoted.emplace_back(questionIt->author, questionIt->title,
				questionIt->content, questionIt->creationDate, questionIt->tags,
				questionIt->numReputation, questionIt->numViews, questionIt->numFavorite, questionIt->duplicate);
		}
	}
}

void User::addQuestionToFavs(string title, string author) {
	vector<User>::iterator usersIt;
	vector<Question>::iterator questionIt;
	searchAllQuestions(title, author, usersIt, questionIt);
	if (usersIt == users.end())
		throw NoSuchQuestionException();

	vector<Question>::iterator questionSourceIt = this->thumbedUp.begin();
	vector<Question>::iterator questionDestIt = this->thumbedUp.end();
	for (questionSourceIt; questionSourceIt != questionDestIt; questionSourceIt++)
		if (questionSourceIt->title == title)
			break;
	if (questionSourceIt != questionDestIt)
		cout << "you thumbed up this question before" << endl;
	else {
		vector<Question>::iterator thumbedDownSourceIt = this->thumbedDown.begin();
		vector<Question>::iterator thumbedDownDestIt = this->thumbedDown.end();
		for (thumbedDownSourceIt; thumbedDownSourceIt != thumbedDownDestIt; thumbedDownSourceIt++)
			if (thumbedDownSourceIt->title == title) {
				this->thumbedDown.erase(thumbedDownSourceIt);
				questionIt->numFavorite++;
				break;
			}
		if (thumbedDownSourceIt == thumbedDownDestIt) {
			questionIt->numFavorite++;
			this->thumbedUp.emplace_back(questionIt->author, questionIt->title,
				questionIt->content, questionIt->creationDate, questionIt->tags,
				questionIt->numReputation, questionIt->numViews, questionIt->numFavorite, questionIt->duplicate);
		}
	}
}

void User::deleteQuestionFromFavs(string title, string author) {
	vector<User>::iterator usersIt;
	vector<Question>::iterator questionIt;
	searchAllQuestions(title, author, usersIt, questionIt);
	if (usersIt == users.end())
		throw NoSuchQuestionException();

	vector<Question>::iterator questionSourceIt = this->thumbedDown.begin();
	vector<Question>::iterator questionDestIt = this->thumbedDown.end();
	for (questionSourceIt; questionSourceIt != questionDestIt; questionSourceIt++)
		if (questionSourceIt->title == title)
			break;
	if (questionSourceIt != questionDestIt)
		cout << "you didn't thumb it up so you can't thumb it down" << endl;
	else {
		vector<Question>::iterator thumbedUpSourceIt = this->thumbedUp.begin();
		vector<Question>::iterator thumbedUpDestIt = this->thumbedUp.end();
		for (thumbedUpSourceIt; thumbedUpSourceIt != thumbedUpDestIt; thumbedUpSourceIt++)
			if (thumbedUpSourceIt->title == title) {
				this->thumbedUp.erase(thumbedUpSourceIt);
				questionIt->numFavorite--;
				this->thumbedDown.emplace_back(questionIt->author, questionIt->title,
					questionIt->content, questionIt->creationDate, questionIt->tags,
					questionIt->numReputation, questionIt->numViews, questionIt->numFavorite, questionIt->duplicate);
				break;
			}
		if (thumbedUpSourceIt == thumbedUpDestIt) {
			cout << "you didn't thumb it up so you can't thumb it down" << endl;
		}
	}
}

void User::viewFavQuestions() {
	for (vector<Question>::iterator
		questionsSourceIt = this->thumbedUp.begin(), questionsDestIt = this->thumbedUp.end();
		questionsSourceIt != questionsDestIt;
		questionsSourceIt++) {
		cout << questionsSourceIt->title << " asked " <<
			questionsSourceIt->creationDate << " by " << questionsSourceIt->author << endl;
		return;
	}
	cout << "You have no favorite questions" << endl;
}

void User::viewQuestions(string title, string author) {
	vector<User>::iterator userIt;
	vector<Question>::iterator questionIt;
	searchAllQuestions(title, author, userIt, questionIt);
	if (userIt == users.end())
		throw NoSuchQuestionException();

	vector<string>::iterator seenUsersIt = questionIt->seenUsers.begin();
	vector<string>::iterator destSeenUsersIt = questionIt->seenUsers.end();
	for (seenUsersIt; seenUsersIt != destSeenUsersIt; seenUsersIt++)
		if (*seenUsersIt == this->username)
			break;
	if (seenUsersIt == destSeenUsersIt) { // this user still doesn't see question
		questionIt->numViews++;
		questionIt->seenUsers.emplace_back(this->username);
	}
	for (vector<Question>::iterator
		objfavoriteQuestionIt = this->thumbedUp.begin(), objDestfavoriteQuestionsIt = this->thumbedUp.end();
		objfavoriteQuestionIt != objDestfavoriteQuestionsIt;
		objfavoriteQuestionIt++)
		if (objfavoriteQuestionIt->title == title)
			cout << "you chose this question as your favorite one before" << endl;
	if (questionIt->duplicate == true)
		cout << "duplicat to: " << questionIt->duplicateTo.title << " from " << questionIt->duplicateTo.author << endl;
	cout << "title: " << questionIt->title << endl;
	cout << "content: " << questionIt->content << endl;
	cout << "tags: ";
	vector<string>::iterator tagsSourceIt = questionIt->tags.begin();
	vector<string>::iterator tagsDestIt = questionIt->tags.end();
	if (tagsSourceIt != tagsDestIt) { // prevent last excess comma such as in (a, b, )
		cout << *tagsSourceIt;
		tagsSourceIt++;
	}
	for (tagsSourceIt; tagsSourceIt != tagsDestIt; tagsSourceIt++)
		cout << ", " << *tagsSourceIt;
	cout << endl;
	cout << "asked " << questionIt->creationDate << endl;
	cout << "user: " << userIt->username << endl;
	for (vector<Edit>::iterator
		editsSourceIt = questionIt->editors.begin(), editsDestIt = questionIt->editors.end();
		editsSourceIt != editsDestIt;
		editsSourceIt++)
		cout << "edited " << editsSourceIt->editDate << endl << editsSourceIt->editorName << endl;
	cout << "viewed " << questionIt->numViews << " times" << endl;
	cout << "reputation: " << questionIt->numReputation << endl;
	cout << "favorites: " << questionIt->numFavorite << endl;

	cout << "answers:" << endl << endl;
	for (vector<Answer>::iterator
		answerSourceIt = questionIt->answers.begin(), answerDestIt = questionIt->answers.end();
		answerSourceIt != answerDestIt;
		answerSourceIt++) {
		cout << "content: " << answerSourceIt->content << endl;
		cout << "answered " << answerSourceIt->creationDate << endl;
		cout << "user: " << answerSourceIt->author << endl;
		for (vector<Edit>::iterator
			editsSourceIt = answerSourceIt->editors.begin(), editsDestIt = answerSourceIt->editors.end();
			editsSourceIt != editsDestIt;
			editsSourceIt++)
			cout << "edited " << editsSourceIt->editDate << endl << "by: " << editsSourceIt->editorName << endl;
		cout << "reputation: " << answerSourceIt->numReputation << endl;
		cout << endl;
	}
}

void User::changeStatus(string copyTitle, string copyAuthor, string originalTitle, string originalAuthor) {
	if (this->type != UserType::ADMIN)
		throw NotUserPropertyException();

	vector<User>::iterator userIt;
	vector<Question>::iterator copyQuestionIt;
	searchAllQuestions(copyTitle, copyAuthor, userIt, copyQuestionIt);
	if (userIt == users.end())
		throw NoSuchQuestionException();

	vector<Question>::iterator originalQuestionIt;
	searchAllQuestions(originalTitle, originalAuthor, userIt, originalQuestionIt);
	if (userIt == users.end())
		throw NoSuchQuestionException();

	copyQuestionIt->duplicateTo.title = originalTitle;
	copyQuestionIt->duplicateTo.author = originalAuthor;
	copyQuestionIt->duplicate = true;
	cout << "done :)" << endl;
}

void User::addAnswer(string questionTitle, string questionAuthor, string answerContent) {
	vector<User>::iterator userIt;
	vector<Question>::iterator questionIt;
	searchAllQuestions(questionTitle, questionAuthor, userIt, questionIt);
	if (userIt == users.end())
		throw NoSuchQuestionException();

	for (vector<Answer>::iterator
		answerSourceIt = questionIt->answers.begin(), answerDestIt = questionIt->answers.end();
		answerSourceIt != answerDestIt;
		answerSourceIt++)
		if (answerSourceIt->author == this->username) {
			cout << "You answered this question before" << endl;
			return;
		}

	questionIt->answers.emplace_back(this->username, answerContent, currentDateTime(), 0);
	cout << "done :)" << endl;
}

void User::editAnswer(string questionTitle, string questionAuthor, string answerAuthor, string answerNewContent, string description) {
	vector<User>::iterator usersIt;
	vector<Question>::iterator questionIt;
	searchAllQuestions(questionTitle, questionAuthor, usersIt, questionIt);
	if (usersIt == users.end())
		throw NoSuchQuestionException();

	vector<Answer>::iterator answerIt;
	searchQuestionAnswers(answerAuthor, answerIt, questionIt);
	if (answerIt == questionIt->answers.end())
		throw NoSuchAnswerException();

	else if (answerIt->author != this->username) { // need to be accept by admin
		for (vector<User>::iterator
			usersSourceIt = users.begin(), usersDestIt = users.end();
			usersSourceIt != usersDestIt;
			usersSourceIt++)
			if (usersSourceIt->type == UserType::ADMIN) {
				cout << "your request is sended to ADMIN " << usersSourceIt->username << endl;
				usersSourceIt->answerEditRequests.emplace_back(questionAuthor, questionTitle,
					answerAuthor, this->username, answerNewContent, currentDateTime(), description);
			}
	}

	else if (answerIt->author == this->username) { // don't need admin acception
		answerIt->content = answerNewContent;
		answerIt->editors.emplace_back(this->username, currentDateTime(), description);
		cout << "done :)" << endl;
	}
}

void User::handleAnswerEditRequest() {
	if (this->type != UserType::ADMIN)
		throw NotUserPropertyException();

	vector<answerEditRequest>::iterator editRequestSourceIt = this->answerEditRequests.begin();
	while (editRequestSourceIt != this->answerEditRequests.end()) {
		vector<User>::iterator userIt;
		vector<Question>::iterator questionIt;
		searchAllQuestions(editRequestSourceIt->questionTitle, editRequestSourceIt->questionAuthor, userIt, questionIt);

		vector<Answer>::iterator answerIt;
		searchQuestionAnswers(editRequestSourceIt->answerAuthor, answerIt, questionIt);

		cout << "main answer was:" << endl;
		cout << "content: " << answerIt->content << endl;
		cout << "answered " << answerIt->creationDate << endl;
		cout << "user: " << answerIt->author << endl;
		for (vector<Edit>::iterator
			previousEditsSourceIt = answerIt->editors.begin(), previousEditsDestIt = answerIt->editors.end();
			previousEditsSourceIt != previousEditsDestIt;
			previousEditsSourceIt++)
			cout << "edited " << previousEditsSourceIt->editDate << endl << "by: " << previousEditsSourceIt->editorName << endl;
		cout << "reputation: " << answerIt->numReputation << endl;
		cout << endl;

		cout << "edited answer is:" << endl;
		cout << "Edited: " << editRequestSourceIt->editDate << endl;
		cout << "By: " << editRequestSourceIt->editorName << endl;
		cout << "Body: " << editRequestSourceIt->editedContent << endl;
		cout << "Rev: " << editRequestSourceIt->description << endl;

		string choice;
		while (true) {
			cout << "will you accept this edit?(y:yes, n:no, e:exit): ";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			getline(cin, choice);
			if (choice == "y" || choice == "Y") {
				answerIt->content = editRequestSourceIt->editedContent;
				answerIt->editors.emplace_back(editRequestSourceIt->editorName,
					editRequestSourceIt->editDate, editRequestSourceIt->description);
				editRequestSourceIt = this->answerEditRequests.erase(editRequestSourceIt);
				cout << "done :)" << endl;
				break;
			}
			else if (choice == "n" || choice == "N") {
				editRequestSourceIt++;
				break;
			}
			else if (choice == "e" || choice == "E") {
				return;
			}
			else {
				cout << "not an option!" << endl;
			}
		}
	}
}

void User::deleteAnswer(string questionTitle, string questionAuthor, string answerAuthor) {
	vector<Question>::iterator questionIt;
	vector<User>::iterator userIt;
	searchAllQuestions(questionTitle, questionAuthor, userIt, questionIt);
	if (userIt == users.end())
		throw NoSuchQuestionException();

	vector<Answer>::iterator answerIt;
	searchQuestionAnswers(answerAuthor, answerIt, questionIt);
	if (answerIt == questionIt->answers.end())
		throw NoSuchAnswerException();

	if (this->username != answerIt->author) {
		cout << "this isn't your answer" << endl;
		return;
	}

	questionIt->answers.erase(answerIt);
	cout << "done :)" << endl;
}

void User::upvoteAnswer(string questionTitle, string questionAuthor, string answerAuthor) {
	vector<User>::iterator usersIt;
	vector<Question>::iterator questionIt;
	searchAllQuestions(questionTitle, questionAuthor, usersIt, questionIt);
	if (usersIt == users.end())
		throw NoSuchQuestionException();

	vector<Answer>::iterator answerIt;
	searchQuestionAnswers(answerAuthor, answerIt, questionIt);
	if (answerIt == questionIt->answers.end())
		throw NoSuchAnswerException();

	vector<Answer>::iterator answerSourceIt = this->upvotedAnswers.begin();
	vector<Answer>::iterator answerDestIt = this->upvotedAnswers.end();
	for (; answerSourceIt != answerDestIt; answerSourceIt++)
		if (answerSourceIt->author == answerAuthor)
			break;
	if (answerSourceIt != answerDestIt)
		cout << "you upvoted this answer before" << endl;
	else {
		vector<Answer>::iterator downvotedSourceIt = this->downvotedAnswers.begin();
		vector<Answer>::iterator downvotedDestIt = this->downvotedAnswers.end();
		for (downvotedSourceIt; downvotedSourceIt != downvotedDestIt; downvotedSourceIt++)
			if (downvotedSourceIt->author == answerAuthor) {
				this->downvotedAnswers.erase(downvotedSourceIt);
				answerIt->numReputation++;
				break;
			}
		if (downvotedSourceIt == downvotedDestIt) {
			answerIt->numReputation++;
			this->upvotedAnswers.emplace_back
			(answerIt->author,answerIt->content,answerIt->creationDate,answerIt->numReputation);
		}
	}
}

void User::downvoteAnswer(string questionTitle, string questionAuthor, string answerAuthor) {
	vector<User>::iterator usersIt;
	vector<Question>::iterator questionIt;
	searchAllQuestions(questionTitle, questionAuthor, usersIt, questionIt);
	if (usersIt == users.end())
		throw NoSuchQuestionException();

	vector<Answer>::iterator answerIt;
	searchQuestionAnswers(answerAuthor, answerIt, questionIt);
	if (answerIt == questionIt->answers.end())
		throw NoSuchAnswerException();

	vector<Answer>::iterator answerSourceIt = this->downvotedAnswers.begin();
	vector<Answer>::iterator answerDestIt = this->downvotedAnswers.end();
	for (; answerSourceIt != answerDestIt; answerSourceIt++)
		if (answerSourceIt->author == answerAuthor)
			break;
	if (answerSourceIt != answerDestIt)
		cout << "you downvoted this answer before" << endl;
	else {
		vector<Answer>::iterator upvotedSourceIt = this->upvotedAnswers.begin();
		vector<Answer>::iterator upvotedDestIt = this->upvotedAnswers.end();
		for (; upvotedSourceIt != upvotedDestIt; upvotedSourceIt++)
			if (upvotedSourceIt->author == answerAuthor) {
				this->upvotedAnswers.erase(upvotedSourceIt);
				answerIt->numReputation--;
				break;
			}
		if (upvotedSourceIt == upvotedDestIt) {
			answerIt->numReputation--;
			this->downvotedAnswers.emplace_back
			(answerIt->author, answerIt->content, answerIt->creationDate, answerIt->numReputation);
		}
	}
}

