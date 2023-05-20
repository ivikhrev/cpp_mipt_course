// #include "node.hpp"
#include "avl_tree.hpp"

#include <utils/test_utils.hpp>

#include <gtest/gtest.h>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <vector>

namespace fs = std::filesystem;

// TEST(Node, CanConstructWithKeyParameter) {
//     ASSERT_NO_THROW((Node{0}));
// }

// TEST(Node, CanConstructWithAllParameters) {
//     Node<int> parent{0};
//     Node<int> left{1};
//     Node<int> right{2};
//     ASSERT_NO_THROW((Node<int>{0, &parent, &left, &right}));
// }

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
    ASSERT_EQ(expected, t.inorder_traversal());
}

TEST(AVLTree, LeftRotateRoot) {
    std::vector<int> v{1, 2, 3};
    std::vector<int> expected{1, 2, 3};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_traversal());
}

TEST(AVLTree, LeftRotate) {
    std::vector<int> v{2, 1, 3, 4, 5};
    std::vector<int> expected{1, 2, 3, 4, 5};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_traversal());
}

// TEST(AVLTree, LeftRotate2) {
//     std::vector<int> v{2, 5, 1, 3, 4};
//     std::vector<int> expected{1, 2, 3, 4, 5};
//     auto t = AVLTree<int>(v);
//     ASSERT_EQ(expected, t.inorder_traversal());
// }

TEST(AVLTree, RightRotateRoot) {
    std::vector<int> v{3, 2, 1};
    std::vector<int> expected{1, 2, 3};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_traversal());
}

TEST(AVLTree, RightRotate) {
    std::vector<int> v{4, 5, 3, 2, 1};
    std::vector<int> expected{1, 2, 3, 4, 5};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_traversal());
}

// TEST(AVLTree, RightRotate2) {
//     std::vector<int> v{4, 5, 3, 2, 1};
//     std::vector<int> expected{1, 2, 3, 4, 5};
//     auto t = AVLTree<int>(v);
//     ASSERT_EQ(expected, t.inorder_traversal());
// }

TEST(AVLTree, LeftRightRotate) {
    std::vector<int> v{3, 1, 2};
    std::vector<int> expected{1, 2, 3};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_traversal());
}

TEST(AVLTree, ComplexLeftRightRotate) {
    std::vector<int> v{5, 1, 3, 2, 4, 0};
    std::vector<int> expected{0, 1, 2, 3, 4, 5};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_traversal());
}

TEST(AVLTree, RightLeftRotate) {
    std::vector<int> v{1, 3 , 2};
    std::vector<int> expected{1, 2, 3};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_traversal());
}

TEST(AVLTree, ComplexRightLeftRotate) {
    std::vector<int> v{0, 4, 5, 2, 1, 3};
    std::vector<int> expected{0, 1, 2, 3, 4, 5};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_traversal());
}

TEST(AVLTree, TreeConstruct) {
    std::vector<int> v{5, 1, 7, 6, 9, 3};
    std::vector<int> expected{1, 3, 5, 6, 7 , 9};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_traversal());
}

TEST(AVLTree, KthMin) {
    std::vector<int> v{0, 1, 2, 3, 4, 5};
    std::vector<int> expected{0, 1, 2, 3, 4, 5};
    auto t = AVLTree<int>(v);
    ASSERT_EQ(expected, t.inorder_traversal());
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
    ASSERT_EQ(expected, t.inorder_traversal());
    ASSERT_EQ(0, t.less_count(0));
    ASSERT_EQ(1, t.less_count(1));
    ASSERT_EQ(2, t.less_count(2));
    ASSERT_EQ(3, t.less_count(3));
    ASSERT_EQ(4, t.less_count(4));
    ASSERT_EQ(5, t.less_count(5));
}


class SearchTreesFixtureTests : public testing::TestWithParam<std::string> {
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

TEST_P(SearchTreesFixtureTests, End2EndTest) {
    auto input_file = fs::path(GetParam());
    auto answer_file = input_file.parent_path() / "answers" / input_file.filename();
    auto input = read_input_data(input_file);

    ASSERT_EQ(read_answer_data(answer_file), calc_answer(input))
        << "on input data: " << input_file << '\n'
        << "answer file: " << answer_file << '\n';;
}

INSTANTIATE_TEST_SUITE_P(SearchTrees,
                         SearchTreesFixtureTests,
                         ::testing::ValuesIn(get_files_in_dir(SearchTreesFixtureTests::data_directory()))
);

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
