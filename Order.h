#pragma once
#include <string>
#include "Client.h"
#include "Vehicle.h"

enum class Status { PENDING, APPROVED, REJECTED, COMPLETED, CANCELLED };
enum class ReturnStatus { NOT_RETURNED, RETURNED, RETURNED_WITH_DAMAGE };

class Order {
private:
    static int nextId;
    int id;
    Client* client;     
    Vehicle* vehicle;  
    int clientId;      // ID клиента для сериализации
    int vehicleId;     // ID транспортного средства для сериализации
    int days;
    double totalPrice;
    Status status;
    std::string rejectReason;
    ReturnStatus returnStatus;      // Статус возврата
public:
    Order() : clientId(-1), vehicleId(-1), id(nextId++), client(nullptr), vehicle(nullptr), 
             days(0), totalPrice(0.0), status(Status::PENDING), rejectReason(""), 
             returnStatus(ReturnStatus::NOT_RETURNED) {}
    Order(Client* client, Vehicle* vehicle, int days)
        : id(nextId++), client(client), vehicle(vehicle), days(days),
        totalPrice(0.0), status(Status::PENDING), rejectReason(""), returnStatus(ReturnStatus::NOT_RETURNED)
    {
        if (client) clientId = client->getId();
        if (vehicle) vehicleId = vehicle->getId();
        calculateTotal();
    }
    Order(const Order& other)
        : id(other.id), client(other.client), vehicle(other.vehicle),
          clientId(other.clientId), vehicleId(other.vehicleId),
          days(other.days), totalPrice(other.totalPrice), status(other.status),
          rejectReason(other.rejectReason), returnStatus(other.returnStatus) {}
    Order& operator=(const Order& other) {
        if (this != &other) {
            id = other.id;
            client = other.client;
            vehicle = other.vehicle;
            clientId = other.clientId;
            vehicleId = other.vehicleId;
            days = other.days;
            totalPrice = other.totalPrice;
            status = other.status;
            rejectReason = other.rejectReason;
            returnStatus = other.returnStatus;
        }
        return *this;
    }
    ~Order() = default;
    void calculateTotal();
    void approve();
    void reject(const std::string& reason);
    void returnVehicle(ReturnStatus returnStatus); // Обработка возврата
    static void setNextId(int id);
    static int getNextId();
    int getId() const;
    void setId(int newId);
    Client* getClient() const;
    Vehicle* getVehicle() const;
    Status getStatus() const;
    void setStatus(Status newStatus);
    double getTotalPrice() const;
    void setTotalPrice(double price);
    int getDays() const;
    void setDays(int newDays);
    std::string getRejectReason() const;
    void setRejectReason(const std::string& reason);
    ReturnStatus getReturnStatus() const;
    void setReturnStatus(ReturnStatus status);
    int getClientId() const;  // Возвращает ID клиента 
    int getVehicleId() const; // Возвращает ID транспортного средства
    void setClientId(int id); // Устанавливает ID клиента 
    void setVehicleId(int id); // Устанавливает ID транспортного средства 
    void setClient(Client* client); 
    void setVehicle(Vehicle* vehicle); 
    void printHeader() const;
    void printInfo() const;
    friend std::ostream& operator<<(std::ostream& os, const Order& order);
    friend std::istream& operator>>(std::istream& is, Order& order);
};
