#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Iterator.h"
#include "Container.h"
#include <vector>

namespace Algorithm {
    template <typename T, typename Pred>
    Iterator<T*> find(Container<T>& container, Pred pred);

    template <typename T, typename Pred>
    Container<T> findAll(Container<T>& container, Pred pred);

    template <typename T, typename Comp>
    void bubbleSort(Container<T>& container, Comp comp);
}

#include "Algorithm.cpp"

#endif
