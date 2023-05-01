#include "lru.hpp"
#include "lfu.hpp"

#include <gtest/gtest.h>

#include <fstream>
#include <filesystem>
#include <vector>
#include <utility>

namespace fs = std::filesystem;
using str_pair = std::pair<std::string, std::string>;

class BaseFixtureTests : public testing::TestWithParam<std::pair<std::string, std::string>> {
protected:
    const std::string& data_directory() {
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


    std::pair<int, std::vector<int>> read_input_data(const std::string& file_path_str) {
        const fs::path file_path = fs::path(data_directory()) / file_path_str;
        std::ifstream file(file_path);
        std::cout << "Current path is " << file_path << '\n';
        int cache_capacity, elements_number;
        std::vector<int> data;
        if (file.is_open()) {
            file >> cache_capacity;
            file >> elements_number;
            for (int i; file >> i;) {
                data.push_back(i);
                // if (file.peek() == ' ') {
                //     file.ignore();
                // } ??
            }
        }
        return {cache_capacity, data};
    }

    int read_answer_data(const std::string& file_path_str) {
        const fs::path file_path = fs::path(data_directory()) / file_path_str;
        std::ifstream file(file_path);
        int answer;
        if (file.is_open()) {
            file >> answer;
        }

        return answer;
    }

    template<class Cache>
    int calc_hits_number(int capacity, const std::vector<int>& input) {
        Cache cache(capacity);
        int hits = 0;
        for (auto k : input) {
            if (cache.get(k) != -1) {
                ++hits;
            } else {
                cache.put(k, k);
            }
        }
        return hits;
    }
};

TEST_P(BaseFixtureTests, End2EndTest) {
    auto [input_file, answer_file] = GetParam();
    auto [capacity, input] = read_input_data(input_file);
    ASSERT_EQ(read_answer_data(answer_file), calc_hits_number<LRUCache>(capacity, input));
}

INSTANTIATE_TEST_SUITE_P(LRUCache,
                         BaseFixtureTests,
                         ::testing::Values(str_pair{"0.txt", "answers/0_answer.txt"},
                         str_pair{"1.txt", "answers/1_answer.txt"},
                         str_pair{"2.txt", "answers/2_answer.txt"},
                         str_pair{"3.txt", "answers/3_answer.txt"},
                         str_pair{"4.txt", "answers/4_answer.txt"},
                         str_pair{"5.txt", "answers/5_answer.txt"},
                         str_pair{"6.txt", "answers/6_answer.txt"},
                         str_pair{"7.txt", "answers/7_answer.txt"},
                         str_pair{"8.txt", "answers/8_answer.txt"},
                         str_pair{"9.txt", "answers/9_answer.txt"})
);

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}