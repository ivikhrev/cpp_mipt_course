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

// AVL Trees tests

TEST(AVLNode, CanConstructWithParameters) {
    ASSERT_NO_THROW((AVLNode<int>{0}));
}

TEST(AVLNode, CanConstructWithAllParameters) {
    AVLNode<int> parent{0};
    AVLNode<int> left{1};
    AVLNode<int> right{2};
    ASSERT_NO_THROW((AVLNode<int>{0, &parent, &left, &right}));
}

TEST(AVLTree, CanDefaultConstruct) {
    ASSERT_NO_THROW((AVLTree<int>{}));
}


TEST(AVLTree, CanConstructFromVector) {
    ASSERT_NO_THROW(AVLTree<int>({1, 2, 3}));
}

TEST(AVLTree, InorderTraversal) {
    std::vector<int> v{2, 1, 3};
    std::vector<int> expected{1, 2, 3};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, CanCopy) {
    auto t = AVLTree<int>({1, 2, 3, 4, 5});
    auto copy = t;
    ASSERT_EQ(t.inorder_keys(), copy.inorder_keys());
}

TEST(AVLTree, TrueDeepCopy) {
    auto t = AVLTree<int>({1, 2, 4, 5});
    auto copy = t;
    ASSERT_EQ(t.inorder_keys(), copy.inorder_keys());
    t.insert(3);
    copy.insert(3);
    ASSERT_EQ(t.inorder_keys(), copy.inorder_keys());
}

TEST(AVLTree, CopyCorrectKmin) {
    auto t = AVLTree<int>({1, 2, 3, 4, 5});
    auto copy = t;
    ASSERT_EQ(t.kmin(1), copy.kmin(1));
    ASSERT_EQ(t.kmin(2), copy.kmin(2));
    ASSERT_EQ(t.kmin(3), copy.kmin(3));
    ASSERT_EQ(t.kmin(4), copy.kmin(4));
    ASSERT_EQ(t.kmin(5), copy.kmin(5));
}

TEST(AVLTree, CopyCorrectLessCount) {
    auto t = AVLTree<int>({1, 2, 3, 4, 5});
    auto copy = t;
    ASSERT_EQ(t.less_count(1), copy.less_count(1));
    ASSERT_EQ(t.less_count(2), copy.less_count(2));
    ASSERT_EQ(t.less_count(3), copy.less_count(3));
    ASSERT_EQ(t.less_count(4), copy.less_count(4));
    ASSERT_EQ(t.less_count(5), copy.less_count(5));
}

TEST(AVLTree, LeftRotateRoot) {
    std::vector<int> v{1, 2, 3};
    std::vector<int> expected{1, 2, 3};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, LeftRotate) {
    std::vector<int> v{2, 1, 3, 4, 5};
    std::vector<int> expected{1, 2, 3, 4, 5};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, RightRotateRoot) {
    std::vector<int> v{3, 2, 1};
    std::vector<int> expected{1, 2, 3};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, RightRotate) {
    std::vector<int> v{4, 5, 3, 2, 1};
    std::vector<int> expected{1, 2, 3, 4, 5};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, LeftRightRotate) {
    std::vector<int> v{3, 1, 2};
    std::vector<int> expected{1, 2, 3};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, ComplexLeftRightRotate) {
    std::vector<int> v{5, 1, 3, 2, 4, 0};
    std::vector<int> expected{0, 1, 2, 3, 4, 5};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, RightLeftRotate) {
    std::vector<int> v{1, 3 , 2};
    std::vector<int> expected{1, 2, 3};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, ComplexRightLeftRotate) {
    std::vector<int> v{0, 4, 5, 2, 1, 3};
    std::vector<int> expected{0, 1, 2, 3, 4, 5};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, TreeConstruct) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{1, 3, 5, 6, 7 , 9};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, Insert) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{1, 2, 3, 5, 6, 7 , 9};
    auto t = AVLTree<int>(v);
    t.insert(2);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, Find) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(1, t.find(1)->key);
    ASSERT_EQ(nullptr, t.find(2));
    ASSERT_EQ(3, t.find(3)->key);
    ASSERT_EQ(nullptr, t.find(4));
    ASSERT_EQ(5, t.find(5)->key);
    ASSERT_EQ(6, t.find(6)->key);
    ASSERT_EQ(7, t.find(7)->key);
    ASSERT_EQ(9, t.find(9)->key);
}

TEST(AVLTree, FindAfterInsert) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    auto t = AVLTree<int>(v);
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

TEST(AVLTree, EraseLeaf) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{1, 5, 6, 7 , 9};
    auto t = AVLTree<int>(v);
    t.erase(3);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, EraseRoot0) {
    std::vector<int> v{2, 3};
    std::vector<int> expected{3};
    auto t = AVLTree<int>(v);
    t.erase(2);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, EraseRoot1) {
    std::vector<int> v{3, 2};
    std::vector<int> expected{2};
    auto t = AVLTree<int>(v);
    t.erase(3);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, EraseRoot2) {
    std::vector<int> v{3, 2, 1};
    std::vector<int> expected{2, 3};
    auto t = AVLTree<int>(v);
    t.erase(1);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, EraseRoot3) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{1, 3, 6, 7 , 9};
    auto t = AVLTree<int>(v);
    t.erase(5);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, EraseMiddle1) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{3, 5, 6, 7 , 9};
    auto t = AVLTree<int>(v);
    t.erase(1);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, EraseMiddle2) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{1, 3, 5, 6, 9};
    auto t = AVLTree<int>(v);
    t.erase(7);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, EraseMiddle3) {
    std::vector<int> v{5, 1, 7, 6, 3};
    std::vector<int> expected{1, 3, 5, 6};
    auto t = AVLTree<int>(v);
    t.erase(7);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, EraseMiddle4) {
    std::vector<int> v{5, 1, 7, 6, 9, 0};
    std::vector<int> expected{0, 5, 6, 7, 9};
    auto t = AVLTree<int>(v);
    t.erase(1);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, EraseMiddle5) {
    std::vector<int> v{5, 1, 3, 2, 7, 6, 0, -1, 4, 8, 9, 10, 11, 12};
    std::vector<int> expected{-1, 0, 2, 3, 4, 5, 6, 7 ,8, 9, 10, 11, 12};
    auto t = AVLTree<int>(v);
    t.erase(1);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, EraseMiddle6) {
    std::vector<int> v{5, 1, 3, 2, 7, 6, 0, -1, 4, 8, 9, 10, 11, 12};
    std::vector<int> expected{-1, 0, 1, 2, 3, 4, 5, 7 ,8, 9, 10, 11, 12};
    auto t = AVLTree<int>(v);
    t.erase(6);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(AVLTree, EraseStressTest1) {
    std::vector<int> v{5, 1, 3, 2, 7, 6, 0, -1, 4, 8, 9, 10, 11, 12};
    std::vector<int> expected{-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    auto t = AVLTree<int>(v);
    for (size_t i = 0; i < v.size(); ++i) {
        t.erase(v[i]);
        expected.erase(std::find(expected.begin(), expected.end(), v[i]));
        ASSERT_EQ(expected, t.inorder_keys());
    }
}

TEST(AVLTree, EraseStressTest2) {
    std::vector<int> expected(100);
    std::iota(expected.begin(), expected.end(), 0);
    std::vector<int> copy = expected;
    auto t = AVLTree<int>(expected);
    for (size_t i = 0; i < 100; ++i) {
        t.erase(copy[i]);
        expected.erase(std::find(expected.begin(), expected.end(), copy[i]));
        ASSERT_EQ(expected, t.inorder_keys());
    }
}

TEST(AVLTree, KthMin) {
    std::vector<int> v{0, 1, 2, 3, 4, 5};
    std::vector<int> expected{0, 1, 2, 3, 4, 5};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
    ASSERT_EQ(0, t.kmin(1));
    ASSERT_EQ(1, t.kmin(2));
    ASSERT_EQ(2, t.kmin(3));
    ASSERT_EQ(3, t.kmin(4));
    ASSERT_EQ(4, t.kmin(5));
    ASSERT_EQ(5, t.kmin(6));
}

TEST(AVLTree, LessCount) {
    std::vector<int> v{0, 1, 2, 3, 4, 5};
    std::vector<int> expected{0, 1, 2, 3, 4, 5};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
    ASSERT_EQ(0, t.less_count(0));
    ASSERT_EQ(1, t.less_count(1));
    ASSERT_EQ(2, t.less_count(2));
    ASSERT_EQ(3, t.less_count(3));
    ASSERT_EQ(4, t.less_count(4));
    ASSERT_EQ(5, t.less_count(5));
}

class AVLTreeFixtureTests : public testing::TestWithParam<std::string> {
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
        AVLTree<int> t;
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

TEST_P(AVLTreeFixtureTests, End2EndTest) {
    auto input_file = fs::path(GetParam());
    auto answer_file = input_file.parent_path() / "answers" / input_file.filename();
    auto input = read_input_data(input_file);

    ASSERT_EQ(read_answer_data(answer_file), calc_answer(input))
        << "on input data: " << input_file << '\n'
        << "answer file: " << answer_file << '\n';;
}

INSTANTIATE_TEST_SUITE_P(AVLTree,
                         AVLTreeFixtureTests,
                         ::testing::ValuesIn(get_files_in_dir(AVLTreeFixtureTests::data_directory()))
);

// RB Trees tests

TEST(RBNode, CanConstructWithParameters) {
    ASSERT_NO_THROW((RBNode<int>{0}));
}

TEST(RBNode, CanConstructWithAllParameters) {
    RBNode<int> parent{0};
    RBNode<int> left{1};
    RBNode<int> right{2};
    ASSERT_NO_THROW((RBNode<int>{0, &parent, &left, &right}));
}

TEST(RBTree, CanDefaultConstruct) {
    ASSERT_NO_THROW((RBTree<int>{}));
}

TEST(RBTree, CanConstructFromVector) {
    ASSERT_NO_THROW(RBTree<int>({1, 2, 3}));
}

TEST(RBTree, InorderTraversal) {
    std::vector<int> v{2, 1, 3};
    std::vector<int> expected{1, 2, 3};
    auto t = RBTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, LeftRotateRoot) {
    std::vector<int> v{1, 2, 3};
    std::vector<int> expected{1, 2, 3};
    auto t = RBTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, LeftRotate) {
    std::vector<int> v{2, 1, 3, 4, 5};
    std::vector<int> expected{1, 2, 3, 4, 5};
    auto t = RBTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, RightRotateRoot) {
    std::vector<int> v{3, 2, 1};
    std::vector<int> expected{1, 2, 3};
    auto t = RBTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, RightRotate) {
    std::vector<int> v{4, 5, 3, 2, 1};
    std::vector<int> expected{1, 2, 3, 4, 5};
    auto t = RBTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, LeftRightRotate) {
    std::vector<int> v{3, 1, 2};
    std::vector<int> expected{1, 2, 3};
    auto t = RBTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, ComplexLeftRightRotate) {
    std::vector<int> v{5, 1, 3, 2, 4, 0};
    std::vector<int> expected{0, 1, 2, 3, 4, 5};
    auto t = RBTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, RightLeftRotate) {
    std::vector<int> v{1, 3 , 2};
    std::vector<int> expected{1, 2, 3};
    auto t = RBTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, ComplexRightLeftRotate) {
    std::vector<int> v{0, 4, 5, 2, 1, 3};
    std::vector<int> expected{0, 1, 2, 3, 4, 5};
    auto t = RBTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, TreeConstruct) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{1, 3, 5, 6, 7 , 9};
    auto t = RBTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, Insert) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{1, 2, 3, 5, 6, 7 , 9};
    auto t = RBTree<int>(v);
    t.insert(2);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, Find) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    auto t = RBTree<int>(v);
    ASSERT_EQ(1, t.find(1)->key);
    ASSERT_EQ(nullptr, t.find(2));
    ASSERT_EQ(3, t.find(3)->key);
    ASSERT_EQ(nullptr, t.find(4));
    ASSERT_EQ(5, t.find(5)->key);
    ASSERT_EQ(6, t.find(6)->key);
    ASSERT_EQ(7, t.find(7)->key);
    ASSERT_EQ(9, t.find(9)->key);
}

TEST(RBTree, FindAfterInsert) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    auto t = RBTree<int>(v);
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

TEST(RBTree, CanCopy) {
    auto t = RBTree<int>({1, 2, 3, 4, 5});
    auto copy = t;
    ASSERT_EQ(t.inorder_keys(), copy.inorder_keys());
}

TEST(RBTree, TrueDeepCopy) {
    auto t = RBTree<int>({1, 2, 4, 5});
    auto copy = t;
    ASSERT_EQ(t.inorder_keys(), copy.inorder_keys());
    t.insert(3);
    copy.insert(3);
    ASSERT_EQ(t.inorder_keys(), copy.inorder_keys());
}

TEST(RBTree, CopyCorrectKmin) {
    auto t = RBTree<int>({1, 2, 3, 4, 5});
    auto copy = t;
    ASSERT_EQ(t.kmin(1), copy.kmin(1));
    ASSERT_EQ(t.kmin(2), copy.kmin(2));
    ASSERT_EQ(t.kmin(3), copy.kmin(3));
    ASSERT_EQ(t.kmin(4), copy.kmin(4));
    ASSERT_EQ(t.kmin(5), copy.kmin(5));
}

TEST(RBTree, CopyCorrectLessCount) {
    auto t = RBTree<int>({1, 2, 3, 4, 5});
    auto copy = t;
    ASSERT_EQ(t.less_count(1), copy.less_count(1));
    ASSERT_EQ(t.less_count(2), copy.less_count(2));
    ASSERT_EQ(t.less_count(3), copy.less_count(3));
    ASSERT_EQ(t.less_count(4), copy.less_count(4));
    ASSERT_EQ(t.less_count(5), copy.less_count(5));
}

TEST(RBTree, KthMin) {
    std::vector<int> v{0, 1, 2, 3, 4, 5};
    std::vector<int> expected{0, 1, 2, 3, 4, 5};
    auto t = RBTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
    ASSERT_EQ(0, t.kmin(1));
    ASSERT_EQ(1, t.kmin(2));
    ASSERT_EQ(2, t.kmin(3));
    ASSERT_EQ(3, t.kmin(4));
    ASSERT_EQ(4, t.kmin(5));
    ASSERT_EQ(5, t.kmin(6));
}

TEST(RBTree, LessCount) {
    std::vector<int> v{0, 1, 2, 3, 4, 5};
    std::vector<int> expected{0, 1, 2, 3, 4, 5};
    auto t = RBTree<int>(v);
    ASSERT_EQ(expected, t.inorder_keys());
    ASSERT_EQ(0, t.less_count(0));
    ASSERT_EQ(1, t.less_count(1));
    ASSERT_EQ(2, t.less_count(2));
    ASSERT_EQ(3, t.less_count(3));
    ASSERT_EQ(4, t.less_count(4));
    ASSERT_EQ(5, t.less_count(5));
}

class RBTreeFixtureTests : public testing::TestWithParam<std::string> {
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
        RBTree<int> t;
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

TEST_P(RBTreeFixtureTests, End2EndTest) {
    auto input_file = fs::path(GetParam());
    auto answer_file = input_file.parent_path() / "answers" / input_file.filename();
    auto input = read_input_data(input_file);

    ASSERT_EQ(read_answer_data(answer_file), calc_answer(input))
        << "on input data: " << input_file << '\n'
        << "answer file: " << answer_file << '\n';;
}

INSTANTIATE_TEST_SUITE_P(RBTree,
                         RBTreeFixtureTests,
                         ::testing::ValuesIn(get_files_in_dir(RBTreeFixtureTests::data_directory()))
);


TEST(RBTree, EraseLeaf) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{1, 5, 6, 7 , 9};
    auto t = RBTree<int>(v);
    t.erase(3);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, EraseRoot0) {
    std::vector<int> v{2, 3};
    std::vector<int> expected{3};
    auto t = RBTree<int>(v);
    t.erase(2);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, EraseRoot1) {
    std::vector<int> v{3, 2};
    std::vector<int> expected{2};
    auto t = RBTree<int>(v);
    t.erase(3);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, EraseRoot2) {
    std::vector<int> v{3, 2, 1};
    std::vector<int> expected{2, 3};
    auto t = RBTree<int>(v);
    t.erase(1);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, EraseRoot3) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{1, 3, 6, 7 , 9};
    auto t = RBTree<int>(v);
    t.erase(5);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, EraseMiddle1) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{3, 5, 6, 7 , 9};
    auto t = RBTree<int>(v);
    t.erase(1);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, EraseMiddle2) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{1, 3, 5, 6, 9};
    auto t = RBTree<int>(v);
    t.erase(7);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, EraseMiddle3) {
    std::vector<int> v{5, 1, 7, 6, 3};
    std::vector<int> expected{1, 3, 5, 6};
    auto t = RBTree<int>(v);
    t.erase(7);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, EraseMiddle4) {
    std::vector<int> v{5, 1, 7, 6, 9, 0};
    std::vector<int> expected{0, 5, 6, 7, 9};
    auto t = RBTree<int>(v);
    t.erase(1);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, EraseMiddle5) {
    std::vector<int> v{5, 1, 3, 2, 7, 6, 0, -1, 4, 8, 9, 10, 11, 12};
    std::vector<int> expected{-1, 0, 2, 3, 4, 5, 6, 7 ,8, 9, 10, 11, 12};
    auto t = RBTree<int>(v);
    t.erase(1);
    ASSERT_EQ(expected, t.inorder_keys());
}

TEST(RBTree, EraseMiddle6) {
    std::vector<int> v{5, 1, 3, 2, 7, 6, 0, -1, 4, 8, 9, 10, 11, 12};
    std::vector<int> expected{-1, 0, 1, 2, 3, 4, 5, 7 ,8, 9, 10, 11, 12};
    auto t = RBTree<int>(v);
    t.erase(6);
    ASSERT_EQ(expected, t.inorder_keys());
}


TEST(RBTree, EraseFullTree1) {
    std::vector<int> v{5, 1, 3, 2, 7, 6, 0, -1, 4, 8, 9, 10, 11, 12};
    std::vector<int> expected{-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    auto t = RBTree<int>(v);
    for (size_t i = 0; i < v.size(); ++i) {
        t.erase(v[i]);
        expected.erase(std::find(expected.begin(), expected.end(), v[i]));
        ASSERT_EQ(expected, t.inorder_keys())
            << "on element " << v[i];
    }
}

TEST(RBTree, EraseFullTree2) {
    std::vector<int> expected(100);
    std::iota(expected.begin(), expected.end(), 0);
    std::vector<int> copy = expected;
    auto t = RBTree<int>(expected);
    for (size_t i = 0; i < 100; ++i) {
        t.erase(copy[i]);
        expected.erase(std::find(expected.begin(), expected.end(), copy[i]));
        ASSERT_EQ(expected, t.inorder_keys())
            << "on element " << copy[i];
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
