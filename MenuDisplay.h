#ifndef MENUDISPLAY_H
#define MENUDISPLAY_H

#include <iostream>
#include <string>
#include <limits>
#include "Container.h"

class MenuDisplay {
public:
    void clearScreen();
    void pause();
    int getChoice(int min, int max);
    std::string getStringInput(const std::string& prompt);
    double getDoubleInput(const std::string& prompt);
    int getIntInput(const std::string& prompt);
    
    // Шаблонная функция для вывода всех элементов контейнера
    template<typename T>
    void viewAllItems(Container<T>& container, const std::string& title, bool showScreen = true);
    
    // Шаблонная функция для поиска индекса по ID
    template<typename T>
    int findIndexById(Container<T>& container, int id);
};

#endif

