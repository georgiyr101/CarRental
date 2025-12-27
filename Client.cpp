#include "Client.h"
#include <iostream>
#include <iomanip>

int Client::getId() const {return id;}

void Client::setId(int id){this->id = id;}

string Client::getFirstName() const{return firstName;}

string Client::getLastName() const{return lastName;}

string Client::getPassport() const{return passport;}

string Client::getPhoneNumber() const{return phoneNumber;}

string Client::getEmail() const{return email;}

string Client::getDateOfBirth() const{return dateOfBirth;}

string Client::getDriverLicense() const{return driverLicense;}

double Client::getTotalSpent() const{ return totalSpent;}

int Client::getRentalCount() const{return rentalCount;}

void Client::setFirstName(const string& name){this->firstName = name;}

void Client::setLastName(const string& name){this->lastName = name;}

void Client::setPassport(const string& passport){this->passport = passport;}

void Client::setPhoneNumber(const string& phone){this->phoneNumber = phone;}

void Client::setEmail(const string& email){this->email = email;}

void Client::setDateOfBirth(const string& date){this->dateOfBirth = date;}

void Client::setDriverLicense(const string& license){this->driverLicense = license;}

void Client::setTotalSpent(double spent){this->totalSpent = spent;}

void Client::setRentalCount(int count){this->rentalCount = count;}

void Client::addToTotalSpent(double amount){totalSpent += amount;}

void Client::incrementRentalCount(){rentalCount++;}

void Client::printHeader() const
{
    cout << left << setw(6) << "ID"
        << left << setw(15) << "Login"
        << left << setw(15) << "First Name"
        << left << setw(15) << "Last Name"
        << left << setw(15) << "Passport"
        << left << setw(15) << "Phone"
        << left << setw(20) << "Email"
        << left << setw(15) << "Date of Birth"
        << left << setw(15) << "Driver License"
        << left << setw(12) << "Total Spent"
        << left << setw(10) << "Rentals"
        << endl;
}

void Client::printInfo() const
{
    cout << left << setw(6) << id
        << left << setw(15) << getLogin()
        << left << setw(15) << firstName
        << left << setw(15) << lastName
        << left << setw(15) << passport
        << left << setw(15) << phoneNumber
        << left << setw(20) << email
        << left << setw(15) << dateOfBirth
        << left << setw(15) << driverLicense
        << left << setw(12) << fixed << setprecision(2) << totalSpent
        << left << setw(10) << rentalCount
        << endl;
}

ostream& operator<<(ostream& os, const Client& client)
{
    os << client.id << " ";
    os << static_cast<const User&>(client);
    os << " " << client.firstName << " " << client.lastName << " " << client.passport << " "
       << client.phoneNumber << " " << client.email << " " << client.dateOfBirth << " "
       << client.driverLicense << " " << client.totalSpent << " " << client.rentalCount;
    return os;
}

istream& operator>>(istream& is, Client& client)
{
    is >> client.id;
    is >> static_cast<User&>(client);
    cout << "First name: ";
    is >> client.firstName;
    cout << "Last name: ";
    is >> client.lastName;
    cout << "Passport: ";
    is >> client.passport;
    cout << "Phone: ";
    is >> client.phoneNumber;
    cout << "Email: ";
    is >> client.email;
    cout << "Date of birth: ";
    is >> client.dateOfBirth;
    cout << "Driver license: ";
    is >> client.driverLicense;
    cout << "Total spent: ";
    is >> client.totalSpent;
    cout << "Rental count: ";
    is >> client.rentalCount;
    return is;
}