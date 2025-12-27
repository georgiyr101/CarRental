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
    template<typename T>
    void viewAllItems(Container<T>& container, const std::string& title, bool showScreen = true);
    template<typename T>
    int findIndexById(Container<T>& container, int id);
};

#endif

