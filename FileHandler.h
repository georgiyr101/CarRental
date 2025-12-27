#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include "EconomyCar.h"
#include "PremiumCar.h"
#include "Truck.h"
#include "ElectricCar.h"
#include "Order.h"
#include "Client.h"
#include "Container.h"
#include "Exp_file.h"
// Класс для работы с файлами
class FileHandler {
public:
    FileHandler(const std::string& baseDirectory): baseDirectory(baseDirectory) {}
    ~FileHandler() = default;
    bool fileExists(const std::string& filename) const;
    template<typename T>
    void saveContainer(const Container<T>& container, const std::string& filename)
    {
        std::ofstream file(getFullPath(filename));
        if (!file.is_open()) {
            throw Exp_file(1, "Failed to open file for writing: " + filename);
        }

        for (int i = 0; i < container.size(); ++i) {
            if (container.isValid(i)) {
                if (!(file << container[i] << std::endl)) {
                    file.close();
                    throw Exp_file(2, "Error writing to file: " + filename);
                }
            }
        }
        if (file.fail()) {
            file.close();
            throw Exp_file(3, "Error closing file after writing: " + filename);
        }
        file.close();
    }
    template<typename T>
    void loadContainer(Container<T>& container, const std::string& filename)
    {
        std::ifstream file(getFullPath(filename));
        if (!file.is_open()) {return;}
        T item;
        while (file >> item) {
            try {
                container.add(item);
            }
            catch (...) {
                file.close();
                throw Exp_file(4, "Error adding element to container from file: " + filename);
            }
        }
        if (file.bad()) {
            file.close();
            throw Exp_file(5, "Error reading file: " + filename);
        }
        file.close();
    }
    bool loadOrders(Container<Order>& orders, const std::string& filename, const Container<Client>& clients);
    std::string getFullPath(const std::string& filename) const;
private:
    std::string baseDirectory;
};

#endif // FILEHANDLER_H

