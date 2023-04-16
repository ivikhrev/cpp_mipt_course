#include <cmath>

struct Point {
    float x = std::nanf("x");
    float y = std::nanf("y");
    float z = std::nanf("z");

    static constexpr float epsilon = 1e-6;

    Point() = default;
    Point(float x, float y, float z) : x(x), y(y), z(z) {}

    bool valid() const;
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;

    Point operator+(const Point& other) const ;
    Point& operator+=(const Point& other);
    Point operator-(const Point& other) const;
    Point& operator-=(const Point& other);
};

struct Line {
    Point p;
    Point d;
};

struct Plane {
    float a = 0.0f;
    float b = 0.0f;
    float c = 0.0f;
    float d = 0.0f;

    Plane(Point p1, Point p2, Point p3);
};

struct Triangle {
    Point v1;
    Point v2;
    Point v3;

    Plane get_plane();
    bool valid() const;
    bool equal() const;
};

Point cross_product(Point v1, Point v2);

float calc_distance(Plane plane, Point point);

bool triangle_intersection(Triangle t1, Triangle t2);