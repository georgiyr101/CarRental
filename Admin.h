#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include "Order.h"
#include "Vehicle.h"
using namespace std;
class Admin : public User
{
public:
	Admin() { login = "login", password = "password"; }
	Admin(const string& login, const string& password): User(login, password){}
	virtual ~Admin() = default;
	void rejectOrder(Order& order, const string& reason);
	void approveOrder(Order& order); // Approve order
	void manageVehicle(Vehicle* vehicle, bool makeAvailable); // Manage vehicle availability
};

#endif