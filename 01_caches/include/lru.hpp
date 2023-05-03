#include <list>
#include <unordered_map>

class LRUCache {
public:
    LRUCache() = default;
    LRUCache(int capacity) : capacity(capacity) {};

    bool full() const;
    int get(int key);
    void put(int key, int value);

private:
    using ListIt = typename std::list<std::pair<int, int>>::iterator;
    void move_front(const ListIt& elit);

    std::list<std::pair<int, int>> cache;
    std::unordered_map<int, ListIt> hash;
    int capacity;
};
