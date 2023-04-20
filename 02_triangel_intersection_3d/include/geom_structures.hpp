#include <cmath>
#include <iostream>

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
    bool normalized() const;

    bool operator==(const Vec3& other) const;
    bool operator!=(const Vec3& other) const;

    Vec3 operator+(const Vec3& other) const ;
    Vec3& operator+=(const Vec3& other);
    Vec3 operator-(const Vec3& other) const;
    Vec3& operator-=(const Vec3& other);

    float len() const;
    void normalize();

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
    Plane plane1;
    Plane plane2;
};

struct Triangle {
    Vec3 p1;
    Vec3 p2;
    Vec3 p3;

    Triangle(const Vec3& p1, const Vec3& p2, const Vec3& p3);

    Plane get_plane() const;
    bool valid() const;
    bool degenerate() const;
};

bool point_belong_to_plane(const Plane& plane, const Vec3& p);

bool point_belong_to_line(const Plane& plane, const Vec3& p);

float dot_product(const Vec3& v1, const Vec3& v2);

Vec3 cross_product(const Vec3& v1, const Vec3& v2);

float calc_signed_distance(const Plane& plane, const Vec3& point);

float calc_distance(const Vec3& point1, const Vec3& point2);

bool planes_are_parallel(const Plane& plane1, const Plane& plane2);

bool triangle_intersection_3d(const Triangle& t1, const Triangle& t2);
