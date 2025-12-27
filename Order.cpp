#include "Order.h"
#include "Vehicle.h"
#include <iostream>
#include <iomanip>

int Order::nextId = 1;
void Order::setNextId(int id) {nextId = id;}
int Order::getNextId() {return nextId;}
void Order::calculateTotal()
{
    if (vehicle && days > 0){totalPrice = vehicle->calculateRentPrice(days);}
    else {totalPrice = 0.0;}
}

void Order::approve()
{
    if (vehicle) {vehicle->setIsAvailable(false); }
    status = Status::APPROVED;
    rejectReason.clear();
    std::cout << "Order #" << id << " approved." << std::endl;
}

void Order::reject(const std::string& reason)
{
    status = Status::REJECTED;
    rejectReason = reason;
    std::cout << "Order #" << id << " rejected. Reason: " << reason << std::endl;
}

int Order::getId() const {return id;}

void Order::setId(int newId) {id = newId;}

void Order::setTotalPrice(double price) {totalPrice = price;}

void Order::setDays(int newDays) {days = newDays;}

void Order::setRejectReason(const std::string& reason) {rejectReason = reason;}

void Order::setReturnStatus(ReturnStatus status){returnStatus = status;}

Client* Order::getClient() const{return client;}

Vehicle* Order::getVehicle() const{return vehicle;}

Status Order::getStatus() const{return status;}

double Order::getTotalPrice() const{return totalPrice;}

std::string Order::getRejectReason() const{return rejectReason;}

int Order::getDays() const{return days;}

void Order::setStatus(Status newStatus){status = newStatus;}

void Order::returnVehicle(ReturnStatus returnStatus)
{
    this->returnStatus = returnStatus;
    
    if (vehicle)
    {
        double estimatedMileage = days * 100.0; 
        double currentMileage = vehicle->getMileage();
        vehicle->setMileage(currentMileage + estimatedMileage);
    }
    status = Status::COMPLETED;
}

ReturnStatus Order::getReturnStatus() const { return returnStatus; }

int Order::getClientId() const 
{
    if (client) {return client->getId();}
    return clientId;
}

int Order::getVehicleId() const 
{
    if (vehicle) {return vehicle->getId();}
    return vehicleId;
}

void Order::setClientId(int id) {clientId = id;}

void Order::setVehicleId(int id) {vehicleId = id;}

void Order::setClient(Client* client)
{
    this->client = client;
    if (client) {clientId = client->getId();}
}

void Order::setVehicle(Vehicle* vehicle)
{
    this->vehicle = vehicle;
    if (vehicle) {vehicleId = vehicle->getId();}
}

void Order::printHeader() const
{
    std::cout << std::left << std::setw(8) << "Order ID"
        << std::left << std::setw(25) << "Client"
        << std::left << std::setw(25) << "Vehicle"
        << std::left << std::setw(8) << "Days"
        << std::left << std::setw(15) << "Total Price"
        << std::left << std::setw(15) << "Status"
        << std::left << std::setw(30) << "Reject Reason"
        << std::endl;
}

void Order::printInfo() const
{
    std::string clientName = "";
    if (client) {
        clientName = client->getFirstName() + " " + client->getLastName();
    } else {clientName = "N/A";}
    
    std::string vehicleInfo = "";
    if (vehicle) {
        vehicleInfo = vehicle->getBrand() + " " + vehicle->getModel();
    } else {vehicleInfo = "N/A";}
    
    std::string statusStr;
    switch (status) {
        case Status::PENDING: statusStr = "Pending"; break;
        case Status::APPROVED: statusStr = "Approved"; break;
        case Status::REJECTED: statusStr = "Rejected"; break;
        case Status::COMPLETED: statusStr = "Completed"; break;
        case Status::CANCELLED: statusStr = "Cancelled"; break;
    }
    std::cout << std::left << std::setw(8) << id
        << std::left << std::setw(25) << clientName
        << std::left << std::setw(25) << vehicleInfo
        << std::left << std::setw(8) << days
        << std::left << std::setw(15) << std::fixed << std::setprecision(2) << totalPrice
        << std::left << std::setw(15) << statusStr
        << std::left << std::setw(30) << (rejectReason.empty() ? "-" : rejectReason)
        << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Order& order)
{
    int statusValue = static_cast<int>(order.status);
    int returnStatusValue = static_cast<int>(order.returnStatus);
    int clientId = order.getClientId();
    int vehicleId = order.getVehicleId();
    int orderId = order.getId();
    int days = order.getDays();
    double totalPrice = order.getTotalPrice();
    std::string rejectReason = order.getRejectReason();

    os << orderId << " " << clientId << " " << vehicleId << " " 
       << days << " " << totalPrice << " " 
       << statusValue << " " << rejectReason.length() << " ";

    if (!rejectReason.empty()) {
        os << rejectReason;
    }
    os << " " << returnStatusValue;
    return os;
}

std::istream& operator>>(std::istream& is, Order& order)
{
    int orderId, statusValue, returnStatusValue, reasonLength;
    int clientId, vehicleId, days;
    double totalPrice;
    is >> orderId >> clientId >> vehicleId 
       >> days >> totalPrice >> statusValue >> reasonLength;
    order.setId(orderId);
    order.setClientId(clientId);
    order.setVehicleId(vehicleId);
    order.setDays(days);
    order.setTotalPrice(totalPrice);
    is.ignore(1);
    if (reasonLength > 0 && reasonLength < 10000) {  
        std::string reason;
        reason.resize(reasonLength);
        is.read(&reason[0], reasonLength);
        order.setRejectReason(reason);
    } else {
        order.setRejectReason("");
    }
    if (is.peek() == ' ') {
        is.ignore(1);
    }
    is >> returnStatusValue;
    order.setStatus(static_cast<Status>(statusValue));
    order.setReturnStatus(static_cast<ReturnStatus>(returnStatusValue));
    return is;
}


