#include <cmath>
#include <iostream>
#include <vector>
namespace numeric_utils {
    constexpr float epsilon = 1e-6;
}

struct Vec3 {
    float x = std::nanf("x");
    float y = std::nanf("y");
    float z = std::nanf("z");

    Vec3() = default;
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    bool valid() const;
    bool is_normalized() const;

    bool operator==(const Vec3& other) const;
    bool operator!=(const Vec3& other) const;

    Vec3 operator+(const Vec3& other) const ;
    Vec3& operator+=(const Vec3& other);
    Vec3 operator-(const Vec3& other) const;
    Vec3& operator-=(const Vec3& other);
    Vec3 operator*(int f) const;
    Vec3 operator/(int d) const;

    float operator[](std::size_t idx) {
        if (idx == 0)
            return x;
        if (idx == 1)
            return y;
        return z;
    }

    const float operator[](std::size_t idx) const {
        if (idx == 0)
            return x;
        if (idx == 1)
            return y;
        return z;
    }

    float len() const;
    float len_squared() const;
    Vec3 normalize();
    int max_idx() const;

    friend std::ostream& operator<<(std::ostream& os, const Vec3& v) {
        return os << v.x << " " << v.y << " " << v.z;
    }
};

struct Plane {
    float a = 0.0f;
    float b = 0.0f;
    float c = 0.0f;
    float d = 0.0f;

    Plane(float a, float b, float c, float d) : a(a), b(b), c(c), d(d) {}
    Plane(const Vec3& p1, const Vec3& p2, const Vec3& p3);

    bool operator==(const Plane& other) const;
    bool operator!=(const Plane& other) const;

    Vec3 normal() const;
    float operator() (const Vec3& p) const;

    friend std::ostream& operator<<(std::ostream& os, const Plane& p) {
        return os << p.a << " " << p.b << " " << p.c << " " << p.d;
    }
};

struct Line {
    Vec3 point;
    Vec3 direction;

    Line() = default;
    Line(const Vec3& point, const Vec3& direction) : point(point), direction(direction) {}
    Line(const Plane& plane1, const Plane& plane2);

    // bool operator==(const Line& other) const;
    // bool operator!=(const Line& other) const;
};

struct Triangle {
    std::vector<Vec3> vertices;

    Triangle() = default;
    Triangle(const std::vector<Vec3>& v) : vertices(v) {};
    Triangle(std::initializer_list<Vec3> v) : vertices(v) {};
    Triangle(std::vector<Vec3>&& v) : vertices(v) {};

    bool operator==(const Triangle& other) const;
    bool operator!=(const Triangle& other) const;

    // void sort_vertices();
    Plane get_plane() const;
    bool valid() const;
    bool degenerate() const;
};

bool point_belong_to_plane(const Plane& plane, const Vec3& p);

bool point_belong_to_line(const Line& line, const Vec3& p);

float dot_product(const Vec3& v1, const Vec3& v2);

Vec3 cross_product(const Vec3& v1, const Vec3& v2);

float calc_signed_distance(const Plane& plane, const Vec3& point);

float calc_distance(const Vec3& point1, const Vec3& point2);

float calc_angle(const Plane& plane1, const Plane& plane2);

bool planes_are_parallel(const Plane& plane1, const Plane& plane2);

Vec3 calc_projection(const Plane& plane, const Vec3& point);
float calc_projection_1d(const Line& line, const Vec3& point);

bool test_triangles_intersection_2d(const Triangle& t1, const Triangle& t2);
bool test_triangles_intersection_3d(const Triangle& t1, const Triangle& t2);

Vec3 convert_point_to_2d(const Vec3& p, int zero_coordinate);
Triangle convert_to_2d(const Triangle& t, int zero_coordinate);