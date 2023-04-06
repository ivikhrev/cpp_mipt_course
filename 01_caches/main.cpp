#include <lru.hpp>
#include <lfu.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    int m, n, k;
    int lru_hits = 0, lfu_hits = 0;
    std::cin >> m >> n;
    LRUCache lru_cache(m);
    LFUCache lfu_cache(m);
    for (int i = 0; i < n; ++i) {
        std::cin >> k;
        if (lru_cache.get(k) != -1) {
            ++lru_hits;
        } else {
            lru_cache.put(k, k);
        }

        if (lfu_cache.get(k) != -1) {
            ++lfu_hits;
        } else {
            lfu_cache.put(k, k);
        }
    }

    std::cout << "LRU: " << lru_hits << "\n";
    std::cout << "LFU: " << lfu_hits << "\n";
    return 0;
}
