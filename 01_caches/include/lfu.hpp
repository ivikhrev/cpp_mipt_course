#include <list>
#include <unordered_map>

class LFUCache {
public:
    LFUCache() = default;
    LFUCache(int capacity) : capacity(capacity) {}

    bool full() const;
    int get(int key);
    void put(int key, int value);

private:
    using ListIt = typename std::list<std::pair<int, int>>::iterator;
    std::unordered_map<int, std::pair<int, ListIt>> cache;
    std::unordered_map<int, std::list<std::pair<int, int>> > freq;
    int capacity;
    int min_freq;
};
