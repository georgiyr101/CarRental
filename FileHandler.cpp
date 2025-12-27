#include "FileHandler.h"
#include "Exp_file.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <windows.h>
#include <direct.h>
std::string FileHandler::getFullPath(const std::string& filename) const {return baseDirectory + filename;}
bool FileHandler::fileExists(const std::string& filename) const {
    std::ifstream file(getFullPath(filename));
    return file.good();
}
bool FileHandler::loadOrders(Container<Order>& orders, const std::string& filename, const Container<Client>& clients)
{
    std::ifstream file(getFullPath(filename));
    if (!file.is_open()) {return true;}
    Order order;
    while (file >> order) {
        try {
            int clientId = order.getClientId();
            int vehicleId = order.getVehicleId();
            Client* clientPtr = nullptr;
            Vehicle* vehiclePtr = nullptr;
            for (int i = 0; i < clients.size(); ++i) {
                if (clients.isValid(i)) {
                    const Client& client = clients[i];
                    if (client.getId() == clientId) {
                        clientPtr = const_cast<Client*>(&client);
                        break;
                    }
                }
            }
            order.setClient(clientPtr);
            orders.add(order);
        }
        catch (...) {
            file.close();
            throw Exp_file(6, "Error adding order to container from file: " + filename);
        }
    }
    if (file.bad()) {
        file.close();
        throw Exp_file(7, "Error reading orders file: " + filename);
    }
    file.close();
    return true;
}


