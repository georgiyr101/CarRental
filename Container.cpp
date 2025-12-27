#ifndef CONTAINER_CPP
#define CONTAINER_CPP

#include "Container.h"
#include <string>
template <typename T>
Container<T>::~Container() {
    clear();
}
template <typename T>
void Container<T>::clear() {
    for (auto ptr : data) { delete ptr;}
    data.clear();
}
template <typename T>
Iterator<T*> Container<T>::begin() {
    return Iterator<T*>(&data, 0);
}
template <typename T>
Iterator<T*> Container<T>::end() {
    return Iterator<T*>(&data, data.size());
}
template <typename T>
void Container<T>::add(const T& obj) {
    T* newObj = new T(obj);
    data.push_back(newObj);
    Action<T> act;
    act.type = ActionType::ADD;
    act.newValue = std::make_unique<T>(obj);
    act.index = static_cast<int>(data.size() - 1);
    undoStack.push(std::move(act));
}
template <typename T>
void Container<T>::remove(int index) {
    if (index < 0 || index >= static_cast<int>(data.size())) {
        throw Exp_contr(1, "Index out of range during removal. Index: " + std::to_string(index) + ", size: " + std::to_string(data.size()));
    }
    Action<T> act;
    act.type = ActionType::REMOVE;
    act.index = index;
    if (data[index] != nullptr) {
        act.oldValue = std::make_unique<T>(*data[index]);
    }
    undoStack.push(std::move(act));
    delete data[index];
    data.erase(data.begin() + index);
}
template <typename T>
void Container<T>::update(int index, const std::function<void(T&)>& updater) {
    if (index < 0 || index >= static_cast<int>(data.size())) {
        throw Exp_contr(2, "Index out of range during update. Index: " + std::to_string(index) + ", size: " + std::to_string(data.size()));
    }
    if (data[index] == nullptr) {
        throw Exp_contr(3, "Attempt to update element with nullptr at index: " + std::to_string(index));
    }
    Action<T> act;
    act.type = ActionType::UPDATE;
    act.oldValue = std::make_unique<T>(*data[index]);
    updater(*data[index]);
    act.newValue = std::make_unique<T>(*data[index]);
    act.index = index;
    undoStack.push(std::move(act));
}

template <typename T>
void Container<T>::swap(int index1, int index2) {
    if (index1 < 0 || index1 >= static_cast<int>(data.size()) ||
        index2 < 0 || index2 >= static_cast<int>(data.size())) {
        throw Exp_contr(6, "Index out of range during swap. Index1: " + std::to_string(index1) + ", Index2: " + std::to_string(index2) + ", size: " + std::to_string(data.size()));
    }
    if (data[index1] == nullptr || data[index2] == nullptr) {
        throw Exp_contr(7, "Attempt to swap element with nullptr. Index1: " + std::to_string(index1) + ", Index2: " + std::to_string(index2));
    }
    std::swap(data[index1], data[index2]);
}

template <typename T>
bool Container<T>::undo() {
    if (undoStack.isEmpty()) return false;
    Action<T> act = undoStack.pop();
    switch (act.type) {
    case ActionType::ADD:
        if (act.index >= 0 && act.index < static_cast<int>(data.size())) {
            delete data[act.index];
            data.erase(data.begin() + act.index);
        }
        break;
    case ActionType::REMOVE:
        if (act.oldValue != nullptr) {
            T* restored = new T(*act.oldValue);
            if (act.index >= 0 && act.index <= static_cast<int>(data.size())) {
                data.insert(data.begin() + act.index, restored);
            }
            else {
                data.push_back(restored);
            }
        }
        break;
    case ActionType::UPDATE:
        if (act.oldValue != nullptr && act.index >= 0 && act.index < static_cast<int>(data.size()) && data[act.index] != nullptr) {
            *data[act.index] = *act.oldValue;
        }
        break;
    }
    return true;
}
template <typename T>
int Container<T>::size() const {
    return static_cast<int>(data.size());
}
template <typename T>
T& Container<T>::operator[](int i) {
    if (i < 0 || i >= static_cast<int>(data.size())) {
        throw Exp_contr(4, "Index out of range during access. Index: " + std::to_string(i) + ", size: " + std::to_string(data.size()));
    }
    if (data[i] == nullptr) {
        throw Exp_contr(5, "Attempt to access element with nullptr at index: " + std::to_string(i));
    }
    return *data[i];
}
template <typename T>
const T& Container<T>::operator[](int i) const {
    if (i < 0 || i >= static_cast<int>(data.size())) {
        throw Exp_contr(4, "Index out of range during access. Index: " + std::to_string(i) + ", size: " + std::to_string(data.size()));
    }
    if (data[i] == nullptr) {
        throw Exp_contr(5, "Attempt to access element with nullptr at index: " + std::to_string(i));
    }
    return *data[i];
}
template <typename T>
bool Container<T>::isValid(int i) const {
    return i >= 0 && i < static_cast<int>(data.size()) && data[i] != nullptr;
}
// Явные инстанциации для используемых типов
#include "EconomyCar.h"
#include "PremiumCar.h"
#include "Truck.h"
#include "ElectricCar.h"
#include "Client.h"
#include "Order.h"

template class Container<EconomyCar>;
template class Container<PremiumCar>;
template class Container<Truck>;
template class Container<ElectricCar>;
template class Container<Client>;
template class Container<Order>;

#endif // CONTAINER_CPP



