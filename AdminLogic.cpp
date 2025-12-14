#include "AdminLogic.h"
#include "ClientLogic.h" // Для VehicleSearchCriteria
#include "Car.h"
#include <iomanip>
#include <algorithm>
#include <type_traits>
#include <climits>
#include <cfloat>
#include "MenuDisplay.h"
#include "Input.h"
#include "Exp_contr.h"
#include "AppConnector.h"

AdminLogic::AdminLogic(MenuDisplay& menu, Container<EconomyCar>& ec, Container<PremiumCar>& pc, 
                       Container<Truck>& t, Container<ElectricCar>& elc, Container<Client>& cl, Container<Order>& ord, AppConnector* app)
    : menuDisplay(menu), economyCars(ec), premiumCars(pc), trucks(t), electricCars(elc), clients(cl), orders(ord), currentAdmin(nullptr), appConnector(app) {
}

void AdminLogic::setAppConnector(AppConnector* app) {
    appConnector = app;
}

void AdminLogic::setCurrentAdmin(Admin* admin) {
    currentAdmin = admin;
}

Admin* AdminLogic::getCurrentAdmin() const {
    return currentAdmin;
}

bool AdminLogic::authenticateUser() {
    std::cout << "Enter login: ";
    std::string login = inputString();
    std::cout << "Enter password: ";
    std::string password = inputString();
    
    if (login == "admin" && password == "admin") {
        currentAdmin = new Admin(login, password);
        std::cout << "Welcome, Admin!\n";
        menuDisplay.pause();
        return true;
    }
    else {
        std::cout << "Invalid login or password!\n";
        menuDisplay.pause();
        return false;
    }
}

void AdminLogic::showAdminMenu() {
    while (true) {
        menuDisplay.clearScreen();
        std::cout << "ADMIN MENU:\n\n";
        std::cout << "1. Manage cars\n";
        std::cout << "2. Manage clients\n";
        std::cout << "3. Manage orders\n";
        std::cout << "0. Logout\n\n";
        
        int choice = inputNumber<int>(std::cin, 0, 3);
        
        switch (choice) {
        case 1:
            adminManageVehicles();
            break;
        case 2:
            adminManageClients();
            break;
        case 3:
            adminManageOrders();
            break;
        case 0:
            currentAdmin = nullptr;
            return;
        }
    }
}

int AdminLogic::selectVehicleType(const std::string& title, bool allowAllOption) {
    menuDisplay.clearScreen();
    std::cout << title << "\n\n";
    std::cout << "1. Economy Car\n";
    std::cout << "2. Premium Car\n";
    std::cout << "3. Truck\n";
    std::cout << "4. Electric Car\n";
    if (allowAllOption) {
        std::cout << "5. All types\n";
        std::cout << "0. Back\n\n";
        return menuDisplay.getChoice(0, 5);
    } else {
        std::cout << "0. Back\n\n";
        return menuDisplay.getChoice(0, 4);
    }
}

std::string AdminLogic::getVehicleTypeName(int vehicleType) {
    switch (vehicleType) {
        case 1: return "Economy Car";
        case 2: return "Premium Car";
        case 3: return "Truck";
        case 4: return "Electric Car";
        default: return "";
    }
}

void AdminLogic::adminManageVehicles() {
    int vehicleType = selectVehicleType("MANAGE CARS - Select Vehicle Type");
    if (vehicleType == 0) return;
    
    std::string typeName = getVehicleTypeName(vehicleType);
    switch (vehicleType) {
        case 1: callVehicleTypeMenu(economyCars, typeName, false); break;
        case 2: callVehicleTypeMenu(premiumCars, typeName, false); break;
        case 3: callVehicleTypeMenu(trucks, typeName, false); break;
        case 4: callVehicleTypeMenu(electricCars, typeName, false); break;
    }
}

template<typename T>
Vehicle* AdminLogic::findInContainer(Container<T>& container, int id) {
    for (int i = 0; i < container.size(); ++i) {
        if (container.isValid(i) && container[i].getId() == id) {
            return &container[i];
        }
    }
    return nullptr;
}

Vehicle* AdminLogic::findVehicleById(int id) {
    Vehicle* result = findInContainer(economyCars, id);
    if (result) return result;
    
    result = findInContainer(premiumCars, id);
    if (result) return result;
    
    result = findInContainer(trucks, id);
    if (result) return result;
    
    result = findInContainer(electricCars, id);
    return result;
}

template<typename T>
int AdminLogic::findIndexById(Container<T>& container, int id) {
    for (int i = 0; i < container.size(); ++i) {
        if (container.isValid(i) && container[i].getId() == id) {
            return i;
        }
    }
    return -1;
}

template<typename T>
void AdminLogic::deleteItemById(Container<T>& container, const std::string& typeName, const std::string& itemName) {
    menuDisplay.clearScreen();
    menuDisplay.viewAllItems(container, typeName);
    std::cout << "\nEnter " + itemName + " ID to remove: ";
    int id = inputNumber<int>(std::cin, 0, 1000);
    
    int index = findIndexById(container, id);
    if (index != -1) {
        try {
            container.remove(index);
            std::cout << itemName << " removed!\n";
            if (appConnector) appConnector->saveAllData();
        }
        catch (const Exp_contr& e) {
            std::cout << "Ошибка при удалении: " << e.what() << "\n";
        }
    } else {
        std::cout << itemName << " not found!\n";
    }
    menuDisplay.pause();
}

void AdminLogic::adminManageClients() {
    while (true) {
        menuDisplay.clearScreen();
        std::cout << "MANAGE CLIENTS:\n\n";
        std::cout << "1. Show clients\n";
        std::cout << "2. Add client\n";
        std::cout << "3. Remove client\n";
        std::cout << "4. Update client\n";
        std::cout << "0. Back\n\n";
        
        int choice = inputNumber<int>(cin, 0, 4);
        
        switch (choice) {
        case 1:
            menuDisplay.viewAllItems(clients, "ALL CLIENTS");
            break;
        case 2:
            addClient();
            break;
        case 3:
            deleteItemById(clients, "ALL CLIENTS", "Client");
            break;
        case 4:
            editClient();
            break;
        case 0:
            return;
        }
    }
}

void AdminLogic::addClient() {
    menuDisplay.clearScreen();
    std::cout << "ADD CLIENT:\n\n";
    
    int id = clients.size() + 1;
    std::cout << "Login: ";
    std::string login = inputString();
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
    
    Client newClient(id, login, password, firstName, lastName, passport,
        phone, email, dateOfBirth, driverLicense);
    try {
        clients.add(newClient);
        std::cout << "\nClient added successfully!\n";
    }
    catch (const Exp_contr& e) {
        std::cout << "Ошибка при добавлении клиента: " << e.what() << "\n";
        menuDisplay.pause();
        return;
    }
    if (appConnector) appConnector->saveAllData();
    menuDisplay.pause();
}

void AdminLogic::editClient() {
    menuDisplay.clearScreen();
    menuDisplay.viewAllItems(clients, "ALL CLIENTS");
    std::cout << "\nEnter client ID to update: ";
    int id = inputNumber<int>(std::cin, 0, 1000);
    
    int index = findIndexById(clients, id);
    if (index == -1) {
        std::cout << "Client not found!\n";
        menuDisplay.pause();
        return;
    }
    
    editClientAt(index, nullptr);
}

void AdminLogic::editClientAt(int index, Client* currentClientPtr) {
    if (index < 0 || index >= clients.size() || !clients.isValid(index)) {
        std::cout << "Invalid client index!\n";
        menuDisplay.pause();
        return;
    }
    
    Client& client = clients[index];
    menuDisplay.clearScreen();
    std::cout << "EDIT " << (currentClientPtr ? "PROFILE" : "CLIENT") << ":\n\n";
    
    std::cout << "Current data:\n";
    client.printHeader();
    client.printInfo();
    
    std::cout << "\nWhat to update?\n";
    std::cout << "1. First name\n";
    std::cout << "2. Last name\n";
    std::cout << "3. Phone\n";
    std::cout << "4. Email\n";
    std::cout << "0. Cancel\n\n";
    std::cout << "Your choice: ";
    int field = inputNumber<int>(std::cin, 0, 4);
    if (field == 0) return;
    
    switch (field) {
        case 1: {
            std::cout << "New first name: ";
            std::string newValue = inputString();
            try {
                clients.update(index, [newValue](Client& c) { c.setFirstName(newValue); });
                if (currentClientPtr) currentClientPtr->setFirstName(newValue);
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
                if (currentClientPtr) currentClientPtr->setLastName(newValue);
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
                if (currentClientPtr) currentClientPtr->setPhoneNumber(newValue);
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
                if (currentClientPtr) currentClientPtr->setEmail(newValue);
            }
            catch (const Exp_contr& e) {
                std::cout << "Ошибка при обновлении: " << e.what() << "\n";
                menuDisplay.pause();
                return;
            }
            break;
        }
    }
    
    std::cout << "\n" << (currentClientPtr ? "Profile" : "Client") << " updated successfully!\n";
    if (appConnector) appConnector->saveAllData();
    menuDisplay.pause();
}

void AdminLogic::adminManageOrders() {
    while (true) {
        menuDisplay.clearScreen();
        std::cout << "MANAGE ORDERS:\n\n";
        std::cout << "1. Show all orders\n";
        std::cout << "2. Process orders\n";
        std::cout << "0. Back\n\n";
        std::cout << "Your choice: ";
        int choice = inputNumber<int>(std::cin, 0, 2);
        
        switch (choice) {
        case 1:
            menuDisplay.viewAllItems(orders, "ALL ORDERS", true);
            break;
        case 2:
            adminProcessOrders();
            break;
        case 0:
            return;
        }
    }
}

void AdminLogic::adminProcessOrders() {
    menuDisplay.clearScreen();
    std::cout << "PROCESS ORDERS:\n\n";
    
    // Show only PENDING orders
    std::vector<int> pendingOrderIndices;
    bool hasHeader = false;
    for (int i = 0; i < orders.size(); ++i) {
        if (orders.isValid(i) && orders[i].getStatus() == Status::PENDING) {
            pendingOrderIndices.push_back(i);
            Order& order = orders[i];
            if (!hasHeader) {
                order.printHeader();
                hasHeader = true;
            }
            order.printInfo();
        }
    }
    
    if (pendingOrderIndices.empty()) {
        std::cout << "No pending orders.\n";
        menuDisplay.pause();
        return;
    }
    
    std::cout << "Enter order ID to process: ";
    int orderId = inputNumber<int>(std::cin, 0, INT_MAX);
    Order* selectedOrder = nullptr;
    int selectedIndex = -1;
    
    for (int idx : pendingOrderIndices) {
        if (orders.isValid(idx) && orders[idx].getId() == orderId) {
            selectedOrder = &orders[idx];
            selectedIndex = idx;
            break;
        }
    }
    
    if (!selectedOrder) {
        std::cout << "Order not found!\n";
        menuDisplay.pause();
        return;
    }
    
    std::cout << "\n1. Approve\n";
    std::cout << "2. Reject\n";
    std::cout << "0. Cancel\n";
    std::cout << "Your choice: ";
    int action = inputNumber<int>(std::cin, 0, 2);
    
    if (action == 1) {
        approveOrder(*selectedOrder);
    }
    else if (action == 2) {
        rejectOrder(*selectedOrder);
    }
}

void AdminLogic::approveOrder(Order& order) {
    if (currentAdmin) {
        currentAdmin->approveOrder(order);
        std::cout << "Order approved!\n";
        if (appConnector) appConnector->saveAllData();
    }
    menuDisplay.pause();
}

void AdminLogic::rejectOrder(Order& order) {
    std::cout << "Enter rejection reason: ";
    std::string reason = inputString();
    if (currentAdmin) {
        currentAdmin->rejectOrder(order, reason);
        std::cout << "Order rejected!\n";
        if (appConnector) appConnector->saveAllData();
    }
    menuDisplay.pause();
}

// Шаблонные функции для работы с типами автомобилей
template<typename T>
void AdminLogic::showVehiclesByType(Container<T>& container, const std::string& typeName, bool onlyAvailable) {
    menuDisplay.clearScreen();
    std::cout << typeName << ":\n\n";
    
    if (container.size() == 0) {
        std::cout << "No " << typeName << " found.\n";
        menuDisplay.pause();
        return;
    }
    
    bool found = false;
    for (int i = 0; i < container.size(); ++i) {
        if (container.isValid(i) && (!onlyAvailable || container[i].getIsAvailable())) {
            if (!found) {
                container[i].printHeader();
                found = true;
            }
            container[i].printInfo();
            std::cout << "\n";
        }
    }
    
    if (!found) {
        std::cout << "No available " << typeName << " found.\n";
    }
    
    menuDisplay.pause();
}

// Ввод критериев поиска (копия из ClientLogic)
template<typename T>
VehicleSearchCriteria AdminLogic::inputSearchCriteria() {
    VehicleSearchCriteria criteria;
    auto& ui = this->menuDisplay;
    
    while (true) {
        ui.clearScreen();
        std::cout << "SELECT SEARCH CRITERIA (select multiple, 0 to finish):\n\n";
        std::cout << "1. Brand" << (criteria.useBrand ? " [SET]" : "") << "\n";
        std::cout << "2. Model" << (criteria.useModel ? " [SET]" : "") << "\n";
        std::cout << "3. Min Price" << (criteria.usePriceMin ? " [SET]" : "") << "\n";
        std::cout << "4. Max Price" << (criteria.usePriceMax ? " [SET]" : "") << "\n";
        std::cout << "5. Min Year" << (criteria.useYearMin ? " [SET]" : "") << "\n";
        std::cout << "6. Max Year" << (criteria.useYearMax ? " [SET]" : "") << "\n";
        std::cout << "7. Max Mileage" << (criteria.useMileageMax ? " [SET]" : "") << "\n";
        std::cout << "8. Color" << (criteria.useColor ? " [SET]" : "") << "\n";
        std::cout << "9. Availability" << (criteria.useAvailable ? " [SET]" : "") << "\n";
        
        int maxChoice = 9;
        if constexpr (std::is_base_of_v<Car, T>) {
            std::cout << "10. Fuel Type" << (criteria.useFuelType ? " [SET]" : "") << "\n";
            std::cout << "11. Transmission" << (criteria.useTransmission ? " [SET]" : "") << "\n";
            maxChoice = 11;
            if constexpr (std::is_same_v<T, PremiumCar>) {
                std::cout << "12. Has Driver" << (criteria.useHasDriver ? " [SET]" : "") << "\n";
                maxChoice = 12;
            }
        } else if constexpr (std::is_same_v<T, Truck>) {
            std::cout << "10. Min Max Load" << (criteria.useMaxLoadMin ? " [SET]" : "") << "\n";
            std::cout << "11. Min Cargo Volume" << (criteria.useCargoVolumeMin ? " [SET]" : "") << "\n";
            maxChoice = 11;
        } else if constexpr (std::is_same_v<T, ElectricCar>) {
            std::cout << "10. Min Battery Capacity" << (criteria.useBatteryCapacityMin ? " [SET]" : "") << "\n";
            std::cout << "11. Min Range" << (criteria.useRangeMin ? " [SET]" : "") << "\n";
            maxChoice = 11;
        }
        
        std::cout << "0. Start Search\n\n";
        
        int choice = ui.getChoice(0, maxChoice);
        if (choice == 0) break;
        
        switch (choice) {
            case 1:
                criteria.useBrand = true;
                std::cout << "Enter brand: ";
                criteria.brand = inputString();
                break;
            case 2:
                criteria.useModel = true;
                std::cout << "Enter model: ";
                criteria.model = inputString();
                break;
            case 3:
                criteria.usePriceMin = true;
                std::cout << "Enter minimum price: ";
                criteria.priceMin = inputNumber<double>(std::cin, 0.0, DBL_MAX);
                break;
            case 4:
                criteria.usePriceMax = true;
                std::cout << "Enter maximum price: ";
                criteria.priceMax = inputNumber<double>(std::cin, 0.0, DBL_MAX);
                break;
            case 5:
                criteria.useYearMin = true;
                std::cout << "Enter minimum year: ";
                criteria.yearMin = inputNumber<int>(std::cin, 1900, 2100);
                break;
            case 6:
                criteria.useYearMax = true;
                std::cout << "Enter maximum year: ";
                criteria.yearMax = inputNumber<int>(std::cin, 1900, 2100);
                break;
            case 7:
                criteria.useMileageMax = true;
                std::cout << "Enter maximum mileage: ";
                criteria.mileageMax = inputNumber<double>(std::cin, 0.0, DBL_MAX);
                break;
            case 8:
                criteria.useColor = true;
                std::cout << "Enter color: ";
                criteria.color = inputString();
                break;
            case 9:
                criteria.useAvailable = true;
                std::cout << "1. Available\n2. Not Available\n";
                criteria.available = (ui.getChoice(1, 2) == 1);
                break;
            case 10:
                if constexpr (std::is_base_of_v<Car, T>) {
                    criteria.useFuelType = true;
                    std::cout << "Enter fuel type: ";
                    criteria.fuelType = inputString();
                } else if constexpr (std::is_same_v<T, Truck>) {
                    criteria.useMaxLoadMin = true;
                    std::cout << "Enter minimum max load: ";
                    criteria.maxLoadMin = inputNumber<double>(std::cin, 0.0, DBL_MAX);
                } else if constexpr (std::is_same_v<T, ElectricCar>) {
                    criteria.useBatteryCapacityMin = true;
                    std::cout << "Enter minimum battery capacity: ";
                    criteria.batteryCapacityMin = inputNumber<double>(std::cin, 0.0, DBL_MAX);
                }
                break;
            case 11:
                if constexpr (std::is_base_of_v<Car, T>) {
                    criteria.useTransmission = true;
                    std::cout << "Enter transmission type: ";
                    criteria.transmission = inputString();
                } else if constexpr (std::is_same_v<T, Truck>) {
                    criteria.useCargoVolumeMin = true;
                    std::cout << "Enter minimum cargo volume: ";
                    criteria.cargoVolumeMin = inputNumber<double>(std::cin, 0.0, DBL_MAX);
                } else if constexpr (std::is_same_v<T, ElectricCar>) {
                    criteria.useRangeMin = true;
                    std::cout << "Enter minimum range: ";
                    criteria.rangeMin = inputNumber<double>(std::cin, 0.0, DBL_MAX);
                }
                break;
            case 12:
                if constexpr (std::is_same_v<T, PremiumCar>) {
                    criteria.useHasDriver = true;
                    std::cout << "1. Has Driver\n2. No Driver\n";
                    criteria.hasDriver = (inputNumber<int>(std::cin, 1, 2) == 1);
                }
                break;
        }
    }
    
    return criteria;
}

// Проверка соответствия критериям (копия из ClientLogic)
template<typename T>
bool AdminLogic::matchesCriteria(const T* vehicle, const VehicleSearchCriteria& criteria) {
    if (!vehicle) return false;
    
    if (criteria.useBrand && vehicle->getBrand() != criteria.brand) return false;
    if (criteria.useModel && vehicle->getModel() != criteria.model) return false;
    if (criteria.usePriceMin && vehicle->getPricePerDay() < criteria.priceMin) return false;
    if (criteria.usePriceMax && vehicle->getPricePerDay() > criteria.priceMax) return false;
    if (criteria.useYearMin && vehicle->getYear() < criteria.yearMin) return false;
    if (criteria.useYearMax && vehicle->getYear() > criteria.yearMax) return false;
    if (criteria.useMileageMax && vehicle->getMileage() > criteria.mileageMax) return false;
    if (criteria.useColor && vehicle->getColor() != criteria.color) return false;
    if (criteria.useAvailable && vehicle->getIsAvailable() != criteria.available) return false;
    
    if constexpr (std::is_base_of_v<Car, T>) {
        const Car* car = dynamic_cast<const Car*>(vehicle);
        if (car) {
            if (criteria.useFuelType && car->getFuelType() != criteria.fuelType) return false;
            if (criteria.useTransmission && car->getTransmissionType() != criteria.transmission) return false;
        }
        if constexpr (std::is_same_v<T, PremiumCar>) {
            const PremiumCar* premiumCar = dynamic_cast<const PremiumCar*>(vehicle);
            if (premiumCar && criteria.useHasDriver && premiumCar->getHasDriver() != criteria.hasDriver) return false;
        }
    }
    
    if constexpr (std::is_same_v<T, Truck>) {
        const Truck* truck = dynamic_cast<const Truck*>(vehicle);
        if (truck) {
            if (criteria.useMaxLoadMin && truck->getMaxLoad() < criteria.maxLoadMin) return false;
            if (criteria.useCargoVolumeMin && truck->getCargoVolume() < criteria.cargoVolumeMin) return false;
        }
    }
    
    if constexpr (std::is_same_v<T, ElectricCar>) {
        const ElectricCar* electricCar = dynamic_cast<const ElectricCar*>(vehicle);
        if (electricCar) {
            if (criteria.useBatteryCapacityMin && electricCar->getBatteryCapacity() < criteria.batteryCapacityMin) return false;
            if (criteria.useRangeMin && electricCar->getRangePerCharge() < criteria.rangeMin) return false;
        }
    }
    
    return true;
}

// Универсальная функция поиска
template<typename T>
void AdminLogic::searchVehiclesByType(Container<T>& container, const std::string& typeName, bool onlyAvailable) {
    if (container.size() == 0) {
        std::cout << "No " << typeName << " found.\n";
        menuDisplay.pause();
        return;
    }
    
    VehicleSearchCriteria criteria = inputSearchCriteria<T>();
    
    std::cout << "\nFound " << typeName << ":\n";
    bool found = false;
    bool hasHeader = false;
    
    for (int i = 0; i < container.size(); ++i) {
        if (container.isValid(i) && (!onlyAvailable || container[i].getIsAvailable())) {
            if (matchesCriteria(&container[i], criteria)) {
                if (!hasHeader) {
                    container[i].printHeader();
                    hasHeader = true;
                }
                container[i].printInfo();
                std::cout << "\n";
                found = true;
            }
        }
    }
    if (!found) {std::cout << "No " << typeName << " found.\n";}
    menuDisplay.pause();
}

// Универсальная функция сортировки (копия из ClientLogic)
template<typename T>
void AdminLogic::sortVehiclesByType(Container<T>& container, const std::string& typeName, bool onlyAvailable) {
    menuDisplay.clearScreen();
    
    if (container.size() == 0) {
        std::cout << "No " << typeName << " found.\n";
        menuDisplay.pause();
        return;
    }
    
    std::cout << "SORT " << typeName << " BY FIELD:\n\n";
    std::cout << "1. Brand (ascending)\n";
    std::cout << "2. Brand (descending)\n";
    std::cout << "3. Model (ascending)\n";
    std::cout << "4. Model (descending)\n";
    std::cout << "5. Price (ascending)\n";
    std::cout << "6. Price (descending)\n";
    std::cout << "7. Year (ascending)\n";
    std::cout << "8. Year (descending)\n";
    std::cout << "9. Mileage (ascending)\n";
    std::cout << "10. Mileage (descending)\n";
    
    int maxChoice = 10;
    if constexpr (std::is_same_v<T, Truck>) {
        std::cout << "11. Max Load (ascending)\n";
        std::cout << "12. Max Load (descending)\n";
        std::cout << "13. Cargo Volume (ascending)\n";
        std::cout << "14. Cargo Volume (descending)\n";
        maxChoice = 14;
    }
    if constexpr (std::is_same_v<T, ElectricCar>) {
        std::cout << "11. Battery Capacity (ascending)\n";
        std::cout << "12. Battery Capacity (descending)\n";
        std::cout << "13. Range (ascending)\n";
        std::cout << "14. Range (descending)\n";
        std::cout << "15. Charge Time (ascending)\n";
        std::cout << "16. Charge Time (descending)\n";
        maxChoice = 16;
    }
    
    std::cout << "0. Back\n\n";
    
    int choice = menuDisplay.getChoice(0, maxChoice);
    if (choice == 0) return;
    
    // Collect vehicles into vector
    std::vector<T*> vehicles;
    for (int i = 0; i < container.size(); ++i) {
        if (container.isValid(i) && (!onlyAvailable || container[i].getIsAvailable())) {
            vehicles.push_back(&container[i]);
        }
    }
    
    // Sort
    switch (choice) {
        case 1:
            std::sort(vehicles.begin(), vehicles.end(),
                [](T* a, T* b) { return a->getBrand() < b->getBrand(); });
            std::cout << "Vehicles sorted by brand (ascending).\n";
            break;
        case 2:
            std::sort(vehicles.begin(), vehicles.end(),
                [](T* a, T* b) { return a->getBrand() > b->getBrand(); });
            std::cout << "Vehicles sorted by brand (descending).\n";
            break;
        case 3:
            std::sort(vehicles.begin(), vehicles.end(),
                [](T* a, T* b) { return a->getModel() < b->getModel(); });
            std::cout << "Vehicles sorted by model (ascending).\n";
            break;
        case 4:
            std::sort(vehicles.begin(), vehicles.end(),
                [](T* a, T* b) { return a->getModel() > b->getModel(); });
            std::cout << "Vehicles sorted by model (descending).\n";
            break;
        case 5:
            std::sort(vehicles.begin(), vehicles.end(),
                [](T* a, T* b) { return a->getPricePerDay() < b->getPricePerDay(); });
            std::cout << "Vehicles sorted by price (ascending).\n";
            break;
        case 6:
            std::sort(vehicles.begin(), vehicles.end(),
                [](T* a, T* b) { return a->getPricePerDay() > b->getPricePerDay(); });
            std::cout << "Vehicles sorted by price (descending).\n";
            break;
        case 7:
            std::sort(vehicles.begin(), vehicles.end(),
                [](T* a, T* b) { return a->getYear() < b->getYear(); });
            std::cout << "Vehicles sorted by year (ascending).\n";
            break;
        case 8:
            std::sort(vehicles.begin(), vehicles.end(),
                [](T* a, T* b) { return a->getYear() > b->getYear(); });
            std::cout << "Vehicles sorted by year (descending).\n";
            break;
        case 9:
            std::sort(vehicles.begin(), vehicles.end(),
                [](T* a, T* b) { return a->getMileage() < b->getMileage(); });
            std::cout << "Vehicles sorted by mileage (ascending).\n";
            break;
        case 10:
            std::sort(vehicles.begin(), vehicles.end(),
                [](T* a, T* b) { return a->getMileage() > b->getMileage(); });
            std::cout << "Vehicles sorted by mileage (descending).\n";
            break;
        case 11:
            if constexpr (std::is_same_v<T, Truck>) {
                std::sort(vehicles.begin(), vehicles.end(),
                    [](T* a, T* b) { return a->getMaxLoad() < b->getMaxLoad(); });
                std::cout << "Vehicles sorted by max load (ascending).\n";
            } else if constexpr (std::is_same_v<T, ElectricCar>) {
                std::sort(vehicles.begin(), vehicles.end(),
                    [](T* a, T* b) { return a->getBatteryCapacity() < b->getBatteryCapacity(); });
                std::cout << "Vehicles sorted by battery capacity (ascending).\n";
            }
            break;
        case 12:
            if constexpr (std::is_same_v<T, Truck>) {
                std::sort(vehicles.begin(), vehicles.end(),
                    [](T* a, T* b) { return a->getMaxLoad() > b->getMaxLoad(); });
                std::cout << "Vehicles sorted by max load (descending).\n";
            } else if constexpr (std::is_same_v<T, ElectricCar>) {
                std::sort(vehicles.begin(), vehicles.end(),
                    [](T* a, T* b) { return a->getBatteryCapacity() > b->getBatteryCapacity(); });
                std::cout << "Vehicles sorted by battery capacity (descending).\n";
            }
            break;
        case 13:
            if constexpr (std::is_same_v<T, Truck>) {
                std::sort(vehicles.begin(), vehicles.end(),
                    [](T* a, T* b) { return a->getCargoVolume() < b->getCargoVolume(); });
                std::cout << "Vehicles sorted by cargo volume (ascending).\n";
            } else if constexpr (std::is_same_v<T, ElectricCar>) {
                std::sort(vehicles.begin(), vehicles.end(),
                    [](T* a, T* b) { return a->getRangePerCharge() < b->getRangePerCharge(); });
                std::cout << "Vehicles sorted by range (ascending).\n";
            }
            break;
        case 14:
            if constexpr (std::is_same_v<T, Truck>) {
                std::sort(vehicles.begin(), vehicles.end(),
                    [](T* a, T* b) { return a->getCargoVolume() > b->getCargoVolume(); });
                std::cout << "Vehicles sorted by cargo volume (descending).\n";
            } else if constexpr (std::is_same_v<T, ElectricCar>) {
                std::sort(vehicles.begin(), vehicles.end(),
                    [](T* a, T* b) { return a->getRangePerCharge() > b->getRangePerCharge(); });
                std::cout << "Vehicles sorted by range (descending).\n";
            }
            break;
        case 15:
            if constexpr (std::is_same_v<T, ElectricCar>) {
                std::sort(vehicles.begin(), vehicles.end(),
                    [](T* a, T* b) { return a->getChargeTime() < b->getChargeTime(); });
                std::cout << "Vehicles sorted by charge time (ascending).\n";
            }
            break;
        case 16:
            if constexpr (std::is_same_v<T, ElectricCar>) {
                std::sort(vehicles.begin(), vehicles.end(),
                    [](T* a, T* b) { return a->getChargeTime() > b->getChargeTime(); });
                std::cout << "Vehicles sorted by charge time (descending).\n";
            }
            break;
    }
    
    // Display sorted
    std::cout << "\nSORTED " << typeName << ":\n\n";
    if (!vehicles.empty()) {
        vehicles[0]->printHeader();
    }
    for (T* v : vehicles) {
        v->printInfo();
        std::cout << "\n";
    }
    
    menuDisplay.pause();
}
///////////////
template<typename T>
void AdminLogic::addVehicleByType(Container<T>& container) {
    menuDisplay.clearScreen();
    std::cout << "ADD VEHICLE:\n\n";
    
    std::cout << "Brand: ";
    std::string brand = inputString();
    std::cout << "Model: ";
    std::string model = inputString();
    std::cout << "License plate: ";
    std::string license = inputString();
    std::cout << "Price per day: ";
    double price = inputNumber<double>(std::cin, 0.0, DBL_MAX);
    std::cout << "Year: ";
    int year = inputNumber<int>(std::cin, 1900, 2100);
    std::cout << "Mileage: ";
    double mileage = inputNumber<double>(std::cin, 0.0, DBL_MAX);
    std::cout << "Color: ";
    std::string color = inputString();
    
    try {
        if constexpr (std::is_same_v<T, EconomyCar>) {
            std::cout << "Fuel type: ";
            std::string fuel = inputString();
            std::cout << "Transmission type: ";
            std::string transmission = inputString();
            EconomyCar car(brand, model, license, price, true, year, mileage, color, fuel, transmission);
            try {
                container.add(car);
            }
            catch (const Exp_contr& e) {
                std::cout << "Ошибка при добавлении автомобиля: " << e.what() << "\n";
                menuDisplay.pause();
                return;
            }
        } else if constexpr (std::is_same_v<T, PremiumCar>) {
            std::cout << "Fuel type: ";
            std::string fuel = inputString();
            std::cout << "Transmission type: ";
            std::string transmission = inputString();
            std::cout << "Has driver? (1 - Yes, 2 - No): ";
            int driverChoice = menuDisplay.getChoice(1, 2);
            bool hasDriver = (driverChoice == 1);
            PremiumCar car(brand, model, license, price, true, year, mileage, color, fuel, transmission, hasDriver);
            try {
                container.add(car);
            }
            catch (const Exp_contr& e) {
                std::cout << "Ошибка при добавлении автомобиля: " << e.what() << "\n";
                menuDisplay.pause();
                return;
            }
        } else if constexpr (std::is_same_v<T, Truck>) {
            std::cout << "Max load (tons): ";
            double maxLoad = inputNumber<double>(std::cin, 0.0, 100.0);
            std::cout << "Cargo volume (m³): ";
            double cargoVolume = inputNumber<double>(std::cin, 0.0, 1000.0);
            Truck truck(brand, model, license, price, true, year, mileage, color, maxLoad, cargoVolume);
            try {
                container.add(truck);
            }
            catch (const Exp_contr& e) {
                std::cout << "Ошибка при добавлении автомобиля: " << e.what() << "\n";
                menuDisplay.pause();
                return;
            }
        } else if constexpr (std::is_same_v<T, ElectricCar>) {
            std::cout << "Battery capacity (kWh): ";
            double battery = inputNumber<double>(std::cin, 0.0, 200.0);
            std::cout << "Range (km): ";
            double range = inputNumber<double>(std::cin, 0.0, 1000.0);
            std::cout << "Charge time (hours): ";
            double chargeTime = inputNumber<double>(std::cin, 0.0, 24.0);
            ElectricCar car(brand, model, license, price, true, year, mileage, color, battery, range, chargeTime);
            try {
                container.add(car);
            }
            catch (const Exp_contr& e) {
                std::cout << "Ошибка при добавлении автомобиля: " << e.what() << "\n";
                menuDisplay.pause();
                return;
            }
        }
        
        std::cout << "\nVehicle added successfully!\n";
        if (appConnector) appConnector->saveAllData();
    } catch (const std::exception& e) {
        std::cout << "Error adding vehicle: " << e.what() << "\n";
    }
    
    menuDisplay.pause();
}

template<typename T>
void AdminLogic::deleteVehicleByType(Container<T>& container, const std::string& typeName) {
    if (container.size() == 0) {
        menuDisplay.clearScreen();
        std::cout << "No " << typeName << " found.\n";
        menuDisplay.pause();
        return;
    }
    
    deleteItemById(container, typeName, "Vehicle");
}

template<typename T>
void AdminLogic::updateVehicleByType(Container<T>& container, const std::string& typeName) {
    menuDisplay.clearScreen();
    
    if (container.size() == 0) {
        std::cout << "No " << typeName << " found.\n";
        menuDisplay.pause();
        return;
    }
    
    std::cout << typeName << ":\n\n";
    bool hasHeader = false;
    for (int i = 0; i < container.size(); ++i) {
        if (container.isValid(i)) {
            if (!hasHeader) {
                container[i].printHeader();
                hasHeader = true;
            }
            container[i].printInfo();
            std::cout << "\n";
        }
    }
    
    std::cout << "\nEnter vehicle ID to update: ";
    int id = inputNumber<int>(std::cin, 0, INT_MAX);
    
    int index = findIndexById(container, id);
    if (index == -1) {
        std::cout << "Vehicle not found!\n";
        menuDisplay.pause();
        return;
    }
    
    T& vehicle = container[index];
    std::cout << "\nCurrent vehicle info:\n";
    vehicle.printHeader();
    vehicle.printInfo();
    std::cout << "\n\nWhat to update?\n";
    std::cout << "1. Brand\n";
    std::cout << "2. Model\n";
    std::cout << "3. License plate\n";
    std::cout << "4. Price per day\n";
    std::cout << "5. Year\n";
    std::cout << "6. Mileage\n";
    std::cout << "7. Color\n";
    std::cout << "8. Availability\n";
    
    int maxChoice = 8;
    if constexpr (std::is_same_v<T, PremiumCar>) {
        std::cout << "9. Has driver\n";
        maxChoice = 9;
    }
    std::cout << "0. Cancel\n\n";
    
    int field = menuDisplay.getChoice(0, maxChoice);
    if (field == 0) return;
    
    switch (field) {
        case 1: {
            std::cout << "New brand: ";
            std::string newBrand = inputString();
            try {
                container.update(index, [newBrand](T& v) { v.setBrand(newBrand); });
            }
            catch (const Exp_contr& e) {
                std::cout << "Ошибка при обновлении: " << e.what() << "\n";
                menuDisplay.pause();
                return;
            }
            break;
        }
        case 2: {
            std::cout << "New model: ";
            std::string newModel = inputString();
            try {
                container.update(index, [newModel](T& v) { v.setModel(newModel); });
            }
            catch (const Exp_contr& e) {
                std::cout << "Ошибка при обновлении: " << e.what() << "\n";
                menuDisplay.pause();
                return;
            }
            break;
        }
        case 3: {
            std::cout << "New license plate: ";
            std::string newLicense = inputString();
            try {
                container.update(index, [newLicense](T& v) { v.setLicensePlate(newLicense); });
            }
            catch (const Exp_contr& e) {
                std::cout << "Ошибка при обновлении: " << e.what() << "\n";
                menuDisplay.pause();
                return;
            }
            break;
        }
        case 4: {
            std::cout << "New price per day: ";
            double newPrice = inputNumber<double>(std::cin, 0.0, DBL_MAX);
            try {
                container.update(index, [newPrice](T& v) { v.setPricePerDay(newPrice); });
            }
            catch (const Exp_contr& e) {
                std::cout << "Ошибка при обновлении: " << e.what() << "\n";
                menuDisplay.pause();
                return;
            }
            break;
        }
        case 5: {
            std::cout << "New year: ";
            int newYear = inputNumber<int>(std::cin, 1900, 2100);
            try {
                container.update(index, [newYear](T& v) { v.setYear(newYear); });
            }
            catch (const Exp_contr& e) {
                std::cout << "Ошибка при обновлении: " << e.what() << "\n";
                menuDisplay.pause();
                return;
            }
            break;
        }
        case 6: {
            std::cout << "New mileage: ";
            double newMileage = inputNumber<double>(std::cin, 0.0, DBL_MAX);
            try {
                container.update(index, [newMileage](T& v) { v.setMileage(newMileage); });
            }
            catch (const Exp_contr& e) {
                std::cout << "Ошибка при обновлении: " << e.what() << "\n";
                menuDisplay.pause();
                return;
            }
            break;
        }
        case 7: {
            std::cout << "New color: ";
            std::string newColor = inputString();
            try {
                container.update(index, [newColor](T& v) { v.setColor(newColor); });
            }
            catch (const Exp_contr& e) {
                std::cout << "Ошибка при обновлении: " << e.what() << "\n";
                menuDisplay.pause();
                return;
            }
            break;
        }
        case 8: {
            std::cout << "1. Available\n";
            std::cout << "2. Not available\n";
            int availChoice = menuDisplay.getChoice(1, 2);
            bool newAvail = (availChoice == 1);
            try {
                container.update(index, [newAvail](T& v) { v.setIsAvailable(newAvail); });
            }
            catch (const Exp_contr& e) {
                std::cout << "Ошибка при обновлении: " << e.what() << "\n";
                menuDisplay.pause();
                return;
            }
            break;
        }
        case 9: {
            if constexpr (std::is_same_v<T, PremiumCar>) {
                std::cout << "1. Has driver\n";
                std::cout << "2. No driver\n";
                int driverChoice = menuDisplay.getChoice(1, 2);
                bool newHasDriver = (driverChoice == 1);
                try {
                    container.update(index, [newHasDriver](T& v) { v.setHasDriver(newHasDriver); });
                }
                catch (const Exp_contr& e) {
                    std::cout << "Ошибка при обновлении: " << e.what() << "\n";
                    menuDisplay.pause();
                    return;
                }
            }
            break;
        }
    }
    
    std::cout << "\nVehicle updated successfully!\n";
    if (appConnector) appConnector->saveAllData();
    menuDisplay.pause();
}

template<typename T>
void AdminLogic::undoByType(Container<T>& container, const std::string& typeName) {
    menuDisplay.clearScreen();
    std::cout << "UNDO " << typeName << ":\n\n";
    
    if (container.undo()) {
        std::cout << "Last action undone successfully!\n";
        if (appConnector) appConnector->saveAllData();
    } else {
        std::cout << "No actions to undo.\n";
    }
    
    menuDisplay.pause();
}

template<typename T>
void AdminLogic::adminVehicleTypeMenu(Container<T>& container, const std::string& typeName) {
    while (true) {
        menuDisplay.clearScreen();
        std::cout << "MANAGE " << typeName << ":\n\n";
        std::cout << "1. Show cars\n";
        std::cout << "2. Add car\n";
        std::cout << "3. Remove car\n";
        std::cout << "4. Update car\n";
        std::cout << "5. Sort\n";
        std::cout << "6. Search\n";
        std::cout << "7. Undo\n";
        std::cout << "0. Back\n\n";
        
        int choice = menuDisplay.getChoice(0, 7);
        if (choice == 0) return;
        
        executeAdminOperation(container, typeName, choice);
    }
}

template<typename T>
void AdminLogic::callVehicleTypeMenu(Container<T>& container, const std::string& typeName, bool isClient) {
    if (!isClient) {
        adminVehicleTypeMenu(container, typeName);
    }
}

template<typename T>
void AdminLogic::executeAdminOperation(Container<T>& container, const std::string& typeName, int operation) {
    switch (operation) {
        case 1: showVehiclesByType(container, typeName, false); break;
        case 2: addVehicleByType(container); break;
        case 3: deleteVehicleByType(container, typeName); break;
        case 4: updateVehicleByType(container, typeName); break;
        case 5: sortVehiclesByType(container, typeName, false); break;
        case 6: searchVehiclesByType(container, typeName, false); break;
        case 7: undoByType(container, typeName); break;
    }
}


