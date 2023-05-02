#include "lru.hpp"
#include "lfu.hpp"

#include <utils/tests_utils.hpp>

#include <gtest/gtest.h>

#include <fstream>
#include <filesystem>
#include <vector>
#include <utility>

namespace fs = std::filesystem;

class CacheFixtureTests : public testing::TestWithParam<std::string> {
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
    std::pair<int, std::vector<int>> read_input_data(const fs::path& file_path) {
        std::ifstream file(file_path);
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

    int read_answer_data(const fs::path& file_path) {
        std::ifstream file(file_path);
        int answer;
        if (file.is_open()) {
            file >> answer;
        } else {
            throw std::runtime_error("Can't open file" + file_path.string());
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

TEST_P(CacheFixtureTests, LRUEnd2EndTest) {
    auto input_file = fs::path(GetParam());
    auto answer_file = input_file.parent_path() / "answers_lru" / input_file.filename();
    auto [capacity, input] = read_input_data(input_file);
    ASSERT_EQ(read_answer_data(answer_file), calc_hits_number<LRUCache>(capacity, input));
}

INSTANTIATE_TEST_SUITE_P(LRUCache,
                         CacheFixtureTests,
                         ::testing::ValuesIn(get_files_in_dir(CacheFixtureTests::data_directory()))
);

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}