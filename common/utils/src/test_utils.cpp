#include "utils/test_utils.hpp"

#include <string>
#include <vector>
#include <set>

std::vector<std::string> get_files_in_dir(const std::string& dir) {
    std::set<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        if (entry.is_regular_file()) {
            files.insert(entry.path());
        }
    }

    return std::vector<std::string>{files.begin(), files.end()};
}