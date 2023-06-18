// #include "node.hpp"
#include "avl_tree/avl_node.hpp"
#include "avl_tree/avl_tree.hpp"
#include "rb_tree/rb_node.hpp"
#include "rb_tree/rb_tree.hpp"
#include "common.hpp"

#include <utils/test_utils.hpp>

#include <gtest/gtest.h>

#include <cmath>
#include <filesystem>
#include <fstream>
#include <numeric>
#include <vector>

namespace fs = std::filesystem;

template <typename T>
class TreeNodes : public testing::Test {
public:
    T t;
};

using NodeTypes = testing::Types<AVLNode<int>, RBNode<int>>;
TYPED_TEST_SUITE(TreeNodes, NodeTypes);

TYPED_TEST(TreeNodes, CanConstructWithParameters) {
    ASSERT_NO_THROW((decltype(this->t){0}));
}

TYPED_TEST(TreeNodes, CanConstructWithAllParameters) {
    decltype(this->t) parent{0};
    decltype(this->t) left{1};
    decltype(this->t) right{2};
    ASSERT_NO_THROW((decltype(this->t){0, &parent, &left, &right}));
}

template <typename T>
class SearchingTrees : public testing::Test {
public:
    T t;
};

using TreeTypes = testing::Types<AVLTree<int>, RBTree<int>>;
TYPED_TEST_SUITE(SearchingTrees, TreeTypes);

TYPED_TEST(SearchingTrees, CanDefaultConstruct) {
    ASSERT_NO_THROW(((decltype(this->t){})));
}


TYPED_TEST(SearchingTrees, CanConstructFromVector) {
    ASSERT_NO_THROW((decltype(this->t)({1, 2, 3})));
}

TYPED_TEST(SearchingTrees, InorderTraversal) {
    std::vector<int> v{2, 1, 3};
    std::vector<int> expected{1, 2, 3};
    auto t = decltype(this->t)(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, CanCopy) {
    auto t = decltype(this->t)({1, 2, 3, 4, 5});
    auto copy = t;
    ASSERT_EQ(t.inorder_keys(), copy.inorder_keys());
}

TYPED_TEST(SearchingTrees, TrueDeepCopy) {
    auto t = decltype(this->t)({1, 2, 4, 5});
    auto copy = t;
    ASSERT_EQ(t.inorder_keys(), copy.inorder_keys());
    t.insert(3);
    copy.insert(3);
    ASSERT_EQ(t.inorder_keys(), copy.inorder_keys());
}

TYPED_TEST(SearchingTrees, CopyCorrectKmin) {
    auto t = decltype(this->t)({1, 2, 3, 4, 5});
    auto copy = t;
    ASSERT_EQ(t.kmin(1), copy.kmin(1));
    ASSERT_EQ(t.kmin(2), copy.kmin(2));
    ASSERT_EQ(t.kmin(3), copy.kmin(3));
    ASSERT_EQ(t.kmin(4), copy.kmin(4));
    ASSERT_EQ(t.kmin(5), copy.kmin(5));
}

TYPED_TEST(SearchingTrees, CopyCorrectLessCount) {
    auto t = decltype(this->t)({1, 2, 3, 4, 5});
    auto copy = t;
    ASSERT_EQ(t.less_count(1), copy.less_count(1));
    ASSERT_EQ(t.less_count(2), copy.less_count(2));
    ASSERT_EQ(t.less_count(3), copy.less_count(3));
    ASSERT_EQ(t.less_count(4), copy.less_count(4));
    ASSERT_EQ(t.less_count(5), copy.less_count(5));
}

TYPED_TEST(SearchingTrees, LeftRotateRoot) {
    std::vector<int> v{1, 2, 3};
    std::vector<int> expected{1, 2, 3};
    auto t = decltype(this->t)(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, LeftRotate) {
    std::vector<int> v{2, 1, 3, 4, 5};
    std::vector<int> expected{1, 2, 3, 4, 5};
    auto t = decltype(this->t)(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, RightRotateRoot) {
    std::vector<int> v{3, 2, 1};
    std::vector<int> expected{1, 2, 3};
    auto t = decltype(this->t)(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, RightRotate) {
    std::vector<int> v{4, 5, 3, 2, 1};
    std::vector<int> expected{1, 2, 3, 4, 5};
    auto t = decltype(this->t)(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, LeftRightRotate) {
    std::vector<int> v{3, 1, 2};
    std::vector<int> expected{1, 2, 3};
    auto t = decltype(this->t)(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, ComplexLeftRightRotate) {
    std::vector<int> v{5, 1, 3, 2, 4, 0};
    std::vector<int> expected{0, 1, 2, 3, 4, 5};
    auto t = decltype(this->t)(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, RightLeftRotate) {
    std::vector<int> v{1, 3 , 2};
    std::vector<int> expected{1, 2, 3};
    auto t = decltype(this->t)(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, ComplexRightLeftRotate) {
    std::vector<int> v{0, 4, 5, 2, 1, 3};
    std::vector<int> expected{0, 1, 2, 3, 4, 5};
    auto t = decltype(this->t)(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, TreeConstruct) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{1, 3, 5, 6, 7 , 9};
    auto t = decltype(this->t)(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, Insert) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{1, 2, 3, 5, 6, 7 , 9};
    auto t = decltype(this->t)(v);
    t.insert(2);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, Find) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    auto t = decltype(this->t)(v);
    ASSERT_EQ(1, t.find(1)->key);
    ASSERT_EQ(nullptr, t.find(2));
    ASSERT_EQ(3, t.find(3)->key);
    ASSERT_EQ(nullptr, t.find(4));
    ASSERT_EQ(5, t.find(5)->key);
    ASSERT_EQ(6, t.find(6)->key);
    ASSERT_EQ(7, t.find(7)->key);
    ASSERT_EQ(9, t.find(9)->key);
}

TYPED_TEST(SearchingTrees, FindAfterInsert) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    auto t = decltype(this->t)(v);
    ASSERT_EQ(1, t.find(1)->key);
    ASSERT_EQ(nullptr, t.find(2));
    ASSERT_EQ(3, t.find(3)->key);
    ASSERT_EQ(nullptr, t.find(4));
    ASSERT_EQ(5, t.find(5)->key);
    ASSERT_EQ(6, t.find(6)->key);
    ASSERT_EQ(7, t.find(7)->key);
    ASSERT_EQ(9, t.find(9)->key);
    t.insert(2);
    ASSERT_EQ(2, t.find(2)->key);
}

TYPED_TEST(SearchingTrees, EraseLeaf) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{1, 5, 6, 7 , 9};
    auto t = decltype(this->t)(v);
    t.erase(3);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, EraseRoot0) {
    std::vector<int> v{2, 3};
    std::vector<int> expected{3};
    auto t = decltype(this->t)(v);
    t.erase(2);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, EraseRoot1) {
    std::vector<int> v{3, 2};
    std::vector<int> expected{2};
    auto t = decltype(this->t)(v);
    t.erase(3);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, EraseRoot2) {
    std::vector<int> v{3, 2, 1};
    std::vector<int> expected{2, 3};
    auto t = decltype(this->t)(v);
    t.erase(1);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, EraseRoot3) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{1, 3, 6, 7 , 9};
    auto t = decltype(this->t)(v);
    t.erase(5);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, EraseMiddle1) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{3, 5, 6, 7 , 9};
    auto t = decltype(this->t)(v);
    t.erase(1);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, EraseMiddle2) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{1, 3, 5, 6, 9};
    auto t = decltype(this->t)(v);
    t.erase(7);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, EraseMiddle3) {
    std::vector<int> v{5, 1, 7, 6, 3};
    std::vector<int> expected{1, 3, 5, 6};
    auto t = decltype(this->t)(v);
    t.erase(7);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, EraseMiddle4) {
    std::vector<int> v{5, 1, 7, 6, 9, 0};
    std::vector<int> expected{0, 5, 6, 7, 9};
    auto t = decltype(this->t)(v);
    t.erase(1);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, EraseMiddle5) {
    std::vector<int> v{5, 1, 3, 2, 7, 6, 0, -1, 4, 8, 9, 10, 11, 12};
    std::vector<int> expected{-1, 0, 2, 3, 4, 5, 6, 7 ,8, 9, 10, 11, 12};
    auto t = decltype(this->t)(v);
    t.erase(1);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, EraseMiddle6) {
    std::vector<int> v{5, 1, 3, 2, 7, 6, 0, -1, 4, 8, 9, 10, 11, 12};
    std::vector<int> expected{-1, 0, 1, 2, 3, 4, 5, 7 ,8, 9, 10, 11, 12};
    auto t = decltype(this->t)(v);
    t.erase(6);
    ASSERT_EQ(expected, t.inorder_keys());
}

TYPED_TEST(SearchingTrees, EraseFullTree1) {
    std::vector<int> v{5, 1, 3, 2, 7, 6, 0, -1, 4, 8, 9, 10, 11, 12};
    std::vector<int> expected{-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    auto t = decltype(this->t)(v);
    for (size_t i = 0; i < v.size(); ++i) {
        t.erase(v[i]);
        expected.erase(std::find(expected.begin(), expected.end(), v[i]));
        ASSERT_EQ(expected, t.inorder_keys());
    }
}

TYPED_TEST(SearchingTrees, EraseFullTree2) {
    std::vector<int> expected(100);
    std::iota(expected.begin(), expected.end(), 0);
    std::vector<int> copy = expected;
    auto t = decltype(this->t)(expected);
    for (size_t i = 0; i < 100; ++i) {
        t.erase(copy[i]);
        expected.erase(std::find(expected.begin(), expected.end(), copy[i]));
        ASSERT_EQ(expected, t.inorder_keys());
    }
}

TYPED_TEST(SearchingTrees, KthMin) {
    std::vector<int> v{0, 1, 2, 3, 4, 5};
    std::vector<int> expected{0, 1, 2, 3, 4, 5};
    auto t = decltype(this->t)(v);
    ASSERT_EQ(expected, t.inorder_keys());
    ASSERT_EQ(0, t.kmin(1));
    ASSERT_EQ(1, t.kmin(2));
    ASSERT_EQ(2, t.kmin(3));
    ASSERT_EQ(3, t.kmin(4));
    ASSERT_EQ(4, t.kmin(5));
    ASSERT_EQ(5, t.kmin(6));
}

TYPED_TEST(SearchingTrees, LessCount) {
    std::vector<int> v{0, 1, 2, 3, 4, 5};
    std::vector<int> expected{0, 1, 2, 3, 4, 5};
    auto t = decltype(this->t)(v);
    ASSERT_EQ(expected, t.inorder_keys());
    ASSERT_EQ(0, t.less_count(0));
    ASSERT_EQ(1, t.less_count(1));
    ASSERT_EQ(2, t.less_count(2));
    ASSERT_EQ(3, t.less_count(3));
    ASSERT_EQ(4, t.less_count(4));
    ASSERT_EQ(5, t.less_count(5));
}

template<class Tree>
class End2EndTreeFixtureTests : public testing::Test {
public:
    static const std::string& data_directory() {
        // Establish the directory name only once for the application.
        static const std::string data_dir = [] {
            // Look for an environment variable specifying the directory name.
            const char* env_dir = std::getenv("TEST_DATA_DIR");
            if (env_dir == nullptr) {
                // If an environment variable is not set, rely on the definition
                // coming from the build system.
                return std::string(TEST_DATA_DIR);
            }
            // Initialise the directory name using the environment variable.
            return std::string(env_dir);
        }();

        // Return the previously initialised variable.
        return data_dir;
    }

protected:
    std::vector<std::string> read_input_data(const fs::path& file_path) {
        std::ifstream file(file_path);
        std::vector<std::string> input;
        std::string s;
        if (file.is_open()) {
            while (file >> s) {
                input.push_back(s);
            }
        }
        return input;
    }

    std::vector<int> read_answer_data(const fs::path& file_path) {
        std::ifstream file(file_path);
        std::vector<int> answer;
        if (file.is_open()) {
            int val;
            while (file >> val) {
                answer.push_back(val);
            }
        } else {
            throw std::runtime_error("Can't open file" + file_path.string());
        }

        return answer;
    }

    std::vector<int> calc_answer(const std::vector<std::string>& input) {
        Tree t;
        std::vector<int> answer;
        for (size_t i = 0; i < input.size() - 2; i += 2) {
            std::string op = input[i];
            int num = std::stoi(input[i + 1]);
            if (op == "k") {
                t.insert(num);
            }
            if (op == "m") {
                answer.push_back(t.kmin(num));
            }
            if (op == "n") {
                answer.push_back(t.less_count(num));
            }
        }
        return answer;
    }
};

TYPED_TEST_SUITE(End2EndTreeFixtureTests, TreeTypes);

TYPED_TEST(End2EndTreeFixtureTests, End2EndTest) {
    for (auto file_str : get_files_in_dir(this->data_directory())) {
        auto input_file = fs::path(file_str);
        auto answer_file = input_file.parent_path() / "answers" / input_file.filename();
        auto input =this->read_input_data(input_file);

        ASSERT_EQ(this->read_answer_data(answer_file), this->calc_answer(input))
            << "on input data: " << input_file << '\n'
            << "answer file: " << answer_file << '\n';
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
