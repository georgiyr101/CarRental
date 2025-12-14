#include "FileHandler.h"
#include "Exp_file.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <cerrno>
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
    if (!file.is_open()) {
        // Если файл не существует, это не критическая ошибка - просто создадим пустой контейнер
        return true;
    }

    Order order;
    while (file >> order) {
        try {
            // Восстанавливаем указатели на клиента и транспортное средство по ID
            int clientId = order.getClientId();
            int vehicleId = order.getVehicleId();

            Client* clientPtr = nullptr;
            Vehicle* vehiclePtr = nullptr;

            // Поиск клиента по ID
            for (int i = 0; i < clients.size(); ++i) {
                if (clients.isValid(i)) {
                    const Client& client = clients[i];
                    if (client.getId() == clientId) {
                        // Используем const_cast для получения неконстантного указателя
                        clientPtr = const_cast<Client*>(&client);
                        break;
                    }
                }
            }

            // Транспортное средство будет найдено в AppConnector после загрузки всех контейнеров
            // Здесь только сохраняем ID
            order.setClient(clientPtr);
            // vehiclePtr будет установлен позже в AppConnector

            orders.add(order);
        }
        catch (...) {
            file.close();
            throw Exp_file(6, "Ошибка при добавлении заказа в контейнер из файла: " + filename);
        }
    }

    if (file.bad()) {
        file.close();
        throw Exp_file(7, "Ошибка чтения файла заказов: " + filename);
    }
    file.close();
    return true;
}

//// Сохранение EconomyCar
//bool FileHandler::saveEconomyCars(const Container<EconomyCar>& cars, const std::string& filename) 
//{
//        std::ofstream file(getFullPath(filename));
//        if (!file.is_open()) {
//            return false;
//        }
//
//        for (int i = 0; i < cars.size(); ++i) {
//            if (cars.isValid(i)) {
//                file << cars[i] << std::endl;
//            }
//        }
//        file.close();
//        return true;
//}
//
//// Загрузка EconomyCar
//bool FileHandler::loadEconomyCars(Container<EconomyCar>& cars, const std::string& filename) {
//        std::ifstream file(getFullPath(filename));
//        if (!file.is_open()) {
//            return false;
//        }
//
//        EconomyCar car;
//        while (file >> car) {
//            cars.add(car);
//        }
//
//        file.close();
//        return true;
//}
//
//// Сохранение PremiumCar
//bool FileHandler::savePremiumCars(const Container<PremiumCar>& cars, const std::string& filename) {
//    
//        std::ofstream file(getFullPath(filename));
//        if (!file.is_open()) {
//            return false;
//        }
//
//        for (int i = 0; i < cars.size(); ++i) {
//            if (cars.isValid(i)) {
//                file << cars[i] << std::endl;
//            }
//        }
//
//        file.close();
//        return true;
//}
//
//// Загрузка PremiumCar
//bool FileHandler::loadPremiumCars(Container<PremiumCar>& cars, const std::string& filename) 
//{
//        std::ifstream file(getFullPath(filename));
//        if (!file.is_open()) {
//            return false;
//        }
//
//        PremiumCar car;
//        while (file >> car) {
//            cars.add(car);
//        }
//
//        file.close();
//        return true;
//}
//
//// Сохранение Truck
//bool FileHandler::saveTrucks(const Container<Truck>& trucks, const std::string& filename) 
//{
//        std::ofstream file(getFullPath(filename));
//        if (!file.is_open()) {
//            return false;
//        }
//
//        for (int i = 0; i < trucks.size(); ++i) {
//            if (trucks.isValid(i)) {
//                file << trucks[i] << std::endl;
//            }
//        }
//
//        file.close();
//        return true;
//}
//
//// Загрузка Truck
//bool FileHandler::loadTrucks(Container<Truck>& trucks, const std::string& filename) 
//{
//        std::ifstream file(getFullPath(filename));
//        if (!file.is_open()) {
//            return false;
//        }
//
//        Truck truck;
//        while (file >> truck) {
//            trucks.add(truck);
//        }
//
//        file.close();
//        return true;
//}
//
//// Сохранение ElectricCar
//bool FileHandler::saveElectricCars(const Container<ElectricCar>& cars, const std::string& filename) 
//{
//        std::ofstream file(getFullPath(filename));
//        if (!file.is_open()) {
//            return false;
//        }
//
//        for (int i = 0; i < cars.size(); ++i) {
//            if (cars.isValid(i)) {
//                file << cars[i] << std::endl;
//            }
//        }
//
//        file.close();
//        return true;
//}
//
//// Загрузка ElectricCar
//bool FileHandler::loadElectricCars(Container<ElectricCar>& cars, const std::string& filename) 
//{
//        std::ifstream file(getFullPath(filename));
//        if (!file.is_open()) {
//            return false;
//        }
//
//        ElectricCar car;
//        while (file >> car) {
//            cars.add(car);
//        }
//
//        file.close();
//        return true;
//}
//
//// Сохранение Order
//bool FileHandler::saveOrders(const Container<Order>& orders, const std::string& filename) 
//{
//        std::ofstream file(getFullPath(filename));
//        if (!file.is_open()) {
//            return false;
//        }
//
//        for (int i = 0; i < orders.size(); ++i) {
//            if (orders.isValid(i)) {
//                file << orders[i] << std::endl;
//            }
//        }
//
//        file.close();
//        return true;
//}
//
//// Загрузка Order
//
//// Сохранение Client
//bool FileHandler::saveClients(const Container<Client>& clients, const std::string& filename) 
//{
//        std::ofstream file(getFullPath(filename));
//        if (!file.is_open()) {
//            return false;
//        }
//
//        for (int i = 0; i < clients.size(); ++i) {
//            if (clients.isValid(i)) {
//                file << clients[i] << std::endl;
//            }
//        }
//
//        file.close();
//        return true;
//}
//
//// Загрузка Client
//bool FileHandler::loadClients(Container<Client>& clients, const std::string& filename) 
//{
//        std::ifstream file(getFullPath(filename));
//        if (!file.is_open()) {
//            return false;
//        }
//
//        Client client;
//        while (file >> client) {
//            clients.add(client);
//        }
//
//        file.close();
//        return true;
//}
//
