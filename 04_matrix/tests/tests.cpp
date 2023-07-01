#include <utils/test_utils.hpp>

#include <gtest/gtest.h>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <vector>

namespace fs = std::filesystem;


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
