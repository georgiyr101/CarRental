#ifndef APPCONNECTOR_H
#define APPCONNECTOR_H

#include "MenuDisplay.h"
#include "ClientLogic.h"
#include "AdminLogic.h"
#include "FileHandler.h"
#include "Client.h"
#include "Admin.h"
#include "Vehicle.h"
#include "EconomyCar.h"
#include "PremiumCar.h"
#include "Truck.h"
#include "ElectricCar.h"
#include "Order.h"
#include "Container.h"

class AppConnector {
private:
    MenuDisplay menuDisplay;
    FileHandler fileHandler;
    
    // Контейнеры для данных
    Container<EconomyCar> economyCars;
    Container<PremiumCar> premiumCars;
    Container<Truck> trucks;
    Container<ElectricCar> electricCars;
    Container<Client> clients;
    Container<Order> orders;
    
    // Логика клиента и админа 
    ClientLogic* clientLogic;
    AdminLogic* adminLogic;
    
    Client* currentClient; // Текущий пользователь
    Admin* currentAdmin; // Текущий администратор 
    bool isAdminMode;
    
    void loadAllData();
    bool authenticateUser(bool isAdmin);
    void registerClient();
    
public:
    AppConnector();
    ~AppConnector();
    
    void showMainMenu();
    void saveAllData();
};

#endif

