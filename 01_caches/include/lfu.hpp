#include <list>
#include <unordered_map>

class LFUCache {
public:
    LFUCache() = default;
    LFUCache(int capacity) : capacity(capacity) {}

    bool full() const {
        return capacity == static_cast<int>(cache.size());
    }

    int get(int key)  {
        auto hit = cache.find(key);
        if (hit == cache.end()) {
            return -1;
        }
        auto f = hit->second.first;
        auto elit = hit->second.second;
        int value = elit->second;
        freq[f + 1].emplace_front(elit->first, elit->second);
        hit->second.second = freq[f + 1].begin();
        freq[f].erase(elit);

        return value;
    }

    void put(int key, int value) {
        auto hit = get(key);
        if (hit == -1) {
            if (full()) {
                freq[min_freq].pop_back();
            }
            min_freq = 1;
            freq[min_freq].emplace_front(key, value);
            cache[key] = {min_freq , freq[min_freq].begin()};
        } else {
            cache[key].second->second = value;
        }
    }

private:
    using ListIt = typename std::list<std::pair<int, int>>::iterator;
    std::unordered_map<int, std::pair<int, ListIt>> cache;
    std::unordered_map<int, std::list<std::pair<int, int>> > freq;
    int capacity;
    int min_freq;
};
