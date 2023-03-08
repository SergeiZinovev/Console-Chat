#pragma once
#include <vector>
#include <exception>
#include <memory>
#include "Message.h"
struct UserLoginExp :public std::exception {// call the exception class in case the user all
	const char* what() const noexcept override {
		return "error: user login is busy";
	}

};
struct UserNameExp :public std::exception {//call the exception class in case the user all
	const char* what()const noexcept override {
		return "error: user name is busy";
	}
};
class Chat {
	bool isChatWork_ = false;//variable responsible for the chat operation
	std::vector<User> userList_;//create a vector that stores the chat users
	std::vector<Message> messageList_;//create a vector that stores chat messages
	std::shared_ptr<User> currentUser_ = nullptr;//smart pointer points to the current user

	void login();
	void signUp();
	void showChat()const;
	void showAllUsersName()const;
	void addMessage();

	std::vector<User>& getAllUsers() {
		return userList_;
	}
	std::vector<Message>& getAllMessages() {
		return messageList_;
	}
	std::shared_ptr<User>getUserByLogin(const std::string& login)const;
	std::shared_ptr<User>getUserByName(const std::string& name)const;
public:
	void start();
	bool isChatWork()const {
		return isChatWork_;
	}
	std::shared_ptr<User>getCurrentUser()const {
		return currentUser_;
	}
	void showLoginMenu();
	void showUserMenu();
};
