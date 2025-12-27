#include "MenuDisplay.h"
#include "Client.h"
#include "Order.h"
#include "EconomyCar.h"
#include "PremiumCar.h"
#include "Truck.h"
#include "ElectricCar.h"
#include <limits>
#include <climits>
#include <cfloat>
#include "Input.h"
#include <windows.h>
// Очистка экрана
void MenuDisplay::clearScreen() { 
    system("cls"); 
}
// Пауза с ожиданием нажатия Enter
void MenuDisplay::pause() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

// Получение выбора пользователя в заданном диапазоне
int MenuDisplay::getChoice(int min, int max) {
    std::cout << "Choose option: ";
    return inputNumber<int>(std::cin, min, max);
}

// Получение строкового ввода
std::string MenuDisplay::getStringInput(const std::string& prompt) {
    std::cout << prompt;
    return inputString();
}

// Получение вещественного числа
double MenuDisplay::getDoubleInput(const std::string& prompt) {
    std::cout << prompt;
    return inputNumber<double>(std::cin, -DBL_MAX, DBL_MAX);
}

// Получение целого числа
int MenuDisplay::getIntInput(const std::string& prompt) {
    std::cout << prompt;
    return inputNumber<int>(std::cin, INT_MIN, INT_MAX);
}

// Шаблонная функция для вывода всех элементов контейнера
template<typename T>
void MenuDisplay::viewAllItems(Container<T>& container, const std::string& title, bool showScreen) {
    if (showScreen) {
        clearScreen();
        std::cout << title << ":\n\n";
    }
    // Вывод заголовка таблицы один раз
    bool hasHeader = false;
    for (int i = 0; i < container.size(); ++i) {
        if (container.isValid(i)) {
            T& item = container[i];
            if (!hasHeader) {
                item.printHeader();
                hasHeader = true;
            }
            item.printInfo();
        }
    }
    
    if (showScreen) {
        pause();
    }
}
// Шаблонная функция для поиска индекса элемента по ID
template<typename T>
int MenuDisplay::findIndexById(Container<T>& container, int id) {
    for (int i = 0; i < container.size(); ++i) {
        if (container.isValid(i) && container[i].getId() == id) {
            return i;
        }
    }
    return -1;
}

// Явные инстанцирования шаблонов для используемых типов
// Это необходимо для компиляции шаблонных функций с конкретными типами
template void MenuDisplay::viewAllItems<Client>(Container<Client>&, const std::string&, bool);
template void MenuDisplay::viewAllItems<Order>(Container<Order>&, const std::string&, bool);
template void MenuDisplay::viewAllItems<EconomyCar>(Container<EconomyCar>&, const std::string&, bool);
template void MenuDisplay::viewAllItems<PremiumCar>(Container<PremiumCar>&, const std::string&, bool);
template void MenuDisplay::viewAllItems<Truck>(Container<Truck>&, const std::string&, bool);
template void MenuDisplay::viewAllItems<ElectricCar>(Container<ElectricCar>&, const std::string&, bool);

template int MenuDisplay::findIndexById<Client>(Container<Client>&, int);
template int MenuDisplay::findIndexById<Order>(Container<Order>&, int);
template int MenuDisplay::findIndexById<EconomyCar>(Container<EconomyCar>&, int);
template int MenuDisplay::findIndexById<PremiumCar>(Container<PremiumCar>&, int);
template int MenuDisplay::findIndexById<Truck>(Container<Truck>&, int);
template int MenuDisplay::findIndexById<ElectricCar>(Container<ElectricCar>&, int);

