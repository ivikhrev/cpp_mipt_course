#include <list>
#include <unordered_map>

class LRUCache {
public:
    LRUCache() = default;
    LRUCache(int capacity) :capacity(capacity) {};

    bool full() const {
        return capacity == static_cast<int>(cache.size());
    }

    int get(int key) {
        auto hit = hash.find(key);
        if (hit == hash.end()) {
            return -1;
        }
        auto elit = hit->second;
        move_front(elit);
        return elit->second;
    }

    void put(int key, int value) {
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

private:
    using ListIt = typename std::list<std::pair<int, int>>::iterator;
    void move_front(const ListIt& elit) {
        if (elit != cache.begin()) {
            cache.splice(cache.begin(), cache, elit, std::next(elit));
        }
    }

    std::list<std::pair<int, int>> cache;
    std::unordered_map<int, ListIt> hash;
    int capacity;
};
