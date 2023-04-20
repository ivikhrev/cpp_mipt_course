#include "geom_structures.hpp"

#include <gtest/gtest.h>

TEST(Vec3, CanConstruct) {
    ASSERT_NO_THROW((Vec3{0.f, 0.f, 0.f}));
}

TEST(Vec3, DefaultConstruct) {
    ASSERT_NO_THROW((Vec3{}));
}

TEST(Vec3, DefaultConstructNotValid) {
    EXPECT_FALSE(Vec3{}.valid());
}

TEST(Vec3, ValidVec3IsValid) {
    EXPECT_TRUE(Vec3(0.f, 0.f, 0.f).valid());
}

TEST(Vec3, EqualPointsIsEqual) {
    Vec3 p1{0.f, 0.f, 0.f};
    Vec3 p2{0.f, 0.f, 0.f};

    ASSERT_EQ(p1, p2);
}

TEST(Vec3, NotEqualPointsIsNotEqual) {
    Vec3 p1{0.f, 0.f, 0.f};
    Vec3 p2{1.f, 0.f, 0.f};

    EXPECT_NE(p1, p2);
}

TEST(Vec3, AddZeroDidNothing) {
    Vec3 p1{1.f, 1.f, 1.f};
    Vec3 p2{0.f, 0.f, 0.f};
    ASSERT_EQ(p1 + p2, p1);
}

TEST(Vec3, SimpleAddIsCorrect) {
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

TEST(Vec3, NormalizedVecIsNormalized) {
    Vec3 p1{1.f, 0.f, 0.f};
    EXPECT_TRUE(p1.normalized());
    Vec3 p2{0.f, 1.f, 0.f};
    EXPECT_TRUE(p2.normalized());
    Vec3 p3{0.f, 0.f, 1.f};
    EXPECT_TRUE(p3.normalized());
}

TEST(Vec3, NotNormalizedVecIsNotNormalized) {
    Vec3 p1{1.f, 1.f, 1.f};
    EXPECT_FALSE(p1.normalized());
}

TEST(Vec3, CanNormalizeVector) {
    Vec3 p1{1.f, 1.f, 1.f};
    EXPECT_FALSE(p1.normalized());
    p1.normalize();
    EXPECT_EQ(p1, Vec3(0.57735026919f, 0.57735026919f, 0.57735026919f));
    EXPECT_TRUE(p1.normalized());
}

TEST(CrossProduct, CrossProduct1) {
    Vec3 p1(5.f, 6.f, 2.f);
    Vec3 p2(1.f, 1.f, 1.f);

    EXPECT_EQ(cross_product(p1, p2), Vec3(4.f, -3.f, -1.f));
}

TEST(CrossProduct, CrossProduct2) {
    Vec3 p1(3.f, 0.f, -3.f);
    Vec3 p2(3.f, 3.f, 3.f);

    EXPECT_EQ(cross_product(p1, p2), Vec3(9.f, -18.f, 9.f));
}

TEST(DotProduct, DotProduct1) {
    Vec3 p1(1.f, 0.f, 0.f);
    Vec3 p2(1.f, 0.f, 0.f);

    EXPECT_FLOAT_EQ(dot_product(p1, p2), 1.f);
}

TEST(DotProduct, DotProduct2) {
    Vec3 p1(1.f, 1.f, 1.f);
    Vec3 p2(1.f, 2.f, 3.f);

    EXPECT_FLOAT_EQ(dot_product(p1, p2), 6.f);
}

TEST(Plane, Plane) {
    Vec3 v1(1.f, -2.f, 1.f);
    Vec3 v2(4.f, -2.f, -2.f);
    Vec3 v3(4.f, 1.f, 4.f);

    EXPECT_EQ(Plane(v1, v2, v3), Plane(9, -18, 9, -54));
}

TEST(Plane, SubstitutePoint) {
    Plane plane = Plane(9, -18, 9, -54);
    Vec3 point(1.f, -2.f, 1.f);

    EXPECT_FLOAT_EQ(plane(point), 0.0f);
}

TEST(Plane, CheckPointsBelongToPlane) {
    // Plane plane = Plane(9, -18, 9, -54);
    Plane plane = Plane(1, -2, 1, -6);
    Vec3 point1(1.f, -2.f, 1.f);
    Vec3 point2(1.f, 1.f, 1.f);
    EXPECT_TRUE(point_belong_to_plane(plane, point1));
    EXPECT_FALSE(point_belong_to_plane(plane, point2));
}


TEST(SignedDistance, CalculateSignedDistanceFromPointToPlain1) {
    // Plane plane = Plane(9, -18, 9, -54);
    Plane plane = Plane(1, 0, 0, 0);
    Vec3 point(0.5f, 0.f, 0.f);
    EXPECT_FLOAT_EQ(calc_signed_distance(plane, point), 0.5f);
}

TEST(SignedDistance, CalculateSignedDistanceFromPointToPlain2) {
    // Plane plane = Plane(9, -18, 9, -54);
    Plane plane = Plane(1, 0, 0, 0);
    Vec3 point(-0.5f, 0.f, 0.f);
    EXPECT_FLOAT_EQ(calc_signed_distance(plane, point), -0.5f);
}

TEST(SignedDistance, CalculateSignedDistanceFromPointToPlain3) {
    // Plane plane = Plane(9, -18, 9, -54);
    Plane plane = Plane(5, 1, -1, -1);
    Vec3 point(0.5f, 0.f, 0.f);
    EXPECT_FLOAT_EQ(calc_signed_distance(plane, point), sqrtf(3) / 6);
}

TEST(Distance, CalculateDistanceBetweenPoints) {
    Vec3 point1(1.f, 0.f, 0.f);
    Vec3 point2(0.f, 1.f, 0.f);
    EXPECT_FLOAT_EQ(calc_distance(point1, point2), sqrtf(2));
}

TEST(Distance, CalculateDistanceBetweenPointsSamePoint) {
    Vec3 point1(1.f, 0.f, 0.f);
    Vec3 point2(1.f, 0.f, 0.f);
    EXPECT_FLOAT_EQ(calc_distance(point1, point2), 0.f);
}

TEST(Triangle, NotDegenerateTriangle) {
    Vec3 point1(1.f, 0.f, 0.f);
    Vec3 point2(0.f, 1.f, 0.f);
    Vec3 point3(0.f, 0.f, 1.f);
    Triangle t(point1, point2, point3);

    EXPECT_FALSE(t.degenerate());
}

TEST(Triangle, DegenerateTriangle) {
    Vec3 point1(1.f, 0.f, 0.f);
    Vec3 point2(2.f, 0.f, 0.f);
    Vec3 point3(3.f, 0.f, 0.f);
    Triangle t(point1, point2, point3);

    EXPECT_TRUE(t.degenerate());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
