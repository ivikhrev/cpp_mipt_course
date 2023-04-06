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
        if (freq.find(f + 1) == freq.end()) {
            freq[f + 1] = {{elit->first, elit->second}};
        } else {
            freq[f + 1].emplace_front(elit->first, elit->second);
        }
        hit->second.first = f + 1;
        hit->second.second = freq[f + 1].begin();
        freq[f].erase(elit);
        if (f == min_freq && freq[f].empty()) {
            freq.erase(f);
            min_freq = f + 1;
        }

        return value;
    }

    void put(int key, int value) {
        auto hit = get(key);
        if (hit == -1) {
            if (full()) {
                auto [key, val] = freq[min_freq].back();
                cache.erase(key);
                freq[min_freq].pop_back();
                if (freq[min_freq].empty()) {
                    freq.erase(min_freq);
                }
            }
            min_freq = 1;
            if (freq.find(min_freq) == freq.end()) {
                freq[min_freq] = {{key, value}};
            } else {
                freq[min_freq].emplace_front(key, value);
            }
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
