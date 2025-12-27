#include "AppConnector.h"
#include "Input.h"
#include "Exp_file.h"
#include "Exp_contr.h"
#include <algorithm>
#include <iostream>

// Конструктор класса AppConnector - инициализирует все компоненты системы
AppConnector::AppConnector() 
    : fileHandler("data/"), currentClient(nullptr), currentAdmin(nullptr), isAdminMode(false) {
    
        // Создание объектов логики администратора и клиента
        adminLogic = new AdminLogic(menuDisplay, economyCars, premiumCars, trucks, electricCars, clients, orders, this);
        clientLogic = new ClientLogic(menuDisplay, economyCars, premiumCars, trucks, electricCars, orders, this, adminLogic);
        // Загрузка всех данных из файлов при старте приложения
        loadAllData();
}

// Деструктор - сохраняет данные и освобождает память
AppConnector::~AppConnector() {
    saveAllData();
    delete clientLogic;
    delete adminLogic;
}

// Загрузка всех данных из файлов в контейнеры
void AppConnector::loadAllData() {
    // Загрузка данных с обработкой исключений
    // Загрузка экономичных автомобилей
    try {
        fileHandler.loadContainer(economyCars, "EconomyCar.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Error loading EconomyCar.txt: " << e.what() << std::endl;
    }
    
    // Загрузка премиум автомобилей
    try {
        fileHandler.loadContainer(premiumCars, "PremiumCar.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Error loading PremiumCar.txt: " << e.what() << std::endl;
    }
    
    // Загрузка грузовиков
    try {
        fileHandler.loadContainer(trucks, "Truck.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Error loading Truck.txt: " << e.what() << std::endl;
    }
    
    // Загрузка электромобилей
    try {
        fileHandler.loadContainer(electricCars, "ElectricCar.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Error loading ElectricCar.txt: " << e.what() << std::endl;
    }
    
    // Загрузка клиентов
    try {
        fileHandler.loadContainer(clients, "Client.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Error loading Client.txt: " << e.what() << std::endl;
    }
    
    // Поиск максимального ID среди всех транспортных средств для обновления nextId
    int maxId = 0;
    // Поиск максимального ID в экономичных автомобилях
    for (int i = 0; i < economyCars.size(); ++i) {
        if (economyCars.isValid(i) && economyCars[i].getId() > maxId) {
            maxId = economyCars[i].getId();
        }
    }
    // Поиск максимального ID в премиум автомобилях
    for (int i = 0; i < premiumCars.size(); ++i) {
        if (premiumCars.isValid(i) && premiumCars[i].getId() > maxId) {
            maxId = premiumCars[i].getId();
        }
    }
    // Поиск максимального ID в грузовиках
    for (int i = 0; i < trucks.size(); ++i) {
        if (trucks.isValid(i) && trucks[i].getId() > maxId) {
            maxId = trucks[i].getId();
        }
    }
    // Поиск максимального ID в электромобилях
    for (int i = 0; i < electricCars.size(); ++i) {
        if (electricCars.isValid(i) && electricCars[i].getId() > maxId) {
            maxId = electricCars[i].getId();
        }
    }
    // Установка nextId в maxId + 1, чтобы новые транспортные средства получали уникальные ID
    Vehicle::setNextId(maxId + 1);
    
    // Загрузка заказов с восстановлением связей
    Container<Order> tempOrders;
    try {
        fileHandler.loadOrders(tempOrders, "Order.txt", clients);
    }
    catch (const Exp_file& e) {
        std::cerr << "Error loading Order.txt: " << e.what() << std::endl;
    }
    
    // Поиск максимального ID среди всех заказов для обновления nextId
    int maxOrderId = 0;
    
    // Восстановление связей для заказов (связывание с клиентами и транспортными средствами)
    for (int i = 0; i < tempOrders.size(); ++i) {
        if (tempOrders.isValid(i)) {
            Order order = tempOrders[i];
            int orderId = order.getId();
            if (orderId > maxOrderId) {
                maxOrderId = orderId;
            }
            
            int clientId = order.getClientId();
            int vehicleId = order.getVehicleId();
            
            // Поиск клиента по ID
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
            
            // Поиск транспортного средства по ID во всех типах контейнеров
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
            
            // Установка связей заказа с клиентом и транспортным средством
            order.setClient(clientPtr);
            order.setVehicle(vehiclePtr);
            try {
                orders.add(order);
            }
            catch (const Exp_contr& e) {
                std::cerr << "Error adding order to container: " << e.what() << std::endl;
            }
        }
    }
    // Установка Order::nextId в maxOrderId + 1, чтобы новые заказы получали уникальные ID
    Order::setNextId(maxOrderId + 1);
}

// Сохранение всех данных в файлы
void AppConnector::saveAllData() {
    try {
        fileHandler.saveContainer(economyCars, "EconomyCar.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Error saving EconomyCar.txt: " << e.what() << std::endl;
    }
    try {
        fileHandler.saveContainer(premiumCars, "PremiumCar.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Error saving PremiumCar.txt: " << e.what() << std::endl;
    }
    try {
        fileHandler.saveContainer(trucks, "Truck.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Error saving Truck.txt: " << e.what() << std::endl;
    }
    try {
        fileHandler.saveContainer(electricCars, "ElectricCar.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Error saving ElectricCar.txt: " << e.what() << std::endl;
    }
    try {
        fileHandler.saveContainer(clients, "Client.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Error saving Client.txt: " << e.what() << std::endl;
    }
    try {
        fileHandler.saveContainer(orders, "Order.txt");
    }
    catch (const Exp_file& e) {
        std::cerr << "Error saving Order.txt: " << e.what() << std::endl;
    }
}

// Аутентификация пользователя (администратора или клиента)
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

// Регистрация нового клиента
void AppConnector::registerClient() {
    clientLogic->registerClient(clients);
    currentClient = clientLogic->getCurrentClient();
    isAdminMode = false;
    saveAllData();
    clientLogic->showClientMenu(clients);
}

// Отображение главного меню приложения
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
            // Вход как клиент
            if (authenticateUser(false)) {
                clientLogic->showClientMenu(clients);
                saveAllData();
            }
            break;
        case 2:
            // Регистрация нового клиента
            registerClient();
            break;
        case 3:
            // Вход как администратор
            if (authenticateUser(true)) {
                adminLogic->showAdminMenu();
                saveAllData();
            }
            break;
        case 4:
            // Выход из приложения с сохранением данных
            saveAllData();
            return;
        }
    }
}

