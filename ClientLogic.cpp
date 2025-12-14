#include "ClientLogic.h"
#include "AdminLogic.h"
#include "Car.h"
#include <iomanip>
#include <algorithm>
#include <type_traits>
#include <climits>
#include <cfloat>
#include "Input.h"
#include "AppConnector.h"

ClientLogic::ClientLogic(MenuDisplay& menu, Container<EconomyCar>& ec, Container<PremiumCar>& pc, 
                         Container<Truck>& t, Container<ElectricCar>& elc, Container<Order>& ord, AppConnector* app, AdminLogic* admin)
    : menuDisplay(menu), economyCars(ec), premiumCars(pc), trucks(t), electricCars(elc), orders(ord), currentClient(nullptr), appConnector(app), adminLogic(admin) {
}

void ClientLogic::setAppConnector(AppConnector* app) {
    appConnector = app;
}

void ClientLogic::setAdminLogic(AdminLogic* admin) {
    adminLogic = admin;
}

void ClientLogic::setCurrentClient(Client* client) {
    currentClient = client;
}

Client* ClientLogic::getCurrentClient() const {
    return currentClient;
}

bool ClientLogic::authenticateUser() {
    std::cout << "Enter login: ";
    std::string login = inputString();
    std::cout << "Enter password: ";
    std::string password = inputString();
    
    // Поиск клиента будет выполняться в AppConnector, так как нужен доступ к контейнеру clients
    return false; // Временная заглушка
}

void ClientLogic::registerClient(Container<Client>& clients) {
    menuDisplay.clearScreen();
    std::cout << "REGISTER NEW CLIENT:\n\n";
    
    // Check if login already exists
    std::cout << "Login: ";
    std::string login = inputString();
    for (int i = 0; i < clients.size(); ++i) {
        if (clients.isValid(i) && clients[i].getLogin() == login) {
            std::cout << "This login is already taken!\n";
            menuDisplay.pause();
            return;
        }
    }
    
    std::cout << "Password: ";
    std::string password = inputString();
    std::cout << "First name: ";
    std::string firstName = inputString();
    std::cout << "Last name: ";
    std::string lastName = inputString();
    std::cout << "Passport: ";
    std::string passport = inputString();
    std::cout << "Phone: ";
    std::string phone = inputString();
    std::cout << "Email: ";
    std::string email = inputString();
    std::cout << "Date of birth: ";
    std::string dateOfBirth = inputString();
    std::cout << "Driver license: ";
    std::string driverLicense = inputString();
    
    // Find maximum ID
    int maxId = 0;
    for (int i = 0; i < clients.size(); ++i) {
        if (clients.isValid(i) && clients[i].getId() > maxId) {
            maxId = clients[i].getId();
        }
    }
    int newId = maxId + 1;
    
    Client newClient(newId, login, password, firstName, lastName, passport,
        phone, email, dateOfBirth, driverLicense);
    try {
        clients.add(newClient);
        std::cout << "\nRegistration successful!\n";
    }
    catch (const Exp_contr& e) {
        std::cout << "Ошибка при регистрации: " << e.what() << "\n";
        menuDisplay.pause();
        return;
    }
    std::cout << "Welcome, " << firstName << "!\n";
    
    // Auto-login
    for (int i = 0; i < clients.size(); ++i) {
        if (clients.isValid(i) && clients[i].getId() == newId) {
            currentClient = &clients[i];
            break;
        }
    }
    
    menuDisplay.pause();
}

void ClientLogic::showClientMenu(Container<Client>& clients) {
    while (true) {
        menuDisplay.clearScreen();
        std::cout << "CLIENT MENU:\n\n";
        std::cout << "1. Browse cars\n";
        std::cout << "2. Make order\n";
        std::cout << "3. View my orders\n";
        std::cout << "4. Edit profile\n";
        std::cout << "0. Logout\n\n";
        
        int choice = menuDisplay.getChoice(0, 4);
        
        switch (choice) {
        case 1:
            clientBrowseVehicles();
            break;
        case 2:
            clientCreateOrder();
            break;
        case 3:
            clientViewOrders();
            break;
        case 4:
            clientEditProfile(clients);
            break;
        case 0:
            currentClient = nullptr;
            return;
        }
    }
}


void ClientLogic::clientBrowseVehicles() {
    if (!adminLogic) return;
    int vehicleType = adminLogic->selectVehicleType("BROWSE CARS", true);
    if (vehicleType == 0) return;
    
    if (vehicleType == 5) {
        // Показать все типы
        menuDisplay.clearScreen();
        std::cout << "ALL AVAILABLE CARS:\n\n";
        showVehicles(true);
        menuDisplay.pause();
        return;
    }
    
    std::string typeName = adminLogic->getVehicleTypeName(vehicleType);
    switch (vehicleType) {
        case 1: callVehicleTypeMenu(economyCars, typeName, true); break;
        case 2: callVehicleTypeMenu(premiumCars, typeName, true); break;
        case 3: callVehicleTypeMenu(trucks, typeName, true); break;
        case 4: callVehicleTypeMenu(electricCars, typeName, true); break;
    }
}


std::vector<Vehicle*> ClientLogic::getAllVehicles(bool onlyAvailable) {
    std::vector<Vehicle*> result;
    
    for (int i = 0; i < economyCars.size(); ++i) {
        if (economyCars.isValid(i) && (!onlyAvailable || economyCars[i].getIsAvailable())) {
            result.push_back(&economyCars[i]);
        }
    }
    for (int i = 0; i < premiumCars.size(); ++i) {
        if (premiumCars.isValid(i) && (!onlyAvailable || premiumCars[i].getIsAvailable())) {
            result.push_back(&premiumCars[i]);
        }
    }
    for (int i = 0; i < trucks.size(); ++i) {
        if (trucks.isValid(i) && (!onlyAvailable || trucks[i].getIsAvailable())) {
            result.push_back(&trucks[i]);
        }
    }
    for (int i = 0; i < electricCars.size(); ++i) {
        if (electricCars.isValid(i) && (!onlyAvailable || electricCars[i].getIsAvailable())) {
            result.push_back(&electricCars[i]);
        }
    }
    return result;
}

void ClientLogic::showVehicles(bool onlyAvailable) {
    std::cout << "\n--- Economy Class ---\n";
    for (int i = 0; i < economyCars.size(); ++i) {
        if (economyCars.isValid(i) && (!onlyAvailable || economyCars[i].getIsAvailable())) {
            economyCars[i].printInfo();
            std::cout << "\n";
        }
    }
    
    std::cout << "\n--- Premium Class ---\n";
    for (int i = 0; i < premiumCars.size(); ++i) {
        if (premiumCars.isValid(i) && (!onlyAvailable || premiumCars[i].getIsAvailable())) {
            premiumCars[i].printInfo();
            std::cout << "\n";
        }
    }
    
    std::cout << "\n--- Trucks ---\n";
    for (int i = 0; i < trucks.size(); ++i) {
        if (trucks.isValid(i) && (!onlyAvailable || trucks[i].getIsAvailable())) {
            trucks[i].printInfo();
            std::cout << "\n";
        }
    }
    
    std::cout << "\n--- Electric Cars ---\n";
    for (int i = 0; i < electricCars.size(); ++i) {
        if (electricCars.isValid(i) && (!onlyAvailable || electricCars[i].getIsAvailable())) {
            electricCars[i].printInfo();
            std::cout << "\n";
        }
    }
}

Vehicle* ClientLogic::selectVehicle() {
    if (!adminLogic) return nullptr;
    showVehicles(true);
    std::cout << "\nEnter car ID: ";
    int id = inputNumber<int>(std::cin, 0, INT_MAX);
    
    Vehicle* vehicle = adminLogic->findVehicleById(id);
    if (!vehicle) {
        std::cout << "Car not found!\n";
    }
    return vehicle;
}

void ClientLogic::clientCreateOrder() {
    if (!adminLogic) return;
    menuDisplay.clearScreen();
    std::cout << "MAKE ORDER:\n\n";
    
    showVehicles(true);
    
    std::cout << "\nEnter selected car ID: ";
    int vehicleId = inputNumber<int>(std::cin, 0, INT_MAX);
    
    Vehicle* selectedVehicle = adminLogic->findVehicleById(vehicleId);
    
    if (!selectedVehicle || !selectedVehicle->getIsAvailable()) {
        std::cout << "Car not found or not available!\n";
        menuDisplay.pause();
        return;
    }
    
    std::cout << "Enter number of rental days: ";
    int days = inputNumber<int>(std::cin, 1, 365);
    
    Order newOrder(currentClient, selectedVehicle, days);
    try {
        orders.add(newOrder);
        if (appConnector) appConnector->saveAllData();
        std::cout << "\nOrder created successfully!\n";
    }
    catch (const Exp_contr& e) {
        std::cout << "Ошибка при создании заказа: " << e.what() << "\n";
        menuDisplay.pause();
        return;
    }
    std::cout << "Order ID: " << newOrder.getId() << "\n";
    std::cout << "Total price: " << std::fixed << std::setprecision(2)
        << newOrder.getTotalPrice() << " rub.\n";
    
    menuDisplay.pause();
}

void ClientLogic::clientViewOrders() {
    menuDisplay.clearScreen();
    std::cout << "MY ORDERS:\n\n";
    
    bool found = false;
    bool hasHeader = false;
    for (int i = 0; i < orders.size(); ++i) {
        if (orders.isValid(i)) {
            Order& order = orders[i];
            if (order.getClient() == currentClient) {
                if (!hasHeader) {
                    order.printHeader();
                    hasHeader = true;
                }
                order.printInfo();
                found = true;
            }
        }
    }
    
    if (!found) {
        std::cout << "You have no orders.\n";
    }
    
    menuDisplay.pause();
}

void ClientLogic::clientEditProfile(Container<Client>& clients) {
    if (!currentClient) return;
    
    // Find client index
    int index = -1;
    for (int i = 0; i < clients.size(); ++i) {
        if (clients.isValid(i) && &clients[i] == currentClient) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        std::cout << "Error: client not found!\n";
        menuDisplay.pause();
        return;
    }
    
    menuDisplay.clearScreen();
    std::cout << "EDIT PROFILE:\n\n";
    
    std::cout << "Current data:\n";
    currentClient->printHeader();
    currentClient->printInfo();
    
    std::cout << "\nWhat to update?\n";
    std::cout << "1. First name\n";
    std::cout << "2. Last name\n";
    std::cout << "3. Phone\n";
    std::cout << "4. Email\n";
    std::cout << "0. Cancel\n\n";
    
    int field = menuDisplay.getChoice(0, 4);
    if (field == 0) return;
    
    switch (field) {
        case 1: {
            std::cout << "New first name: ";
            std::string newValue = inputString();
            try {
                clients.update(index, [newValue](Client& c) { c.setFirstName(newValue); });
                currentClient->setFirstName(newValue);
            }
            catch (const Exp_contr& e) {
                std::cout << "Ошибка при обновлении: " << e.what() << "\n";
                menuDisplay.pause();
                return;
            }
            break;
        }
        case 2: {
            std::cout << "New last name: ";
            std::string newValue = inputString();
            try {
                clients.update(index, [newValue](Client& c) { c.setLastName(newValue); });
                currentClient->setLastName(newValue);
            }
            catch (const Exp_contr& e) {
                std::cout << "Ошибка при обновлении: " << e.what() << "\n";
                menuDisplay.pause();
                return;
            }
            break;
        }
        case 3: {
            std::cout << "New phone: ";
            std::string newValue = inputString();
            try {
                clients.update(index, [newValue](Client& c) { c.setPhoneNumber(newValue); });
                currentClient->setPhoneNumber(newValue);
            }
            catch (const Exp_contr& e) {
                std::cout << "Ошибка при обновлении: " << e.what() << "\n";
                menuDisplay.pause();
                return;
            }
            break;
        }
        case 4: {
            std::cout << "New email: ";
            std::string newValue = inputString();
            try {
                clients.update(index, [newValue](Client& c) { c.setEmail(newValue); });
                currentClient->setEmail(newValue);
            }
            catch (const Exp_contr& e) {
                std::cout << "Ошибка при обновлении: " << e.what() << "\n";
                menuDisplay.pause();
                return;
            }
            break;
        }
    }
    
    std::cout << "\nProfile updated successfully!\n";
    if (appConnector) appConnector->saveAllData();
    menuDisplay.pause();
}


template<typename T>
void ClientLogic::clientVehicleTypeMenu(Container<T>& container, const std::string& typeName) {
    while (true) {
        menuDisplay.clearScreen();
        std::cout << "BROWSE " << typeName << ":\n\n";
        std::cout << "1. Show available\n";
        std::cout << "2. Search\n";
        std::cout << "3. Sort\n";
        std::cout << "0. Back\n\n";
        
        int choice = menuDisplay.getChoice(0, 3);
        if (choice == 0) return;
        
        executeClientOperation(container, typeName, choice);
    }
}

template<typename T>
void ClientLogic::callVehicleTypeMenu(Container<T>& container, const std::string& typeName, bool isClient) {
    if (isClient) {
        clientVehicleTypeMenu(container, typeName);
    }
}

template<typename T>
void ClientLogic::executeClientOperation(Container<T>& container, const std::string& typeName, int operation) {
    if (!adminLogic) return;
    switch (operation) {
        case 1: adminLogic->showVehiclesByType(container, typeName, true); break;
        case 2: adminLogic->searchVehiclesByType(container, typeName, true); break;
        case 3: adminLogic->sortVehiclesByType(container, typeName, true); break;
    }
}


