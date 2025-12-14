#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
using namespace std;

class User
{
protected:
	string login;
	string password;

public:
	User() {}
	User(const string& login, const string& password) : login(login), password(password){}
	virtual ~User() = default;

	string getLogin() const;
	void setLogin(const string& login);

	string getPassword() const;
	void setPassword(const string& password);

	friend ostream& operator<<(ostream& os, const User& user);
	friend istream& operator>>(istream& is, User& user);
};

#endif
