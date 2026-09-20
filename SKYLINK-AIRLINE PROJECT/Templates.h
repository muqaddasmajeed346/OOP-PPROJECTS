#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <vector>
#include <algorithm>

template <typename T, typename Predicate>
T* genericSearch(const std::vector<T*>& items, Predicate pred) {
    auto it = std::find_if(items.begin(), items.end(), pred);
    if (it != items.end()) {
        return *it;
    }
    return nullptr;
}
#endif
