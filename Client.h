#ifndef CLIENT_H
#define CLIENT_H

#include "User.h"

class Client : public User
{
private:
	int id;
	string firstName;
	string lastName;
	string passport;
	string phoneNumber;        // Номер телефона
	string email;              // Email
	string dateOfBirth;        // Дата рождения
	string driverLicense;      // Номер водительских прав
	double totalSpent;         // Общая сумма потраченная на аренду
	int rentalCount;           // Количество аренд

public:
	Client() : id(0) {}
	Client(int id, const string& login, const string& password,
		const string& firstName, const string& lastName, const string& passport,
		const string& phoneNumber = "", const string& email = "",
		const string& dateOfBirth = "", const string& driverLicense = "",  double totalSpent = 0.0, int rentalCount = 0) :
		User(login, password), id(id), firstName(firstName), lastName(lastName), passport(passport),
		phoneNumber(phoneNumber), email(email), dateOfBirth(dateOfBirth),
		driverLicense(driverLicense), totalSpent(totalSpent),
		rentalCount(rentalCount) {}
	virtual ~Client() = default;

	// Геттеры
	int getId() const;
	void setId(int id);
	string getFirstName() const;
	string getPassport() const;
	string getLastName() const;
	string getPhoneNumber() const;
	string getEmail() const;
	string getDateOfBirth() const;
	string getDriverLicense() const;
	double getTotalSpent() const;
	int getRentalCount() const;

	// Сеттеры
	void setFirstName(const string& name);
	void setLastName(const string& name);
	void setPassport(const string& passport);
	void setPhoneNumber(const string& phone);
	void setEmail(const string& email);
	void setDateOfBirth(const string& date);
	void setDriverLicense(const string& license);
	void setTotalSpent(double spent);
	void setRentalCount(int count);

	// Новые методы
	void addToTotalSpent(double amount);   // Добавить к общей сумме
	void incrementRentalCount();            // Увеличить счетчик аренд

	void printHeader() const;
	void printInfo() const;

	friend ostream& operator<<(ostream& os, const Client& client);
	friend istream& operator>>(istream& is, Client& client);
};

#endif
