#include "geom_structures.hpp"

#include <gtest/gtest.h>

TEST(Vec3, CanConstruct) {
    ASSERT_NO_THROW((Vec3{0.f, 0.f, 0.f}));
}

TEST(Vec3, DefaultConstruct) {
    ASSERT_NO_THROW((Vec3{}));
}

TEST(Vec3, DefaultConstructNotValid) {
    EXPECT_EQ(Vec3{}.valid(), false);
}

TEST(Vec3, ValidVec3IsValid) {
    EXPECT_EQ(Vec3(0.f, 0.f, 0.f).valid(), true);
}

TEST(Vec3, Equal) {
    Vec3 p1{0.f, 0.f, 0.f};
    Vec3 p2{0.f, 0.f, 0.f};

    ASSERT_EQ(p1, p2);
}

TEST(Vec3, NotEqual) {
    Vec3 p1{0.f, 0.f, 0.f};
    Vec3 p2{1.f, 0.f, 0.f};

    EXPECT_NE(p1, p2);
}

TEST(Vec3, AddZero) {
    Vec3 p1{1.f, 1.f, 1.f};
    Vec3 p2{0.f, 0.f, 0.f};
    ASSERT_EQ(p1 + p2, p1);
}

TEST(Vec3, Add) {
    Vec3 p1{1.f, 1.f, 1.f};
    Vec3 p2{1.f, 2.f, 3.f};
    EXPECT_EQ(p1 + p2, Vec3(2.f,3.f,4.f));
}

TEST(Vec3, SubtractZero) {
    Vec3 p1{1.f, 1.f, 1.f};
    Vec3 p2{0.f, 0.f, 0.f};
    EXPECT_EQ(p1 - p2, p1);
}

TEST(Vec3, Subtract) {
    Vec3 p1{1.f, 1.f, 1.f};
    Vec3 p2{1.f, 1.f, 1.f};
    EXPECT_EQ(p1 - p2, Vec3(0.f, 0.f, 0.f));
}

TEST(CrossProduct, CrossProduct) {
    Vec3 p1(5.f, 6.f, 2.f);
    Vec3 p2(1.f, 1.f, 1.f);

    EXPECT_EQ(cross_product(p1, p2), Vec3(4.f, -3.f, -1.f));
}

TEST(CrossProduct, CrossProduct2) {
    Vec3 p1(3.f, 0.f, -3.f);
    Vec3 p2(3.f, 3.f, 3.f);

    EXPECT_EQ(cross_product(p1, p2), Vec3(9.f, -18.f, 9.f));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
