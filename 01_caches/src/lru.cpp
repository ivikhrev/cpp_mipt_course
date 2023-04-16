#include "lru.hpp"

bool LRUCache::full() const {
    return capacity == static_cast<int>(cache.size());
}

int LRUCache::get(int key) {
    auto hit = hash.find(key);
    if (hit == hash.end()) {
        return -1;
    }
    auto elit = hit->second;
    move_front(elit);
    return elit->second;
}

void LRUCache::put(int key, int value) {
    auto hit = get(key);
    if (hit == -1) {
        if (full()) {
            hash.erase(cache.back().first);
            cache.pop_back();
        }
        cache.emplace_front(key, value);
        hash[key] = cache.begin();
    } else {
        hash[key]->second = value;
    }
}

void LRUCache::move_front(const ListIt& elit) {
    if (elit != cache.begin()) {
        cache.splice(cache.begin(), cache, elit, std::next(elit));
    }
}
