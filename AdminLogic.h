#ifndef ADMINLOGIC_H
#define ADMINLOGIC_H

#include "MenuDisplay.h"
#include "Admin.h"
#include "Client.h"
#include "Vehicle.h"
#include "EconomyCar.h"
#include "PremiumCar.h"
#include "Truck.h"
#include "ElectricCar.h"
#include "Order.h"
#include "Container.h"
#include "ClientLogic.h" // для VehicleSearchCriteria
#include <vector>
#include <functional>
#include <set>
#include <algorithm>
#include <type_traits>

class AppConnector; // Forward declaration

class AdminLogic {
private:
    MenuDisplay& menuDisplay;
    Admin* currentAdmin;
    Container<EconomyCar>& economyCars;
    Container<PremiumCar>& premiumCars;
    Container<Truck>& trucks;
    Container<ElectricCar>& electricCars;
    Container<Client>& clients;
    Container<Order>& orders;
    AppConnector* appConnector; // Для сохранения данных
    
    // Шаблонные функции для работы с типами автомобилей
    template<typename T>
    void addVehicleByType(Container<T>& container);
    template<typename T>
    void deleteVehicleByType(Container<T>& container, const std::string& typeName);
    template<typename T>
    void updateVehicleByType(Container<T>& container, const std::string& typeName);
    template<typename T>
    void undoByType(Container<T>& container, const std::string& typeName);
    template<typename T>
    void adminVehicleTypeMenu(Container<T>& container, const std::string& typeName);
    template<typename T>
    void callVehicleTypeMenu(Container<T>& container, const std::string& typeName, bool isClient);
    template<typename T>
    void executeAdminOperation(Container<T>& container, const std::string& typeName, int operation);
    
    // Шаблонные функции для работы с контейнерами
    template<typename T>
    void deleteItemById(Container<T>& container, const std::string& typeName, const std::string& itemName);
    template<typename T>
    int findIndexById(Container<T>& container, int id);
    
    void addClient();
    void editClient();
    void editClientAt(int index, Client* currentClientPtr);
    
public:
    AdminLogic(MenuDisplay& menu, Container<EconomyCar>& ec, Container<PremiumCar>& pc, 
               Container<Truck>& t, Container<ElectricCar>& elc, Container<Client>& cl, Container<Order>& ord, AppConnector* app = nullptr);
    void setAppConnector(AppConnector* app);
    
    // Публичные функции для работы с автомобилями (используются также в ClientLogic)
    int selectVehicleType(const std::string& title, bool allowAllOption = false);
    std::string getVehicleTypeName(int vehicleType);
    Vehicle* findVehicleById(int id);
    template<typename T>
    Vehicle* findInContainer(Container<T>& container, int id);
    template<typename T>
    void showVehiclesByType(Container<T>& container, const std::string& typeName, bool onlyAvailable);
    template<typename T>
    void searchVehiclesByType(Container<T>& container, const std::string& typeName, bool onlyAvailable);
    template<typename T>
    void sortVehiclesByType(Container<T>& container, const std::string& typeName, bool onlyAvailable);
    template<typename T>
    VehicleSearchCriteria inputSearchCriteria();
    template<typename T>
    bool matchesCriteria(const T* vehicle, const VehicleSearchCriteria& criteria);
    
    bool authenticateUser();
    void showAdminMenu();
    void adminManageVehicles();
    void adminManageClients();
    void adminManageOrders();
    void adminProcessOrders();
    void approveOrder(Order& order);
    void rejectOrder(Order& order);
    void setCurrentAdmin(Admin* admin);
    Admin* getCurrentAdmin() const;
};

#endif

