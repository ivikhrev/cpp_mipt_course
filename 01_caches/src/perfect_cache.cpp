#include "perfect_cache.hpp"

#include "lru.hpp"

#include <limits>
#include <queue>

PerfectCache::PerfectCache(int capacity, const std::vector<int>& keys) : capacity(capacity) {
    int i = 0;
    for (int k : keys) {
        if (distances.count(k) > 0) {
            distances[k].push_back(i);
        }
        else {
            distances[k] = {i};
        }

        ++i;
    }
}

bool PerfectCache::full() const {
    return capacity == static_cast<int>(cache.size());
}

int PerfectCache::get(int key) {
    auto hit = hash.find(key);
    if (hit == hash.end()) {
        return -1;
    }
    auto elit = hit->second;
    move_front(elit);
    max_distances.erase({distances[key].front(), key});
    distances[key].pop_front();
    if (distances[key].empty()) {
        distances[key].push_back(std::numeric_limits<int>::max());
    }
    max_distances.emplace(distances[key].front(), key);
    return elit->second;
}

void PerfectCache::put(int key, int value) {
    auto hit = get(key);
    if (hit == -1) {
        if (full()) {
            // hash.erase(cache.back().first);
            auto [distance, max_distance_key] = *max_distances.rbegin();
            max_distances.erase({distance, max_distance_key});
            cache.erase(hash[max_distance_key]);
            hash.erase(max_distance_key);
        }
        cache.emplace_front(key, value);
        hash[key] = cache.begin();

        // perfect cache impl
        distances[key].pop_front();
        if (distances[key].empty()) {
            distances[key].push_back(std::numeric_limits<int>::max());
        }
        max_distances.insert({distances[key].front(), key});
    } else {
        hash[key]->second = value;
    }
}

void PerfectCache::move_front(const ListIt& elit) {
    if (elit != cache.begin()) {
        cache.splice(cache.begin(), cache, elit, std::next(elit));
    }
}
