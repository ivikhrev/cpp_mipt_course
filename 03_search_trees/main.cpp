#include "avl_tree/avl_tree.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

template<class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec) {\
    for (int i = 0; i < static_cast<int>(vec.size()) - 1; ++i) {
        out << vec[i] << " ";
    }

    if (!vec.empty()) {
        out << vec.back();
    }

    return out;
}

int main(int argc, char* argv[]) {
    AVLTree<int> t;

    std::string input;
    while (input != "q") {
        std::cout << "Select op:\n"
            "1. insert\n"
            "2. erase\n"
            "3. find\n"
            "4. kmin\n"
            "5. less_count\n";
        std::cin >> input;
        if (input == "1") {
            int n, key;
            std::cout << "N:";
            std::cin >> n;
            std::cout << "\nkey(-s): ";
            for (int i = 0; i < n; ++i) {
                std::cin >> key;
                t.insert(key);
            }
            std::cout << "\n";
        }
        if (input == "2") {
            int key{};
            std::cout << "key: ";
            std::cin >> key;
            std::cout << "\n";
            t.erase(key);
        }
        if (input == "3") {
            int key{};
            std::cout << "key: ";
            std::cin >> key;
            std::cout << "\n";
            auto* res = t.find(key);
            std::cout << (res == nullptr ? "Not found" : std::to_string(res->key)) << "\n";
        }
        if (input == "4") {
            int n{};
            std::cout << "N: ";
            std::cin >> n;
            std::cout << "\n";
            std::cout << t.kmin(n) << "\n";
        }
        if (input == "5") {
            int key{};
            std::cout << "key: ";
            std::cin >> key;
            std::cout << "\n";
            std::cout << t.less_count(key) << "\n";
        }
        std::cout << "AVL Tree inorder keys" << "\n";
        std::cout << t.inorder_keys() << "\n";
        std::cout << "\n";
    }
    return 0;
}
