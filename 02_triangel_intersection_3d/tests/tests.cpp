#include "geom_structures.hpp"

#include <gtest/gtest.h>

TEST(Point, CanConstruct) {
    ASSERT_NO_THROW((Point{0.f, 0.f, 0.f}));
}

TEST(Point, Equal) {
    Point p1{0.f, 0.f, 0.f};
    Point p2{0.f, 0.f, 0.f};

    ASSERT_EQ(p1, p2);
}

TEST(Point, NotEqual) {
    Point p1{0.f, 0.f, 0.f};
    Point p2{1.f, 0.f, 0.f};

    ASSERT_NE(p1, p2);
}

TEST(Point, AddZero) {
    Point p1{1.f, 1.f, 1.f};
    Point p2{0.f, 0.f, 0.f};
    ASSERT_EQ(p1 + p2, p1);
}

TEST(Point, Add) {
    Point p1{1.f, 1.f, 1.f};
    Point p2{1.f, 2.f, 3.f};
    ASSERT_EQ(p1 + p2, Point(2.f,3.f,4.f));
}

TEST(Point, SubtractZero) {
    Point p1{1.f, 1.f, 1.f};
    Point p2{0.f, 0.f, 0.f};
    ASSERT_EQ(p1 - p2, p1);
}

TEST(Point, Subtract) {
    Point p1{1.f, 1.f, 1.f};
    Point p2{1.f, 1.f, 1.f};
    ASSERT_EQ(p1 - p2, Point(0.f, 0.f, 0.f));
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}