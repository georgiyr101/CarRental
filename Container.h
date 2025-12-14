#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>
#include <functional>
#include <memory>
#include "Iterator.h"
#include "Stack.h"
#include "Exp_contr.h"

enum class ActionType { ADD, REMOVE, UPDATE };

template <typename T>
struct Action {
    ActionType type;
    std::unique_ptr<T> oldValue;
    std::unique_ptr<T> newValue;
    int index = -1;

    Action() = default;
    ~Action() = default;

    Action(Action&&) = default;
    Action& operator=(Action&&) = default;

    Action(const Action&) = delete;
    Action& operator=(const Action&) = delete;
};

template <typename T>
class Container {
private:
    std::vector<T*> data;
    Stack<Action<T>> undoStack;

public:
    Container() = default;

    ~Container() {
        clear();
    }

    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;

    void clear() {
        for (auto ptr : data) {
            delete ptr;
        }
        data.clear();
    }

    Iterator<T*> begin() { return Iterator<T*>(&data, 0); }
    Iterator<T*> end() { return Iterator<T*>(&data, data.size()); }

    void add(const T& obj) {
        T* newObj = new T(obj);
        data.push_back(newObj);

        Action<T> act;
        act.type = ActionType::ADD;
        act.newValue = std::make_unique<T>(obj);
        act.index = static_cast<int>(data.size() - 1);

        undoStack.push(std::move(act));
    }

    void remove(int index) {
        if (index < 0 || index >= static_cast<int>(data.size())) {
            throw Exp_contr(1, "Индекс вне диапазона при удалении. Индекс: " + std::to_string(index) + ", размер: " + std::to_string(data.size()));
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

    void update(int index, const std::function<void(T&)>& updater) {
        if (index < 0 || index >= static_cast<int>(data.size())) {
            throw Exp_contr(2, "Индекс вне диапазона при обновлении. Индекс: " + std::to_string(index) + ", размер: " + std::to_string(data.size()));
        }
        if (data[index] == nullptr) {
            throw Exp_contr(3, "Попытка обновить элемент с nullptr по индексу: " + std::to_string(index));
        }

        Action<T> act;
        act.type = ActionType::UPDATE;
        act.oldValue = std::make_unique<T>(*data[index]);

        updater(*data[index]);

        act.newValue = std::make_unique<T>(*data[index]);
        act.index = index;

        undoStack.push(std::move(act));
    }

    bool undo() {
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
                *data[act.index] = *act.oldValue; // ������ ����������� ������ ��������, ��� ��������
            }
            break;
        }

        return true;
    }

    int size() const { return static_cast<int>(data.size()); }

    T& operator[](int i) {
        if (i < 0 || i >= static_cast<int>(data.size())) {
            throw Exp_contr(4, "Индекс вне диапазона при доступе. Индекс: " + std::to_string(i) + ", размер: " + std::to_string(data.size()));
        }
        if (data[i] == nullptr) {
            throw Exp_contr(5, "Попытка доступа к элементу с nullptr по индексу: " + std::to_string(i));
        }
        return *data[i];
    }

    const T& operator[](int i) const {
        if (i < 0 || i >= static_cast<int>(data.size())) {
            throw Exp_contr(4, "Индекс вне диапазона при доступе. Индекс: " + std::to_string(i) + ", размер: " + std::to_string(data.size()));
        }
        if (data[i] == nullptr) {
            throw Exp_contr(5, "Попытка доступа к элементу с nullptr по индексу: " + std::to_string(i));
        }
        return *data[i];
    }

    bool isValid(int i) const {
        return i >= 0 && i < static_cast<int>(data.size()) && data[i] != nullptr;
    }
};

#endif
