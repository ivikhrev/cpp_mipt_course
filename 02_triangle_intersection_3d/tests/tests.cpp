#include "geom_structures.hpp"

#include <utils/test_utils.hpp>

#include <gtest/gtest.h>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <vector>

namespace fs = std::filesystem;

namespace {
    const auto PI = std::acos(-1);
}

TEST(Vec3, CanConstruct) {
    ASSERT_NO_THROW((Vec3{0, 0, 0}));
}

TEST(Vec3, DefaultConstruct) {
    ASSERT_NO_THROW((Vec3{}));
}

TEST(Vec3, DefaultConstructNotValid) {
    EXPECT_FALSE(Vec3{}.valid());
}

TEST(Vec3, ValidVec3IsValid) {
    EXPECT_TRUE(Vec3(0, 0, 0).valid());
}

TEST(Vec3, EqualPointsIsEqual) {
    Vec3 p1{0, 0, 0};
    Vec3 p2{0, 0, 0};

    ASSERT_EQ(p1, p2);
}

TEST(Vec3, NotEqualPointsIsNotEqual) {
    Vec3 p1{0, 0, 0};
    Vec3 p2{1, 0, 0};

    EXPECT_NE(p1, p2);
}

TEST(Vec3, AddZeroDidNothing) {
    Vec3 p1{1, 1, 1};
    Vec3 p2{0, 0, 0};
    ASSERT_EQ(p1 + p2, p1);
}

TEST(Vec3, SimpleAddIsCorrect) {
    Vec3 p1{1, 1, 1};
    Vec3 p2{1, 2, 3};
    EXPECT_EQ(p1 + p2, Vec3(2,3,4));
}

TEST(Vec3, SubtractZero) {
    Vec3 p1{1, 1, 1};
    Vec3 p2{0, 0, 0};
    EXPECT_EQ(p1 - p2, p1);
}

TEST(Vec3, Subtract) {
    Vec3 p1{1, 1, 1};
    Vec3 p2{1, 1, 1};
    EXPECT_EQ(p1 - p2, Vec3(0, 0, 0));
}

TEST(Vec3, NormalizedVecIsNormalized) {
    Vec3 p1{1, 0, 0};
    EXPECT_TRUE(p1.is_normalized());
    Vec3 p2{0, 1, 0};
    EXPECT_TRUE(p2.is_normalized());
    Vec3 p3{0, 0, 1};
    EXPECT_TRUE(p3.is_normalized());
}

TEST(Vec3, NotNormalizedVecIsNotNormalized) {
    Vec3 p1{1, 1, 1};
    EXPECT_FALSE(p1.is_normalized());
}

TEST(Vec3, CanNormalizeVector) {
    Vec3 p1{1, 1, 1};
    EXPECT_FALSE(p1.is_normalized());
    p1 = p1.normalize();
    EXPECT_EQ(p1, Vec3(0.57735026919f, 0.57735026919f, 0.57735026919f));
    EXPECT_TRUE(p1.is_normalized());
}

TEST(CrossProduct, CrossProduct1) {
    Vec3 p1(5, 6, 2);
    Vec3 p2(1, 1, 1);

    EXPECT_EQ(cross_product(p1, p2), Vec3(4, -3, -1));
}

TEST(CrossProduct, CrossProduct2) {
    Vec3 p1(3, 0, -3);
    Vec3 p2(3, 3, 3);

    EXPECT_EQ(cross_product(p1, p2), Vec3(9, -18, 9));
}

TEST(DotProduct, DotProduct1) {
    Vec3 p1(1, 0, 0);
    Vec3 p2(1, 0, 0);

    EXPECT_FLOAT_EQ(dot_product(p1, p2), 1);
}

TEST(DotProduct, DotProduct2) {
    Vec3 p1(1, 1, 1);
    Vec3 p2(1, 2, 3);

    EXPECT_FLOAT_EQ(dot_product(p1, p2), 6);
}

TEST(Plane, Plane) {
    Vec3 v1(1, -2, 1);
    Vec3 v2(4, -2, -2);
    Vec3 v3(4, 1, 4);

    EXPECT_EQ(Plane(v1, v2, v3), Plane(9, -18, 9, -54));
}

TEST(Plane, SubstitutePoint) {
    Plane plane = Plane(9, -18, 9, -54);
    Vec3 point(1, -2, 1);

    EXPECT_FLOAT_EQ(plane(point), 0.0f);
}

TEST(Plane, CheckPointsBelongToPlane) {
    // Plane plane = Plane(9, -18, 9, -54);
    Plane plane = Plane(1, -2, 1, -6);
    Vec3 point1(1, -2, 1);
    Vec3 point2(1, 1, 1);
    EXPECT_TRUE(point_belong_to_plane(plane, point1));
    EXPECT_FALSE(point_belong_to_plane(plane, point2));
}

TEST(Plane, CheckPointBelongToLine) {
    // Plane plane = Plane(9, -18, 9, -54);
    Line l(Plane(1, 0, 0, 0), Plane(0, 1, 0, 0));
    EXPECT_TRUE(point_belong_to_line(l, Vec3(0, 0, 0)));
    EXPECT_TRUE(point_belong_to_line(l, Vec3(0, 0, 1)));
    EXPECT_TRUE(point_belong_to_line(l, Vec3(0, 0, -1)));
    EXPECT_TRUE(point_belong_to_line(l, Vec3(0, 0, 100)));
    EXPECT_FALSE(point_belong_to_line(l, Vec3(1, 0, 0)));
    EXPECT_FALSE(point_belong_to_line(l, Vec3(0, 1, 0)));
    EXPECT_FALSE(point_belong_to_line(l, Vec3(1, 1, 1)));
}

TEST(SignedDistance, PointToPlain1) {
    // Plane plane = Plane(9, -18, 9, -54);
    Plane plane = Plane(1, 0, 0, 0);
    Vec3 point(0.5f, 0, 0);
    EXPECT_FLOAT_EQ(calc_signed_distance(plane, point), 0.5f);
}

TEST(SignedDistance, PointToPlain2) {
    // Plane plane = Plane(9, -18, 9, -54);
    Plane plane = Plane(1, 0, 0, 0);
    Vec3 point(-0.5f, 0, 0);
    EXPECT_FLOAT_EQ(calc_signed_distance(plane, point), -0.5f);
}

TEST(SignedDistance, PointToPlain3) {
    // Plane plane = Plane(9, -18, 9, -54);
    Plane plane = Plane(5, 1, -1, -1);
    Vec3 point(0.5f, 0, 0);
    EXPECT_FLOAT_EQ(calc_signed_distance(plane, point), sqrtf(3) / 6);
}

TEST(Distance, DifferentPoints) {
    Vec3 point1(1, 0, 0);
    Vec3 point2(0, 1, 0);
    EXPECT_FLOAT_EQ(calc_distance(point1, point2), sqrtf(2));
}

TEST(Distance, SamePoint) {
    Vec3 point1(1, 0, 0);
    Vec3 point2(1, 0, 0);
    EXPECT_FLOAT_EQ(calc_distance(point1, point2), 0);
}

TEST(Triangle, NotDegenerateTriangle) {
    Vec3 point1(1, 0, 0);
    Vec3 point2(0, 1, 0);
    Vec3 point3(0, 0, 1);
    Triangle t({point1, point2, point3});

    EXPECT_FALSE(t.degenerate());
}

TEST(Triangle, DegenerateTriangle) {
    Vec3 point1(1, 0, 0);
    Vec3 point2(2, 0, 0);
    Vec3 point3(3, 0, 0);
    Triangle t({point1, point2, point3});

    EXPECT_TRUE(t.degenerate());
}

TEST(Plane, TestParallelPlanes1) {
    EXPECT_TRUE(planes_are_parallel(Plane(1, 0, 0, 0), Plane(1, 0, 0, 0)));
}

TEST(Plane, TestParallelPlanes2) {
    EXPECT_TRUE(planes_are_parallel(Plane(1, 0, 0, 0), Plane(1, 0, 0, 1)));
}

TEST(Plane, TestNotParallelPlanes) {
    EXPECT_FALSE(planes_are_parallel(Plane(1, 0, 0, 0), Plane(0, 1, 0, 0)));
}

TEST(Angle, AngleBetweenPlanes) {
    EXPECT_FLOAT_EQ(calc_angle(Plane(1, 0, 0, 0), Plane(1, 0, 0, 0)), 0);
    EXPECT_FLOAT_EQ(calc_angle(Plane(1, 1, 1, 0), Plane(1, 1, 1, 0)), 0);
    EXPECT_FLOAT_EQ(calc_angle(Plane(1, 0, 0, 0), Plane(0, 1, 0, 0)), PI / 2);
    EXPECT_FLOAT_EQ(calc_angle(Plane(1, 0, 0, 0), Plane(0, 0, 1, 0)), PI / 2);
    EXPECT_FLOAT_EQ(calc_angle(Plane(0, 1, 0, 0), Plane(0, 0, 1, 0)), PI / 2);

    EXPECT_FLOAT_EQ(calc_angle(Plane(2, -1, 1, 0), Plane(1, 0, 1, 0)),  PI / 6);
    EXPECT_FLOAT_EQ(calc_angle(Plane(2, 2, -3, -4), Plane(3, -3, 5, -3)),  0.98267967);

    EXPECT_FLOAT_EQ(calc_angle(Plane(1, 0, 0, 0), Plane(1, 1, 0.1, 0)),  0.78788573);
    EXPECT_FLOAT_EQ(calc_angle(Plane(0, 1, 0, 0), Plane(1, 1, 0.1, 0)),  0.78788573);
    EXPECT_FLOAT_EQ(calc_angle(Plane(0, 0, 1, 0), Plane(1, 1, 0.1, 0)),  1.5002031);
}

TEST(Projection, ProjectPointOntoPlane) {
    EXPECT_EQ(calc_projection(Plane(0, 0, 1, 0), Vec3(0, 0, 1)), Vec3(0, 0, 0));
    EXPECT_EQ(calc_projection(Plane(0, 0, 1, 0), Vec3(1, 0, 1)), Vec3(1, 0, 0));
    EXPECT_EQ(calc_projection(Plane(0, 0, 1, 0), Vec3(0, 1, 1)), Vec3(0, 1, 0));
    EXPECT_EQ(calc_projection(Plane(0, 0, 1, 0), Vec3(0, 0, 0)), Vec3(0, 0, 0));
    EXPECT_EQ(calc_projection(Plane(0, 0, 1, 1), Vec3(0, 0, 1)), Vec3(0, 0, 1));
    EXPECT_EQ(calc_projection(Plane(1, 1, 1, 1), Vec3(1, 1, 1)), Vec3(1 / 3.f, 1 / 3.f, 1 / 3.f));
}

// TEST(Projection, ProjectPointOntoLine) {
//     Line l1(Plane(1, 0, 0, 0), Plane(0, 1, 0, 0));
//     EXPECT_FLOAT_EQ(calc_projection_1d(l1, Vec3(1, 0, 1)), 1);
//     EXPECT_FLOAT_EQ(calc_projection_1d(l1, Vec3(1, 0, 2)), 2);
//     EXPECT_FLOAT_EQ(calc_projection_1d(l1, Vec3(1, 0, 3)), 3);

//     // not trivial case
//     Line l2(Plane(1, 1, 1, -1), Plane(1, 1, 0, 0));
//     EXPECT_FLOAT_EQ(calc_projection_1d(l2, Vec3(1, 4, 3)), 3);
// }

TEST(ConvertTo2d, ConvertPointTo2d) {
    EXPECT_EQ(convert_point_to_2d(Vec3(1, 2, 0), 2), Vec3(1, 2, 0));
    EXPECT_EQ(convert_point_to_2d(Vec3(1, 0, 2), 1), Vec3(1, 2, 0));
    EXPECT_EQ(convert_point_to_2d(Vec3(0, 1, 2), 0), Vec3(1, 2, 0));
}

TEST(ConvertTo2d, ConvertTriangletTo2d) {
    Triangle t1({{1, 0, 0}, {0, 1, 0}, {1, 1, 0}});
    EXPECT_EQ(convert_to_2d(t1, 2), t1);

    Triangle t2({{1, 0, 0}, {0, 0, 1}, {1, 0, 1}});
    EXPECT_EQ(convert_to_2d(t2, 1), t1);

    Triangle t3({{0, 1, 0}, {0, 0, 1}, {0, 1, 1}});
    EXPECT_EQ(convert_to_2d(t3, 0), t1);
}

TEST(TriangleIntersection2D, SameTriangle) {
    // same triangle
    EXPECT_TRUE(
        test_triangles_intersection_2d(
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 1, 0}}),
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 1, 0}})
        )
    );
}

TEST(TriangleIntersection2D, VertexVertexContact1) {
    EXPECT_TRUE(
        test_triangles_intersection_2d(
            Triangle({{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}}),
            Triangle({{0, 1, 0}, {-1, 2, 0}, {1, 2, 0}})
        )
    );
}

TEST(TriangleIntersection2D, VertexVertexContact2) {
    EXPECT_TRUE(
        test_triangles_intersection_2d(
            Triangle({{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}}),
            Triangle({{1, 0, 0}, {2, 1, 0}, {3, 0, 0}})
        )
    );
}

TEST(TriangleIntersection2D, EdgeEdgeContact) {
    EXPECT_TRUE(
        test_triangles_intersection_2d(
            Triangle({{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}}),
            Triangle({{1, 0, 0}, {0, 1, 0}, {2, 1, 0}})
        )
    );
}

TEST(TriangleIntersection2D, VertexEdgeContact) {
    EXPECT_TRUE(
        test_triangles_intersection_2d(
            Triangle({{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}}),
            Triangle({{0, 0, 0}, {1, -1, 0}, {-1, -1, 0}})
        )
    );
}

TEST(TriangleIntersection2D, PlainIntersection) {
    EXPECT_TRUE(
        test_triangles_intersection_2d(
            Triangle({{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}}),
            Triangle({{0, 0, 0}, {-1, 1, 0}, {1, 1, 0}})
        )
    );
}

TEST(TriangleIntersection2D, OneTriangleInsideAnother) {
    EXPECT_TRUE(
        test_triangles_intersection_2d(
            Triangle({{-3, 0, 0}, {3, 0, 0}, {0, 3, 0}}),
            Triangle({{-1, 1, 0}, {1, 1, 0}, {0, 2, 0}})
        )
    );
}

TEST(TriangleIntersection2D, NonintersectingTriangles) {
    EXPECT_FALSE(
        test_triangles_intersection_2d(
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 1, 0}}),
            Triangle({{2, 0, 0}, {4, 0, 0}, {3, 1, 0}})
        )
    );
}

TEST(TriangleIntersection3D, SameTriangle2d) {
    EXPECT_TRUE(
        test_triangles_intersection_3d(
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 1, 0}}),
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 1, 0}})
        )
    );
}

TEST(TriangleIntersection3D, VertexVertexContact1_2d) {
    EXPECT_TRUE(
        test_triangles_intersection_3d(
            Triangle({{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}}),
            Triangle({{0, 1, 0}, {-1, 2, 0}, {1, 2, 0}})
        )
    );
}

TEST(TriangleIntersection3D, VertexVertexContact2_2d) {
    EXPECT_TRUE(
        test_triangles_intersection_3d(
            Triangle({{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}}),
            Triangle({{1, 0, 0}, {2, 1, 0}, {3, 0, 0}})
        )
    );
}

TEST(TriangleIntersection3D, EdgeEdgeContact2d) {
    EXPECT_TRUE(
        test_triangles_intersection_3d(
            Triangle({{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}}),
            Triangle({{1, 0, 0}, {0, 1, 0}, {2, 1, 0}})
        )
    );
}

TEST(TriangleIntersection3D, VertexEdgeContact2d) {
    EXPECT_TRUE(
        test_triangles_intersection_3d(
            Triangle({{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}}),
            Triangle({{0, 0, 0}, {1, -1, 0}, {-1, -1, 0}})
        )
    );
}

TEST(TriangleIntersection3D, PlainIntersection2d) {
    EXPECT_TRUE(
        test_triangles_intersection_3d(
            Triangle({{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}}),
            Triangle({{0, 0, 0}, {-1, 1, 0}, {1, 1, 0}})
        )
    );
}

TEST(TriangleIntersection3D, OneTriangleInsideAnother2d) {
    EXPECT_TRUE(
        test_triangles_intersection_3d(
            Triangle({{-3, 0, 0}, {3, 0, 0}, {0, 3, 0}}),
            Triangle({{-1, 1, 0}, {1, 1, 0}, {0, 2, 0}})
        )
    );
}

TEST(TriangleIntersection3D, NonintersectingTriangles2d) {
    EXPECT_FALSE(
        test_triangles_intersection_3d(
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 1, 0}}),
            Triangle({{2, 0, 0}, {4, 0, 0}, {3, 1, 0}})
        )
    );
}

TEST(TriangleIntersection3D, NonintersectingTrianglesParallelPlanes3d) {
    EXPECT_FALSE(
        test_triangles_intersection_3d(
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 1, 0}}),
            Triangle({{-1, 0, 1}, {1, 0, 1}, {0, 1, 1}})
        )
    );
}

TEST(TriangleIntersection3D, IntersectingEdgeToEdge3d) {
    EXPECT_TRUE(
        test_triangles_intersection_3d(
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 0, 1}}),
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 1, 0}})
        )
    );
}

TEST(TriangleIntersection3D, IntersectingEdgeToVertex3d) {
    EXPECT_TRUE(
        test_triangles_intersection_3d(
            Triangle({{-1, 0, 1}, {1, 0, 1}, {0, 0, 0}}),
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 1, 0}})
        )
    );
}

TEST(TriangleIntersection3D, IntersectingVertexToVertex3d) {
    EXPECT_TRUE(
        test_triangles_intersection_3d(
            Triangle({{-1, 0, 1}, {1, 0, 1}, {0, 0, 0}}),
            Triangle({{0, 0, 0}, {1, 1, 0}, {-1, 1, 0}})
        )
    );
}

TEST(TriangleIntersection3D, IntersectingPerpendicularPlanes3d) {
    EXPECT_TRUE(
        test_triangles_intersection_3d(
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 0, 1}}),
            Triangle({{0, -1, 0}, {0, 1, 0}, {0, 0, 1}})
        )
    );
}

TEST(TriangleIntersection3D, IntersectingOneIntervalInsideAnoherPlanes3d) {
    EXPECT_TRUE(
        test_triangles_intersection_3d(
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 0, 3}}),
            Triangle({{-1, 1, 0}, {1, 1, 0}, {0, -1, 1}})
        )
    );
}

TEST(TriangleIntersection3D, IntersectingOverlappingIntervals3d_1) {
    EXPECT_TRUE(
        test_triangles_intersection_3d(
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 0, 3}}),
            Triangle({{-1, 1, 0}, {1, 1, 0}, {1, -1, 2}})
        )
    );
}

TEST(TriangleIntersection3D, IntersectingOverlappingIntervals3d_2) {
    EXPECT_TRUE(
        test_triangles_intersection_3d(
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 0, 3}}),
            Triangle({{-1, 1, 0}, {1, 1, 0}, {-1, -1, 2}})
        )
    );
}

TEST(TriangleIntersection3D, IntersectingOverlappingIntervalsCommonEnd3d_1) {
    EXPECT_TRUE(
        test_triangles_intersection_3d(
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 0, 3}}),
            Triangle({{-1, 1, 0}, {1, 1, 0}, {-1, -1, 0}})
        )
    );
}

TEST(TriangleIntersection3D, IntersectingOverlappingIntervalsCommonEnd3d_2) {
    EXPECT_TRUE(
        test_triangles_intersection_3d(
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 0, 3}}),
            Triangle({{-1, 1, 0}, {1, 1, 0}, {1, -1, 0}})
        )
    );
}

TEST(TriangleIntersection3D, NonitersectingTrianglesPerpPlanes3d) {
    EXPECT_FALSE(
        test_triangles_intersection_3d(
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 0, 3}}),
            Triangle({{-1, 1, 0}, {1, 1, 0}, {0, 0.5f, 0}})
        )
    );
}

TEST(TriangleIntersection3D, NonitersectingTrianglesOnesVerticesEntirelyOneSideOfThePlaneCase3d) {
    EXPECT_FALSE(
        test_triangles_intersection_3d(
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 0, 3}}),
            Triangle({{-1, 1, 0}, {1, 1, 0}, {0, 0.5f, 3}})
        )
    );
}

TEST(TriangleIntersection3D, NonitersectingTrianglesIntersectingPlanesVerticesOnBothSides3d) {
    EXPECT_FALSE(
        test_triangles_intersection_3d(
            Triangle({{-1, 0, 0}, {1, 0, 0}, {0, 0, 3}}),
            Triangle({{-1, 1, 0}, {1, 1, 0}, {0, -1, -1}})
        )
    );
}

class TriangleIntersections3DFixtureTests : public testing::TestWithParam<std::string> {
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
    std::vector<Triangle> read_input_data(const fs::path& file_path) {
        std::ifstream file(file_path);
        int n;
        int verts_num = 3, coords_num = 3;
        std::vector<Triangle> triangles;
        if (file.is_open()) {
            file >> n;
            for (int i = 0; i < n; ++i) {
                std::vector<Vec3> v;
                for (int j = 0; j < verts_num; ++j) {
                    float x, y, z;
                    file >> x >> y >> z;
                    v.emplace_back(x, y, z);
            }
            triangles.emplace_back(v);
        }
        }
        return triangles;
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

    int calc_intersections(const std::vector<Triangle>& triangles) {
        int intersections_num = 0;
        for (int i = 0; i < static_cast<int>(triangles.size()); ++i) {
            for (int j = i + 1; j < static_cast<int>(triangles.size()); ++j) {
                intersections_num += test_triangles_intersection_3d(triangles[i], triangles[j]);
            }
        }
        return intersections_num;
    }
};

TEST_P(TriangleIntersections3DFixtureTests, End2EndTest) {
    auto input_file = fs::path(GetParam());
    auto answer_file = input_file.parent_path() / "answers" / input_file.filename();
    auto triangles = read_input_data(input_file);

    ASSERT_EQ(read_answer_data(answer_file), calc_intersections(triangles));
}

INSTANTIATE_TEST_SUITE_P(TriangleIntersections,
                         TriangleIntersections3DFixtureTests,
                         ::testing::ValuesIn(get_files_in_dir(TriangleIntersections3DFixtureTests::data_directory()))
);

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
