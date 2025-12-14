#ifndef MANAGER_H
#define MANAGER_H

#include "Stack.h"
#include "Command.h"
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <functional>
#include <iterator>

// Шаблонный класс для управления объектами
template<typename T>
class Manager {
protected:
    std::vector<T*> items;                   // STL контейнер для хранения всех объектов
    std::map<int, T*> itemMap;              // STL контейнер для быстрого поиска по ID
    int nextId;                              // Следующий доступный ID
    
    // История команд для Undo (используем собственный Stack)
    Stack<Command*> commandHistory;

public:
    Manager() : nextId(1) {}
    
    virtual ~Manager() {
        clear();
    }
    
    // Добавление объекта
    void addItem(T* item) {
        if (item == nullptr) {
            return;
        }
        
        items.push_back(item);
        
        // Добавляем в map по ID (предполагаем, что все объекты имеют getId())
        // Для Order ID устанавливается при создании, для Vehicle можно установить через setId
        int id = getIdFromItem(item);
        if (id > 0) {
            itemMap[id] = item;
        }
    }
    
protected:
    // Вспомогательный метод для получения ID (можно переопределить в наследниках)
    virtual int getIdFromItem(T* item) const {
        // Пытаемся вызвать getId(), если метод существует
        // Это будет работать для Vehicle и Order
        // Для классов без getId() (например, Client) нужно переопределить в наследнике
        if (item) {
            return item->getId();
        }
        return 0;
    }
    
    // Удаление объекта по ID
    bool removeItem(int id) {
        auto mapIt = itemMap.find(id);
        if (mapIt == itemMap.end()) {
            return false;
        }
        
        T* item = mapIt->second;
        
        // Удаляем из vector
        auto vecIt = std::find(items.begin(), items.end(), item);
        if (vecIt != items.end()) {
            items.erase(vecIt);
        }
        
        // Удаляем из map
        itemMap.erase(mapIt);
        
        // Удаляем объект
        delete item;
        
        return true;
    }
    
    // Поиск объекта по ID
    T* findItemById(int id) const {
        auto it = itemMap.find(id);
        if (it != itemMap.end()) {
            return it->second;
        }
        return nullptr;
    }
    
    // Получить все объекты
    const std::vector<T*>& getAllItems() const {
        return items;
    }
    
    // Получить доступ к items для наследников (неконстантная версия)
    std::vector<T*>& getItems() {
        return items;
    }
    
    // Получить количество объектов
    size_t getCount() const {
        return items.size();
    }
    
    // Получить следующий доступный ID
    int getNextId() {
        return nextId++;
    }
    
    // Отмена последнего действия
    void undo() {
        if (!commandHistory.empty()) {
            Command* cmd = commandHistory.top();
            cmd->undo();
            commandHistory.pop();
            delete cmd;
        }
    }
    
    // Очистка всех данных
    void clear() {
        // Очищаем историю команд
        while (!commandHistory.empty()) {
            Command* cmd = commandHistory.top();
            commandHistory.pop();
            delete cmd;
        }
        
        // Удаляем все объекты
        for (T* item : items) {
            delete item;
        }
        
        items.clear();
        itemMap.clear();
        nextId = 1;
    }
    
    // Проверка на пустоту
    bool empty() const {
        return items.empty();
    }
    
    // Получить историю команд (для расширенного использования)
    Stack<Command*>& getCommandHistory() {
        return commandHistory;
    }
    
    // Добавить команду в историю
    void pushCommand(Command* cmd) {
        if (cmd != nullptr) {
            commandHistory.push(cmd);
        }
    }
    
    // Универсальная функция поиска по предикату (любому условию)
    // Пример: findItems([](T* item) { return item->getPrice() > 1000; })
    template<typename Predicate>
    std::vector<T*> findItems(Predicate pred) const {
        std::vector<T*> result;
        std::copy_if(items.begin(), items.end(), std::back_inserter(result), pred);
        return result;
    }
    
    // Универсальная функция поиска по нескольким предикатам (комбинация условий)
    // Пример: findItems(pred1, pred2, pred3) - найдет элементы, удовлетворяющие ВСЕМ условиям
    // Работает с C++11/14 через рекурсивный шаблон
    template<typename Predicate1, typename Predicate2>
    std::vector<T*> findItems(Predicate1 pred1, Predicate2 pred2) const {
        std::vector<T*> result;
        for (T* item : items) {
            if (item && pred1(item) && pred2(item)) {
                result.push_back(item);
            }
        }
        return result;
    }
    
    template<typename Predicate1, typename Predicate2, typename Predicate3>
    std::vector<T*> findItems(Predicate1 pred1, Predicate2 pred2, Predicate3 pred3) const {
        std::vector<T*> result;
        for (T* item : items) {
            if (item && pred1(item) && pred2(item) && pred3(item)) {
                result.push_back(item);
            }
        }
        return result;
    }
    
    // Универсальная функция сортировки по любому параметру
    // Пример: sortItems([](T* a, T* b) { return a->getPrice() < b->getPrice(); })
    template<typename Compare>
    void sortItems(Compare comp) {
        std::sort(items.begin(), items.end(), comp);
    }
    
    // Сортировка по возрастанию значения, полученного через функцию-аксессор
    // Пример: sortItemsBy([](T* item) { return item->getPrice(); })
    template<typename Accessor>
    void sortItemsBy(Accessor accessor, bool ascending = true) {
        if (ascending) {
            std::sort(items.begin(), items.end(), 
                [accessor](T* a, T* b) {
                    if (!a || !b) return false;
                    return accessor(a) < accessor(b);
                });
        } else {
            std::sort(items.begin(), items.end(), 
                [accessor](T* a, T* b) {
                    if (!a || !b) return false;
                    return accessor(a) > accessor(b);
                });
        }
    }
    
    // Поиск первого элемента, удовлетворяющего условию
    template<typename Predicate>
    T* findFirst(Predicate pred) const {
        auto it = std::find_if(items.begin(), items.end(), pred);
        return (it != items.end()) ? *it : nullptr;
    }
    
    // Подсчет элементов, удовлетворяющих условию
    template<typename Predicate>
    size_t countItems(Predicate pred) const {
        return std::count_if(items.begin(), items.end(), pred);
    }
};

#endif // MANAGER_H

