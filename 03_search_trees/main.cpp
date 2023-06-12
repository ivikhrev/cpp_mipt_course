#include "avl_tree.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

template<class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec) {\
    for (size_t i = 0; i < vec.size() - 1; ++i) {
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
            int key{};
            std::cout << "key ";
            std::cin >> key;
            std::cout << "\n";
            t.insert(key);
        }
        std::cout << "AVL Tree inorder keys" << "\n";
        std::cout << t.inorder_keys() << "\n";
        std::cout << "\n";
    }
    return 0;
}
