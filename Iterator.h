#pragma once
#include <vector>
#include <stdexcept>

template <typename T>
class Iterator {
private:
    std::vector<T>* vec;
    size_t index;

public:
    Iterator(std::vector<T>* v = nullptr, size_t i = 0);
    Iterator& operator++();
    Iterator& operator--();
    bool operator!=(const Iterator& other) const;
    bool operator==(const Iterator& other) const;
    auto& operator*() const;
    T* operator->() const;
};

#include "Iterator.cpp"
