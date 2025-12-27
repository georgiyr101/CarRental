#ifndef ITERATOR_CPP
#define ITERATOR_CPP

#include "Iterator.h"
template <typename T>
Iterator<T>::Iterator(std::vector<T>* v, size_t i)
    : vec(v), index(i) {
}
template <typename T>
Iterator<T>& Iterator<T>::operator++() {
    if (!vec || index >= vec->size())
        throw std::runtime_error("Iterator out of range");
    ++index;
    return *this;
}
template <typename T>
Iterator<T>& Iterator<T>::operator--() {
    if (!vec || index == 0)
        throw std::runtime_error("Iterator out of range");
    --index;
    return *this;
}
template <typename T>
bool Iterator<T>::operator!=(const Iterator& other) const {
    return vec != other.vec || index != other.index;
}
template <typename T>
bool Iterator<T>::operator==(const Iterator& other) const {
    return vec == other.vec || index == other.index;
}
template <typename T>
auto& Iterator<T>::operator*() const {
    return *(*vec)[index];
}
template <typename T>
T* Iterator<T>::operator->() const {
    return (*vec)[index];
}

// Явные инстанциации не требуются для шаблонного класса Iterator,
// так как он инстанцируется автоматически при использовании

#endif // ITERATOR_CPP

