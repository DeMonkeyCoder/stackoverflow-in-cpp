#include <iostream>
#include <vector>
#include <exception>
#include <fstream>
#include <ctime>
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
    string email;
    UserType type;
};


class User : public AbstractUser{
public:
    User(string username, string password, UserType type,string email){
        this->username = username; 
        this->password = password;
        this->type = type;
        this-> email = email;
    }

    bool authenticate(string username, string password){
    	return this->username == username && this->password == password;
    }

    static string user_information(User * user ){
    	time_t now = time(0);
 	char* dt = ctime(&now);
        tm *gmtm = gmtime(&now);
        dt = asctime(gmtm);
        return user->email +"    " + user->username + "     "+ dt;;
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


    static User* signup(vector<AbstractUser*> *users, string username, string password,string email){
	for( auto user = users->begin(); user != users->end(); user++) { 
       		if((*user)->authenticate(username, password)) {    //Fixing the function
                	UserAlreadyExistsException ex;
                	throw ex;
            	}
        }

        User * new_user = new User(username, password, UserType::MEMBER,email);
        users->push_back(new_user);
        return new_user;
    }

    string username;

};

enum MenuState{
    START,
    LOGGED_IN,
    END
};



class Post{
    public:
    int id;
    string  username;
    string  question;
    vector <string> answers;
    int visited_counter ;
    Post(int id,string username , string question ,vector <string> _answers){
        this -> id = id ;
        this -> username = username ;
        this -> question = question;
        for (int i = 0 ; i < _answers.size() ;i++){
            answers.push_back(_answers[i]);
        }
        this ->visited_counter = 0;
    }
};


void store_questions_answers(vector <Post*> posts){
    ofstream questions_file;
    questions_file.open("questions.txt");
    for (int i = 0 ; i < posts.size() ;i++){
        questions_file << "VISITED : " << posts[i]-> visited_counter   << " TIMES" << endl;
        questions_file << "==============================================" << endl;
        questions_file << posts[i]->username << endl;
        questions_file << i+1 <<" " << posts[i]->question << endl;
        for (int j = 0 ; j < posts[i]->answers.size() ;j++){
            questions_file << posts[i]->answers[j] << endl;
        }
        questions_file << "==============================================" << endl;
    }
}


void store_post(string username,vector <Post *> posts){
    ofstream post_file;
    string file_name = username+".txt";
    post_file.open(file_name);
    for (int i = 0 ; i < posts.size() ;i++){
        post_file << "VISITED : " << posts[i]-> visited_counter   << " TIMES" << endl;
        if (posts[i]->username ==username){
            post_file << i+1 << "  " << posts[i]->question << endl;
            for (int j = 0 ; j < posts[i]->answers.size() ;j++){
            post_file << posts[i]->answers[j] << endl;
            }
        }
        post_file << "==============================================" << endl;
    }
}

void set_id(vector <Post *> posts){
    for (int i = 0 ; i < posts.size() ;i++){
        posts[i]-> id = i+1;
    }
}

int  get_id(vector <Post *> posts){
    return posts.size() ;
}

class AppDatabase { 
public:
    vector<AbstractUser *> appUsers;
    vector<Post*> posts;
    AppDatabase() { 
        appUsers.push_back(new User("abc","123" ,UserType::ADMIN,"YasnaKatebzadeh@gmail.com"));
        vector <string> admin_answers ;
        admin_answers.push_back("a");
        admin_answers.push_back("b");
	Post * admin_post = new Post(1,"abc","How can we ask questions in stackoverflow ?",admin_answers);
        posts.push_back(admin_post);
    }
};

int main(){

    User * loggedInUser = nullptr;
    User * new_user = nullptr;
    AppDatabase appDatabase;
    store_questions_answers(appDatabase.posts);
    store_post("yasnakateb",appDatabase.posts);
    set_id(appDatabase.posts);
    MenuState menuState = MenuState::START;
    
    char choice;
    cout << "Welcome!" << endl;
    int user_counter = 1;
    while(menuState != MenuState::END){
        switch (menuState){
            case MenuState::START: {

                cout << "1. login\n2. signup\ne. exit\n";
                cin >> choice;
                switch(choice) {
                    case '1': {
                        string username, password,email;
                        cout << "Enter Username" << endl;
                        cin >> username;
                        cout << "Enter Password" << endl;
                        cin >> password;
                        
                        LOGGED_IN_STATE :  
                        try{ 
                                                 //Handling user login errors with exceptions
                            loggedInUser = User::login(&appDatabase.appUsers, username, password);
                            menuState = MenuState::LOGGED_IN;
                            string user_info = User::user_information(loggedInUser);
                            ofstream  user_file;
                            string file_name = "log." + to_string (user_counter) + ".txt";
                            user_file.open(file_name);
                            user_file << user_info;
                            user_file.close();
                            user_counter ++;
			    char command;
                            cout << "1.See all posts" << endl;
                            cout << "2.Ask a question" << endl;
                            cout << "b.back" << endl;
                            cin >> command ;
                            switch(command){
                                case '1':
                                {
                                    ifstream questions_file;
                                    questions_file.open("questions.txt");
                                    char str[255];
				    while(questions_file) {
                                    questions_file.getline(str, 255);  // delim defaults to '\n'
                                    if(questions_file) cout << str << endl;
				    }
                                    

                                    questions_file.close();

                                    char post_command ;
                                    int id;
                                    string new_answer;
                                  
                                    cout << "1.Answer a question" <<endl;
                                    cout << "2.Update your post" << endl;
                                    cout << "3.Delete your post" << endl;
                                    cout << "4.Setting" << endl;
                                    cout << "b.back" << endl;
                                    
                                    cin >> post_command;
                                    switch(post_command){
                                        case '1':{
                                            cout << "Enter question's ID" << endl;
                                            cin >> id;
                                            for (int i = 0 ; i < appDatabase.posts.size();i++){
                                                if (appDatabase.posts[i]->id ==id ){
                                                    appDatabase.posts[i]->visited_counter ++;
                                                    cout << "Enter your answer" << endl;
                                                    cin >> new_answer;
                                                    appDatabase.posts[i]->answers.push_back(new_answer);
                                                    
                                                }
                                            }

                                            store_questions_answers(appDatabase.posts);
                                            store_post(username,appDatabase.posts);
                                            set_id(appDatabase.posts);


                                        }
                                        break;
                                        case '2':{
                                            int id ;
                                            string new_question;
                                            cout << "These are your posts" << endl;
                                            cout << "==============================================" << endl;
                                            ifstream user_post_file;
                                            string file_name = username+".txt";
                                            user_post_file.open(file_name);
                                            char str[255];

                                            while(user_post_file) {
                                                user_post_file.getline(str, 255);  // delim defaults to '\n'
                                                if(user_post_file) cout << str << endl;

                                            }

                                            cout << "Enter question's ID" << endl;
                                            cin >> id ;
                                            for (int i = 0 ; i < appDatabase.posts.size();i++){
                                                if (appDatabase.posts[i]->id ==id ){
                                                    cout << "Enter your question" << endl;
                                                    cin >> new_question;
                                                    vector <string> _answers;
                                                    Post *new_post  = new Post(id,username ,new_question , _answers);
                                                    appDatabase.posts[i] = new_post;
                                                    
                                                }
                                            }
                                            store_questions_answers(appDatabase.posts);
                                            store_post(username,appDatabase.posts);
                                            set_id(appDatabase.posts);



                                        }
                                        break;
                                        case '3':{
                                            int id;
                                            cout << "Enter question's ID" << endl;
                                            cin >> id ;
                                            for (int i = 0 ; i < appDatabase.posts.size();i++){
                                                if (appDatabase.posts[i]->id ==id ){
                                                    appDatabase.posts.erase (appDatabase.posts.begin()+i);
                                                    

                                                }
                                            }
                                                store_questions_answers(appDatabase.posts);
                                                store_post(username,appDatabase.posts);
                                                set_id(appDatabase.posts);

                                        }

                                        break;

                                        case '4':{
                                            menuState = MenuState :: LOGGED_IN;
                                            continue;
                                        }
                                        break;

                                        case 'b':{
                                            goto LOGGED_IN_STATE;
                                        }
                                        break;
                                    
                                    }

 
                                }
                                break;

                                case '2':{
                                    string new_question;
                                    cout << "Enter your question" << endl;
                                    cin >> new_question;
                                    vector <string> _answers;
                                    int id = get_id(appDatabase.posts);
                                    Post *new_post  = new Post(id,username ,new_question , _answers);
                                    appDatabase.posts[id] = new_post;
                                    store_questions_answers(appDatabase.posts);
                                    store_post(username,appDatabase.posts);
                                    set_id(appDatabase.posts);
                                }
                                break;


                                case 'b':{
                                    menuState = MenuState::START;
                                    continue;
                                }

                            }
                            
                        }catch(LoginException login_exception){
                        	cout << login_exception.what() << endl;
                            
                        }
                            
                        break;
                        }
                    
                    case '2': {
                        string username, password ,email;
                        cout << "Enter Username" << endl;
                        cin >> username;
                        cout << "Enter Password" << endl;
                        cin >> password;
                        cout << "Enter Email" << endl;
                        cin >> email;
                        try{
                            new_user = User::signup(&appDatabase.appUsers, username, password,email);
                            string user_info = User::user_information(new_user);
                            ofstream  user_file;    
                            string file_name = "log." + to_string (user_counter) + ".txt";
                            user_file.open(file_name);
                            user_file << user_info;
                            user_file.close();
                            user_counter ++;
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
                cout << "b.back\nd.delete account\nl. logout\ne. exit\n";
                cin >> choice;
                switch(choice) {
                    case 'b':{
                        menuState = MenuState::START;
                    }
                    break;
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


