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
    ~Container();
    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;
    void clear();
    Iterator<T*> begin();
    Iterator<T*> end();
    void add(const T& obj);
    void remove(int index);
    void update(int index, const std::function<void(T&)>& updater);
    void swap(int index1, int index2);
    bool undo();
    int size() const;
    T& operator[](int i);
    const T& operator[](int i) const;
    bool isValid(int i) const;
};
#include "Container.cpp"
#endif
