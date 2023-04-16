#include <cmath>

struct Vec3 {
    float x = std::nanf("x");
    float y = std::nanf("y");
    float z = std::nanf("z");

    static constexpr float epsilon = 1e-6;

    Vec3() = default;
    Vec3(float x, float y, float z) : x(x), y(y), z(z);

    bool valid() const;
    bool is_normalized() const;

    bool operator==(const Vec3& other) const;
    bool operator!=(const Vec3& other) const;

    Vec3 operator+(const Vec3& other) const ;
    Vec3& operator+=(const Vec3& other);
    Vec3 operator-(const Vec3& other) const;
    Vec3& operator-=(const Vec3& other);

    float len() const;
    void normilize();
};

struct Line {
    Vec3 p;
    Vec3 d;
};

struct Plane {
    float a = 0.0f;
    float b = 0.0f;
    float c = 0.0f;
    float d = 0.0f;

    Plane(Vec3 p1, Vec3 p2, Vec3 p3);
};

struct Triangle {
    Vec3 v1;
    Vec3 v2;
    Vec3 v3;

    Plane get_plane();
    bool valid() const;
};

Vec3 cross_product(Vec3 v1, Vec3 v2);

float calc_distance(Plane plane, Vec3 point);

bool triangle_intersection(Triangle t1, Triangle t2);
