#ifndef STACK_H
#define STACK_H

#include <stdexcept>
#include <utility>

template <typename T>
class Stack {
private:
    struct Node {
        T value;
        Node* next;
        Node(const T& v) : value(v), next(nullptr) {}
        Node(T&& v) : value(std::move(v)), next(nullptr) {}
    };

    Node* head = nullptr; 
    Node* tail = nullptr; 
    int sz = 0;

    void appendNode(const T& val) {
        Node* n = new Node(val);
        if (!tail) {
            head = tail = n;
        }
        else {
            tail->next = n;
            tail = n;
        }
        ++sz;
    }

    void destroyAll() noexcept {
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        head = tail = nullptr;
        sz = 0;
    }

public:
    Stack() = default;

    // Копирующий конструктор (глубокое копирование, сохраняет порядок элементов)
    Stack(const Stack& other) {
        Node* cur = other.head;
        while (cur) {
            appendNode(cur->value);
            cur = cur->next;
        }
    }

    // Перемещающий конструктор
    Stack(Stack&& other) noexcept
        : head(other.head), tail(other.tail), sz(other.sz)
    {
        other.head = other.tail = nullptr;
        other.sz = 0;
    }

    ~Stack() {
        destroyAll();
    }

    // Копирующее присваивание (copy-and-swap)
    Stack& operator=(const Stack& other) {
        if (this == &other) return *this;
        Stack tmp(other);
        swap(tmp);
        return *this;
    }

    // Перемещающее присваивание
    Stack& operator=(Stack&& other) noexcept {
        if (this == &other) return *this;
        destroyAll();
        head = other.head;
        tail = other.tail;
        sz = other.sz;
        other.head = other.tail = nullptr;
        other.sz = 0;
        return *this;
    }

    // Поменять содержимое местами
    void swap(Stack& other) noexcept {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(sz, other.sz);
    }

    // Добавление (push) — в вершину (O(1))
    void push(const T& value) {
        Node* n = new Node(value);
        n->next = head;
        head = n;
        if (!tail) tail = head;
        ++sz;
    }

    void push(T&& value) {
        Node* n = new Node(std::move(value));
        n->next = head;
        head = n;
        if (!tail) tail = head;
        ++sz;
    }

    // Удаление и возврат верхнего элемента. Бросает исключение, если стек пуст.
    T pop() {
        if (!head) throw std::runtime_error("Stack::pop(): empty stack");
        Node* n = head;
        T val = std::move(n->value);
        head = head->next;
        if (!head) tail = nullptr;
        delete n;
        --sz;
        return val;
    }

    // Доступ к верхнему элементу без удаления
    T& top() {
        if (!head) throw std::runtime_error("Stack::top(): empty stack");
        return head->value;
    }

    const T& top() const {
        if (!head) throw std::runtime_error("Stack::top(): empty stack");
        return head->value;
    }

    // Пуст ли стек
    bool isEmpty() const noexcept {
        return head == nullptr;
    }

    // Размер стека
    int size() const noexcept {
        return sz;
    }

    // Очистка
    void clear() noexcept {
        destroyAll();
    }
};

#endif // STACK_H