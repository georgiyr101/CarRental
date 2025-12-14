#ifndef CLIENTLOGIC_H
#define CLIENTLOGIC_H

#include "MenuDisplay.h"
#include "Client.h"
#include "Vehicle.h"
#include "EconomyCar.h"
#include "PremiumCar.h"
#include "Truck.h"
#include "ElectricCar.h"
#include "Order.h"
#include "Container.h"
#include <vector>
#include <functional>
#include <set>
#include <algorithm>
#include <type_traits>

class AdminLogic; // Forward declaration

// Структура для критериев поиска
struct VehicleSearchCriteria {
    bool useBrand = false;
    std::string brand;
    
    bool useModel = false;
    std::string model;
    
    bool usePriceMin = false;
    double priceMin = 0.0;
    
    bool usePriceMax = false;
    double priceMax = 0.0;
    
    bool useYearMin = false;
    int yearMin = 0;
    
    bool useYearMax = false;
    int yearMax = 0;
    
    bool useMileageMax = false;
    double mileageMax = 0.0;
    
    bool useColor = false;
    std::string color;
    
    bool useAvailable = false;
    bool available = true;
    
    // Для Car и его производных
    bool useFuelType = false;
    std::string fuelType;
    
    bool useTransmission = false;
    std::string transmission;
    
    // Для PremiumCar
    bool useHasDriver = false;
    bool hasDriver = false;
    
    // Для Truck
    bool useMaxLoadMin = false;
    double maxLoadMin = 0.0;
    
    bool useCargoVolumeMin = false;
    double cargoVolumeMin = 0.0;
    
    // Для ElectricCar
    bool useBatteryCapacityMin = false;
    double batteryCapacityMin = 0.0;
    
    bool useRangeMin = false;
    double rangeMin = 0.0;
};

class AppConnector; // Forward declaration

class ClientLogic {
private:
    MenuDisplay& menuDisplay;
    Client* currentClient;
    Container<EconomyCar>& economyCars;
    Container<PremiumCar>& premiumCars;
    Container<Truck>& trucks;
    Container<ElectricCar>& electricCars;
    Container<Order>& orders;
    AppConnector* appConnector; // Для сохранения данных
    AdminLogic* adminLogic; // Для использования общих функций работы с автомобилями
    
    std::vector<Vehicle*> getAllVehicles(bool onlyAvailable = false);
    void showVehicles(bool onlyAvailable = false);
    template<typename T>
    void clientVehicleTypeMenu(Container<T>& container, const std::string& typeName);
    template<typename T>
    void callVehicleTypeMenu(Container<T>& container, const std::string& typeName, bool isClient);
    template<typename T>
    void executeClientOperation(Container<T>& container, const std::string& typeName, int operation);
    
public:
    ClientLogic(MenuDisplay& menu, Container<EconomyCar>& ec, Container<PremiumCar>& pc, 
                Container<Truck>& t, Container<ElectricCar>& elc, Container<Order>& ord, AppConnector* app = nullptr, AdminLogic* admin = nullptr);
    void setAppConnector(AppConnector* app);
    void setAdminLogic(AdminLogic* admin);
    
    bool authenticateUser();
    void registerClient(Container<Client>& clients);
    void showClientMenu(Container<Client>& clients);
    void clientBrowseVehicles();
    Vehicle* selectVehicle();
    void clientCreateOrder();
    void clientViewOrders();
    void clientEditProfile(Container<Client>& clients);
    void setCurrentClient(Client* client);
    Client* getCurrentClient() const;
};

#endif

