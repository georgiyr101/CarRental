#ifndef ALGORITHM_CPP
#define ALGORITHM_CPP

#include "Algorithm.h"

template <typename T, typename Pred>
Iterator<T*> Algorithm::find(Container<T>& container, Pred pred) {
    for (auto it = container.begin(); it != container.end(); ++it) {
        if (pred(*it)) {
            return it;
        }
    }
    return container.end();
}

template <typename T, typename Pred>
Container<T> Algorithm::findAll(Container<T>& container, Pred pred) {
    Container<T> result;

    for (int i = 0; i < container.size(); ++i) {
        if (container.isValid(i) && pred(container[i])) {
            result.add(container[i]);
        }
    }

    return result;
}

template <typename T, typename Comp>
void Algorithm::bubbleSort(Container<T>& container, Comp comp) {
    int n = container.size();
    if (n <= 1) return;

    // Simple bubble sort: iterate through all pairs
    // Limit iterations to prevent infinite loops (n^2 is enough for bubble sort)
    int maxIterations = n * n;
    int iteration = 0;
    
    bool swapped = true;
    while (swapped && iteration < maxIterations) {
        swapped = false;
        ++iteration;
        
        for (int i = 0; i < n - 1; ++i) {
            // Skip invalid elements
            if (!container.isValid(i)) continue;
            
            // Find next valid element
            int j = i + 1;
            while (j < n && !container.isValid(j)) {
                ++j;
            }
            if (j >= n) break;
            
            // Compare and swap if needed
            if (comp(container[j], container[i])) {
                container.swap(i, j);
                swapped = true;
            }
        }
    }
}

#endif // ALGORITHM_CPP



