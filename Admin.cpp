#include "Admin.h"
#include "Order.h"
#include "Vehicle.h"
#include "Input.h"
#include <iostream>

void Admin::rejectOrder(Order& order, const string& reason)
{
    order.reject(reason);
    if (order.getVehicle()){order.getVehicle()->setIsAvailable(true);}
}

void Admin::approveOrder(Order& order)
{
    order.approve();
    if (order.getClient())
    {
        order.getClient()->incrementRentalCount();
        order.getClient()->addToTotalSpent(order.getTotalPrice());
    }
}
void Admin::manageVehicle(Vehicle* vehicle, bool makeAvailable)
{
    if (vehicle) {vehicle->setIsAvailable(makeAvailable);}
}