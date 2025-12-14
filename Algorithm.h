#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Iterator.h"

class Algorithm {
public:

    // Поиск по предикату
    template <typename It, typename Pred>
    static It find(It begin, It end, Pred pred) {
        for (It it = begin; it != end; ++it)
            if (pred(*it))
                return it;
        return end;
    }

    /*template <typename It, typename Pred>
    static std::vector<It> findAll(It begin, It end, Pred pred) {
        std::vector<It> result;

        for (It it = begin; it != end; ++it) {
            if (pred(*it)) {
                result.push_back(it);
            }
        }

        return result;
    }*/

    // Bubble sort
    template <typename It, typename Comp>
    static void bubbleSort(It begin, It end, Comp comp) {
        bool swapped = true;
        while (swapped) {
            swapped = false;

            for (It it = begin; it != end; ++it) {
                It next = it;
                ++next;
                if (next == end) break;

                if (comp(*next, *it)) {
                    std::swap(*next, *it);
                    swapped = true;
                }
            }
        }
    }
};



#endif