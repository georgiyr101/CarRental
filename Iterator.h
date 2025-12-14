#pragma once
#include <vector>
#include <stdexcept>

template <typename T>
class Iterator {
private:
    std::vector<T>* vec;
    size_t index;

public:
    Iterator(std::vector<T>* v = nullptr, size_t i = 0)
        : vec(v), index(i) {
    }

    Iterator& operator++() {
        if (!vec || index >= vec->size())
            throw std::runtime_error("Iterator out of range");
        ++index;
        return *this;
    }

    Iterator& operator--() {
        if (!vec || index == 0)
            throw std::runtime_error("Iterator out of range");
        --index;
        return *this;
    }

    bool operator!=(const Iterator& other) const {
        return vec != other.vec || index != other.index;
    }

    bool operator==(const Iterator& other) const {
        return vec == other.vec && index == other.index;
    }

    // возвращает объект, а не указатель!
    auto& operator*() const {
        return *(*vec)[index];
    }

    // вернёт указатель на объект T
    T* operator->() const {
        return (*vec)[index];
    }
};


