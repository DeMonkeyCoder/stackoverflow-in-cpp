#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <map>

#define lower(str) transform(str.begin(), str.end(), str.begin(), ::tolower)
#define questionTagsSize 20
#define questionAnswersSize 20
#define usersSeenQuestionSize 20
#define questionEditorsSize 20

using namespace std;

class AbstractUser;
class Answer;
class Edit;
class EditRequest;

enum UserType {
	ADMIN,
	MEMBER
};

class Duplicate {
public:
	string author;
	string title;
};

class Answer {
public:
	string author;
	string content;
	string creationDate;
	vector<Edit> editors;
	int numReputation;

	Answer(string au, string co, string cd, int nr)
		:author(au), content(co), creationDate(cd), numReputation(nr) {
		editors.reserve(questionEditorsSize);
	}
};

class Edit {
public:
	string editorName;
	string editDate;
	string description;

	Edit(string en, string ed, string dc) :editorName(en), editDate(ed), description(dc) {}
};

class questionEditRequest {
public:
	string questionAuthor;
	string questionTitle;
	string editorName;
	string editedTitle;
	string editedContent;
	vector<string> editedTags;
	string editDate;
	string description;

	questionEditRequest(string qa, string qt, string en, string et, string ec, vector<string> ta, string ed, string ds)
		:questionAuthor(qa), questionTitle(qt), editorName(en), editedTitle(et), editedContent(ec), editDate(ed), description(ds) {
		editedTags.reserve(questionTagsSize);
		editedTags.insert(editedTags.end(), ta.begin(), ta.end());
	}
};

class answerEditRequest {
public:
	string questionAuthor;
	string questionTitle;
	string answerAuthor;
	string editorName;
	string editedContent;
	string editDate;
	string description;

	answerEditRequest(string qa, string qt, string aa, string en, string ec, string ed, string ds)
		:questionAuthor(qa), questionTitle(qt), answerAuthor(aa), editorName(en), editedContent(ec), editDate(ed), description(ds) {}
};

class AbstractUser {
public:
	// Login related
	hash<string> pass_hash;
	string username;

	// Login related
	virtual bool authenticate(string username, string password) = 0;
	virtual void deleteAccount() = 0;

	// Question related
	virtual void createQuestion(string title, string content, vector<string> tags) = 0;
	virtual void editQuestion(string title, string author,
		string newTitle, string newContent, vector<string> newTags, string description) = 0;
	virtual void handleQuestionEditRequests() = 0; // for admin
	virtual void deleteQuestion(string title, string author) = 0;
	virtual void upvoteQuestion(string title, string author) = 0;
	virtual void downvoteQuestion(string title, string author) = 0;
	virtual void addQuestionToFavs(string title, string author) = 0;
	virtual void deleteQuestionFromFavs(string title, string author) = 0;
	virtual void viewFavQuestions() = 0;
	virtual void viewQuestions(string title, string author) = 0;
	virtual void changeStatus(string copyTitle, string copyAuthor, string originalTitle, string originalAuthor) = 0; // for admin

	// Answer related
	virtual void addAnswer(string questionTitle, string questionAuthor, string answerContent) = 0;
	virtual void editAnswer(string questionTitle, string questionAuthor,
		string answerAuthor, string answerNewContent, string description) = 0;
	virtual void handleAnswerEditRequest() = 0; // for admin
	virtual void deleteAnswer(string questionTitle, string questionAuthor, string answerAuthor) = 0;
	virtual void upvoteAnswer(string questionTitle, string questionAuthor, string answerAuthor) = 0;
	virtual void downvoteAnswer(string questionTitle, string questionAuthor, string answerAuthor) = 0;
	
protected:
	class Question {
	public:
		string author;
		string title;
		string content;
		vector<string> tags;
		string creationDate;
		Duplicate duplicateTo;
		vector<string> seenUsers;
		vector<Edit> editors;
		vector<Answer> answers;
		int numReputation;
		int numFavorite;
		int numViews;
		bool duplicate;

		Question(string au, string ti, string co, string cd, vector<string> tg, int re, int nv, int nf, int du)
			:author(au), title(ti), content(co), creationDate(cd), numReputation(re),
			numViews(nv), numFavorite(nf), duplicate(du) {
			seenUsers.reserve(usersSeenQuestionSize);
			editors.reserve(questionEditorsSize);
			tags.reserve(questionTagsSize);
			answers.reserve(questionAnswersSize);
			tags.insert(tags.end(), tg.begin(), tg.end());
		}

		Question() {}
		bool operator ==(Question q) {
			return this->title == q.title;
		}
	};

	// Login related
	string password;
	string email;
	UserType type;

	// Question related
	vector<Question> upvoted;
	vector<Question> downvoted;
	vector<Question> thumbedUp;
	vector<Question> thumbedDown;
	vector<questionEditRequest> questionEditRequests;

	// Answer related
	vector<Answer> upvotedAnswers;
	vector<Answer> downvotedAnswers;
	vector<answerEditRequest> answerEditRequests;

	// common between Answer and Question
	map< string, vector<Question> > contents;
};