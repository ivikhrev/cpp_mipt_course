#include <matrix.hpp>

#include <utils/test_utils.hpp>

#include <gtest/gtest.h>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <vector>

namespace fs = std::filesystem;

TEST(Matrix, ConstructWithParameters) {
    ASSERT_NO_THROW((Matrix<int>{2, 2, 0}));
}

TEST(Matrix, ConstructFromVector) {
    std::vector v{4, -1};
    ASSERT_NO_THROW((Matrix<int>{2, 2, v.begin(), v.end()}));
}

TEST(Matrix, HasExpectedSize) {
    Matrix<int> m{2, 2, 0};
    ASSERT_EQ(2, m.ncols());
    ASSERT_EQ(2, m.nrows());
}

TEST(Matrix, ConstructedFromVectorHasExpectedSize) {
    std::vector v{9, -1};
    Matrix<int> m{3, 3, v.begin(), v.end()};
    ASSERT_EQ(3, m.ncols());
    ASSERT_EQ(3, m.nrows());
}

TEST(Matrix, HasExpectedValues) {
    Matrix<int> m{2, 2, -1};
    ASSERT_EQ(-1, m[0][0]);
    ASSERT_EQ(-1, m[0][1]);
    ASSERT_EQ(-1, m[1][0]);
    ASSERT_EQ(-1, m[0][1]);
}

TEST(Matrix, ConstructedFromVectorHasExpectedValues) {
    std::vector<int> v = {1, 2, 3, 4};
    Matrix<int> m{2, 2, v.begin(), v.end()};
    ASSERT_EQ(1, m[0][0]);
    ASSERT_EQ(2, m[0][1]);
    ASSERT_EQ(3, m[1][0]);
    ASSERT_EQ(4, m[1][1]);
}

TEST(Matrix, CreateIdentityMatrix2x2) {
    Matrix I = Matrix<int>::eye(2, 2);
    ASSERT_EQ(1, I[0][0]);
    ASSERT_EQ(0, I[0][1]);
    ASSERT_EQ(0, I[1][0]);
    ASSERT_EQ(1, I[1][1]);
}

TEST(Matrix, CreateIdentityMatrix3x3) {
    Matrix I = Matrix<int>::eye(3, 3);
    ASSERT_EQ(1, I[0][0]);
    ASSERT_EQ(0, I[0][1]);
    ASSERT_EQ(0, I[0][2]);
    ASSERT_EQ(0, I[1][0]);
    ASSERT_EQ(1, I[1][1]);
    ASSERT_EQ(0, I[1][2]);
    ASSERT_EQ(0, I[2][0]);
    ASSERT_EQ(0, I[2][1]);
    ASSERT_EQ(1, I[2][2]);
}

TEST(Matrix, CopyMatrix2x2) {
    Matrix I = Matrix<int>::eye(2, 2);
    Matrix copy(I);
    ASSERT_EQ(I[0][0], copy[0][0]);
    ASSERT_EQ(I[0][1], copy[0][1]);
    ASSERT_EQ(I[1][0], copy[1][0]);
    ASSERT_EQ(I[1][1], copy[1][1]);
}

TEST(Matrix, CopyMatrix3x3) {
    Matrix I = Matrix<int>::eye(3, 3);
    Matrix copy(I);
    ASSERT_EQ(I[0][0], copy[0][0]);
    ASSERT_EQ(I[0][1], copy[0][1]);
    ASSERT_EQ(I[0][2], copy[0][2]);
    ASSERT_EQ(I[1][0], copy[1][0]);
    ASSERT_EQ(I[1][1], copy[1][1]);
    ASSERT_EQ(I[1][2], copy[1][2]);
    ASSERT_EQ(I[2][0], copy[2][0]);
    ASSERT_EQ(I[2][1], copy[2][1]);
    ASSERT_EQ(I[2][2], copy[2][2]);
}

TEST(Matrix, EqualMatrixAreEqual) {
    Matrix I1 = Matrix<int>::eye(3, 3);
    Matrix I2 = Matrix<int>::eye(3, 3);
    ASSERT_EQ(I1, I2);
}

TEST(Matrix, NotEqualMatrixArentEqual) {
    Matrix I = Matrix<int>::eye(3, 3);
    Matrix M(3, 3, 0);
    ASSERT_NE(I, M);
}

TEST(Matrix, NotEqualSizesMatrixArentEqual) {
    Matrix I = Matrix<int>::eye(3, 3);
    Matrix M(2, 2, 0);
    ASSERT_NE(I, M);
}

TEST(Matrix, MoveConstructor) {
    Matrix I = Matrix<int>::eye(3, 3);
    Matrix M(std::move(I));
    EXPECT_EQ(Matrix<int>::eye(3, 3), M);
}

TEST(Matrix, CopyAssignment) {
    Matrix I = Matrix<int>::eye(3, 3);
    Matrix M(1, 1, 0);
    M = I;
    EXPECT_EQ(I, M);
}

TEST(Matrix, MoveAssignment) {
    Matrix I = Matrix<int>::eye(3, 3);
    Matrix M(1, 1, 0);
    M = std::move(I);
    EXPECT_EQ(Matrix<int>::eye(3, 3), M);
}

TEST(Matrix, 2x2MatrixDet) {
    std::vector<int> v = {1, 2, 3, 4};
    Matrix<int> m{2, 2, v.begin(), v.end()};
    ASSERT_EQ(-2, m.det());
}

TEST(Matrix, 3x3MatrixDet) {
    std::vector<int> v = {1, 2, 3, 3, 1, 2, 2, 1, 3};
    Matrix<int> m{3, 3, v.begin(), v.end()};
    ASSERT_EQ(-6, m.det());
}

TEST(Matrix, 3x3MatrixDetWithZeroAtFirstDiagonalElement) {
    std::vector<int> v = {0, 2, 3, 3, 1, 2, 2, 1, 3};
    Matrix<int> m{3, 3, v.begin(), v.end()};
    ASSERT_EQ(-7, m.det());
}

TEST(Matrix, 3x3MatrixDetWithZeroAtSecondDiagonalElement) {
    std::vector<int> v = {1, 0, 3, 3, 0, 2, 2, 1, 3};
    Matrix<int> m{3, 3, v.begin(), v.end()};
    ASSERT_EQ(7, m.det());
}


template<class MatrixRepr>
class MatrixDeterminantTests : public testing::Test {
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
    auto read_input_data(const fs::path& file_path) {
        std::ifstream file(file_path);
        int n;
        std::vector<int> values;
        if (file.is_open()) {
            file >> n;
            values.resize(n * n);
            for (int i = 0; i < n * n; ++i) {
                file >> values[i];
            }
        }

        return MatrixRepr(n, n, values.begin(), values.end());
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

};

using MatrixReprs = testing::Types<Matrix<int, MatrixBuffer>, Matrix<int, MatrixJaggedBuffer>>;

TYPED_TEST_SUITE(MatrixDeterminantTests, MatrixReprs);

TYPED_TEST(MatrixDeterminantTests, End2EndTest) {
    for (auto file_str : get_files_in_dir(this->data_directory())) {
        auto input_file = fs::path(file_str);
        auto answer_file = input_file.parent_path() / "answers" / input_file.filename();
        auto matrix = this->read_input_data(input_file);
        ASSERT_EQ(this->read_answer_data(answer_file), matrix.det())
            << "on input data: " << input_file << '\n'
            << "answer file: " << answer_file << '\n';
    }
}

// INSTANTIATE_TEST_SUITE_P(Matrix,
//                          MatrixDeterminantTests,
//                          ::testing::ValuesIn(get_files_in_dir(MatrixDeterminantTests::data_directory()))
// );

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
