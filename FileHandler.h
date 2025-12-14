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
    // Конструктор с путем к директории для файлов
    FileHandler(const std::string& baseDirectory): baseDirectory(baseDirectory) {}
    
    // Деструктор
    ~FileHandler() = default;

    bool fileExists(const std::string& filename) const;

    // Базовый шаблонный метод
    template<typename T>
    void saveContainer(const Container<T>& container, const std::string& filename)
    {
        std::ofstream file(getFullPath(filename));
        if (!file.is_open()) {
            throw Exp_file(1, "Не удалось открыть файл для записи: " + filename);
        }

        for (int i = 0; i < container.size(); ++i) {
            if (container.isValid(i)) {
                if (!(file << container[i] << std::endl)) {
                    file.close();
                    throw Exp_file(2, "Ошибка записи в файл: " + filename);
                }
            }
        }

        if (file.fail()) {
            file.close();
            throw Exp_file(3, "Ошибка при закрытии файла после записи: " + filename);
        }
        file.close();
    }

    // Для всех остальных типов
    template<typename T>
    void loadContainer(Container<T>& container, const std::string& filename)
    {
        std::ifstream file(getFullPath(filename));
        if (!file.is_open()) {
            // Если файл не существует, это не критическая ошибка - просто создадим пустой контейнер
            // Выбрасываем исключение только если файл должен существовать
            // Для совместимости, если файл не найден, просто возвращаемся без ошибки
            return;
        }

        T item;
        while (file >> item) {
            try {
                container.add(item);
            }
            catch (...) {
                file.close();
                throw Exp_file(4, "Ошибка при добавлении элемента в контейнер из файла: " + filename);
            }
        }

        if (file.bad()) {
            file.close();
            throw Exp_file(5, "Ошибка чтения файла: " + filename);
        }
        file.close();
    }

    bool loadOrders(Container<Order>& orders, const std::string& filename, const Container<Client>& clients);



    // Метод для получения полного пути к файлу
    std::string getFullPath(const std::string& filename) const;

private:
    std::string baseDirectory;
};

#endif // FILEHANDLER_H

