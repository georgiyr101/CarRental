#include "AppConnector.h"
#include "Input.h"
#include "Exp_file.h"
#include "Exp_contr.h"
#include <algorithm>
#include <iostream>

AppConnector::AppConnector() 
    : fileHandler("data/"), currentClient(nullptr), currentAdmin(nullptr), isAdminMode(false) {
    
        adminLogic = new AdminLogic(menuDisplay, economyCars, premiumCars, trucks, electricCars, clients, orders, this);
        clientLogic = new ClientLogic(menuDisplay, economyCars, premiumCars, trucks, electricCars, orders, this, adminLogic);
        loadAllData();
}

AppConnector::~AppConnector() {
    saveAllData();
    delete clientLogic;
    delete adminLogic;
}

void AppConnector::loadAllData() {
    // Load data with exception handling
    try {
        fileHandler.loadContainer(economyCars, "EconomyCar.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Ошибка загрузки EconomyCar.txt: " << e.what() << std::endl;
    }
    
    try {
        fileHandler.loadContainer(premiumCars, "PremiumCar.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Ошибка загрузки PremiumCar.txt: " << e.what() << std::endl;
    }
    
    try {
        fileHandler.loadContainer(trucks, "Truck.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Ошибка загрузки Truck.txt: " << e.what() << std::endl;
    }
    
    try {
        fileHandler.loadContainer(electricCars, "ElectricCar.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Ошибка загрузки ElectricCar.txt: " << e.what() << std::endl;
    }
    
    try {
        fileHandler.loadContainer(clients, "Client.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Ошибка загрузки Client.txt: " << e.what() << std::endl;
    }
    
    // Find maximum ID among all vehicles to update nextId
    int maxId = 0;
    for (int i = 0; i < economyCars.size(); ++i) {
        if (economyCars.isValid(i) && economyCars[i].getId() > maxId) {
            maxId = economyCars[i].getId();
        }
    }
    for (int i = 0; i < premiumCars.size(); ++i) {
        if (premiumCars.isValid(i) && premiumCars[i].getId() > maxId) {
            maxId = premiumCars[i].getId();
        }
    }
    for (int i = 0; i < trucks.size(); ++i) {
        if (trucks.isValid(i) && trucks[i].getId() > maxId) {
            maxId = trucks[i].getId();
        }
    }
    for (int i = 0; i < electricCars.size(); ++i) {
        if (electricCars.isValid(i) && electricCars[i].getId() > maxId) {
            maxId = electricCars[i].getId();
        }
    }
    // Set nextId to maxId + 1 so new vehicles get unique IDs
    Vehicle::setNextId(maxId + 1);
    
    // Load orders with relationship restoration
    Container<Order> tempOrders;
    try {
        fileHandler.loadOrders(tempOrders, "Order.txt", clients);
    }
    catch (const Exp_file& e) {
        std::cerr << "Ошибка загрузки Order.txt: " << e.what() << std::endl;
    }
    
    // Find maximum ID among all orders to update nextId
    int maxOrderId = 0;
    
    // Restore relationships for orders
    for (int i = 0; i < tempOrders.size(); ++i) {
        if (tempOrders.isValid(i)) {
            Order order = tempOrders[i];
            int orderId = order.getId();
            if (orderId > maxOrderId) {
                maxOrderId = orderId;
            }
            
            int clientId = order.getClientId();
            int vehicleId = order.getVehicleId();
            
            // Find client
            Client* clientPtr = nullptr;
            for (int j = 0; j < clients.size(); ++j) {
                if (clients.isValid(j)) {
                    Client& client = clients[j];
                    if (client.getId() == clientId) {
                        clientPtr = &client;
                        break;
                    }
                }
            }
            
            // Find vehicle
            Vehicle* vehiclePtr = nullptr;
            for (int j = 0; j < economyCars.size(); ++j) {
                if (economyCars.isValid(j) && economyCars[j].getId() == vehicleId) {
                    vehiclePtr = &economyCars[j];
                    break;
                }
            }
            if (!vehiclePtr) {
                for (int j = 0; j < premiumCars.size(); ++j) {
                    if (premiumCars.isValid(j) && premiumCars[j].getId() == vehicleId) {
                        vehiclePtr = &premiumCars[j];
                        break;
                    }
                }
            }
            if (!vehiclePtr) {
                for (int j = 0; j < trucks.size(); ++j) {
                    if (trucks.isValid(j) && trucks[j].getId() == vehicleId) {
                        vehiclePtr = &trucks[j];
                        break;
                    }
                }
            }
            if (!vehiclePtr) {
                for (int j = 0; j < electricCars.size(); ++j) {
                    if (electricCars.isValid(j) && electricCars[j].getId() == vehicleId) {
                        vehiclePtr = &electricCars[j];
                        break;
                    }
                }
            }
            
            order.setClient(clientPtr);
            order.setVehicle(vehiclePtr);
            try {
                orders.add(order);
            }
            catch (const Exp_contr& e) {
                std::cerr << "Ошибка добавления заказа в контейнер: " << e.what() << std::endl;
            }
        }
    }
    // Set Order::nextId to maxOrderId + 1 so new orders get unique IDs
    Order::setNextId(maxOrderId + 1);
}

void AppConnector::saveAllData() {
    try {
        fileHandler.saveContainer(economyCars, "EconomyCar.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Ошибка сохранения EconomyCar.txt: " << e.what() << std::endl;
    }
    
    try {
        fileHandler.saveContainer(premiumCars, "PremiumCar.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Ошибка сохранения PremiumCar.txt: " << e.what() << std::endl;
    }
    
    try {
        fileHandler.saveContainer(trucks, "Truck.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Ошибка сохранения Truck.txt: " << e.what() << std::endl;
    }
    
    try {
        fileHandler.saveContainer(electricCars, "ElectricCar.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Ошибка сохранения ElectricCar.txt: " << e.what() << std::endl;
    }
    
    try {
        fileHandler.saveContainer(clients, "Client.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Ошибка сохранения Client.txt: " << e.what() << std::endl;
    }
    
    try {
        fileHandler.saveContainer(orders, "Order.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Ошибка сохранения Order.txt: " << e.what() << std::endl;
    }
}

bool AppConnector::authenticateUser(bool isAdmin) {
    if (isAdmin) {
        if (adminLogic->authenticateUser()) {
            currentAdmin = adminLogic->getCurrentAdmin();
            isAdminMode = true;
            return true;
        }
        return false;
    }
    else {
        std::cout << "Enter login: ";
        std::string login = inputString();
        std::cout << "Enter password: ";
        std::string password = inputString();
        
        // Find client
        for (int i = 0; i < clients.size(); ++i) {
            if (clients.isValid(i)) {
                Client& client = clients[i];
                if (client.getLogin() == login && client.getPassword() == password) {
                    currentClient = &client;
                    clientLogic->setCurrentClient(&client);
                    isAdminMode = false;
                    std::cout << "Welcome, " << client.getFirstName() << "!\n";
                    menuDisplay.pause();
                    return true;
                }
            }
        }
        std::cout << "Invalid login or password!\n";
        menuDisplay.pause();
        return false;
    }
}

void AppConnector::registerClient() {
    clientLogic->registerClient(clients);
    currentClient = clientLogic->getCurrentClient();
    isAdminMode = false;
    saveAllData();
    clientLogic->showClientMenu(clients);
}

void AppConnector::showMainMenu() {
    while (true) {
        menuDisplay.clearScreen();
        std::cout << "------ Welcome to RentCar ------\n\n\n";
        std::cout << "1. Login as Client\n";
        std::cout << "2. Register as Client\n";
        std::cout << "3. Login as Admin\n";
        std::cout << "4. Exit\n\n";
        std::cout.flush();
        
        int choice = menuDisplay.getChoice(1, 4);
        
        switch (choice) {
        case 1:
            if (authenticateUser(false)) {
                clientLogic->showClientMenu(clients);
                saveAllData();
            }
            break;
        case 2:
            registerClient();
            break;
        case 3:
            if (authenticateUser(true)) {
                adminLogic->showAdminMenu();
                saveAllData();
            }
            break;
        case 4:
            saveAllData();
            return;
        }
    }
}

