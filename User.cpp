#include "User.h"
#include <iostream>

string User::getLogin() const
{
    return login;
}

void User::setLogin(const string& login)
{
    this->login = login;
}

string User::getPassword() const
{
    return password;
}

void User::setPassword(const string& password)
{
    this->password = password;
}

ostream& operator<<(ostream& os, const User& user)
{
    os << user.login << " " << user.password;
    return os;
}

istream& operator>>(istream& is, User& user)
{
    is >> user.login >> user.password;
    return is;
}