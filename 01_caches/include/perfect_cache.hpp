#include <unordered_map>
#include <vector>
#include <list>
#include <set>

// perfect cache structure that "knows" the future
// therefore can implement perfect caching strategy -
// see the element that will be met later others and pop it
class PerfectCache {
public:
    PerfectCache() = default;
    PerfectCache(int capacity, const std::vector<int>& keys);

    bool full() const;
    int get(int key);
    void put(int key, int value);

private:
    using ListIt = typename std::list<std::pair<int, int>>::iterator;
    void move_front(const ListIt& elit);

    std::list<std::pair<int, int>> cache;
    std::unordered_map<int, ListIt> hash;

    std::unordered_map<int, std::list<int>> distances;
    std::set<std::pair<int, int>> max_distances;
    int capacity;
};
