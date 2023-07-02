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


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
