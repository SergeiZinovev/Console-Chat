#include<iostream>
#include <string>
#include "chat.h"
void Chat::start() {
	isChatWork_ = true;
}
std::shared_ptr<User>Chat::getUserByLogin(const std::string& login) const {//get user's login by his name
	for (auto& user : userList_) {//go through the whole vector
		if (login == user.GetUserLogin())//if reference to login is equal to the value retirned the GetUserLogin function
			return std::make_shared<User>(user);//then we return a shared-pointer to that user
	}
	return nullptr;
}
std::shared_ptr<User>Chat::getUserByName(const std::string& name)const { 
	for (auto& user : userList_) {
		if (name == user.GetUserName())
			return std::make_shared<User>(user);
	}
	return nullptr;
}
void Chat::showLoginMenu() {
	currentUser_ = nullptr;//send the value nullptr to the field the current user
	char operation;
	do {
		std::cout << "\033[33m" << "(1)Login" << std::endl;
		std::cout << "(2)SignUp" << std::endl;
		std::cout << "(0)Shutdown" << std::endl;
		std::cout << "\033[36m" << ">>" << "\033[0m";
		std::cin >> operation;
		switch (operation) {
		case '1':
			login();//log in with existing username and password
			break;
		case '2':
			try {
				signUp();
			}
			catch (const std::exception& e) {//catch the exception raised in the sighup function
				std::cout << e.what() << std::endl;
			}
			break;
		case '0':
			isChatWork_ = false;
			break;
		default:
			std::cout << "1 or 2..." << std::endl;
			break;

		}
	} while (!currentUser_ && isChatWork_);//the loop works as long as the user is not registered and as long as chat works
}
void Chat::login() {
	std::string login, password;
	char operation;

	do {
		std::cout << "login:";
		std::cin >> login;
		std::cout << "password:";
		std::cin >> password;

		currentUser_ = getUserByLogin(login);//a pointer to the registered user
		if (currentUser_ == nullptr || (password != currentUser_->GetUserPassword()))//if the user is not in the vector or if password is incorrect  we return an empty pointer 
		{
			currentUser_ = nullptr;
			std::cout << "login failed..." << std::endl;
			std::cout << "(0)exit or (any key) repeat: ";
			std::cin >> operation;
			if (operation == '0')
				break;
		}
	} while (!currentUser_);
}

void Chat::showChat()const {
	std::string from;
	std::string to;
	std::cout << "---Chat---" << std::endl;
	for (auto& mess : messageList_) {
		if (currentUser_->GetUserLogin() == mess.GetFrom() || currentUser_->GetUserLogin() == mess.GetTo() || mess.GetTo() == "all") {//if the current user
			from = (currentUser_->GetUserLogin() == mess.GetFrom()) ? "me" : getUserByLogin(mess.GetFrom())->GetUserName();
			if (mess.GetTo() == "all") {//if to equals all we address a message to all users
				to = "all";
			}
			else {
				to = (currentUser_->GetUserLogin() == mess.GetTo()) ? "me" : getUserByLogin(mess.GetTo())->GetUserName();
			//if our current username is to we send message to ourselves, ig not we get username and assign its value to the to field
			}
			std::cout << "Message from " << from << " to " << to << std::endl;
			std::cout << " text:" << mess.GetText() << std::endl;
		}
	}
	std::cout << "-----------" << std::endl;

}
void Chat::signUp() {
	std::string login, password, name;
	double time;
	std::cout << "login: ";
	std::cin >> login;
	std::cout << "password:";
	std::cin >> password;
	std::cout << "name:";
	std::cin >> name;
	if (getUserByLogin(login) || login == "all") //if login - all, there is an error
	{
		throw UserLoginExp();
	}
	if (getUserByName(name) || name == "all") {//if name - all, there is an error
		throw UserNameExp();

	}
	User user = User(login, password, name, time);//create an object of class User
	userList_.push_back(user);//add a user to vector
	currentUser_ = std::make_shared<User>(user);//create a pointer to the current user
}
void Chat::showUserMenu() {
	char operation;
	std::cout << "Hi, " << currentUser_->GetUserName() << std::endl;
	while (currentUser_) {
		std::cout << "Menu: (1)ShowChat| (2)Add message|(3)Users|(0)Logout" << std::endl;
		std::cout << std::endl;
		std::cin >> operation;
		switch (operation) {
		case '1':
			showChat();
			break;
		case '2':
			addMessage();
			break;
		case '3':
			showAllUsersName();
			break;
		case'0':
			currentUser_ = nullptr;
			break;
		default:
			std::cout << "unknown choice" << std::endl;
			break;
		}
	}
}
void Chat::addMessage() {
	std::string to, text;

	std::cout << "To (name or all)";
	std::cin >> to;
	std::cout << "Text: ";
	std::cin.ignore();
	getline(std::cin, text);
	if (!(to == "all" || getUserByName(to))) {// send a message to everyone or send messagges to a user who is already in the vector
		std::cout << "error send message: can't find" << to << std::endl;
		return;
	}
	if (to == "all")
		messageList_.push_back(Message{ currentUser_->GetUserLogin(),"all",text });// place an object of class Message in the conctractor of which we specify a reference to the current user, message and to whom it is addressed
	else
		messageList_.push_back(Message{ currentUser_->GetUserLogin(),getUserByName(to)->GetUserLogin(),text });

}
void Chat::showAllUsersName()const {
	std::cout << "---Users---" << std::endl;
	for (auto& user : userList_) {
		std::cout << user.GetUserName();
		if (currentUser_->GetUserLogin() == user.GetUserLogin())
			std::cout << "(me)";
		std::cout << std::endl;

	}
	std::cout << "-----------" << std::endl;
}