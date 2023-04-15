#include <cmath>

struct Point {
    float x = std::nanf("x");
    float y = std::nanf("y");
    float z = std::nanf("z");

    bool valid() const;
    bool equal() const;
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

};

struct Triangle {
    Point v1;
    Point v2;
    Point v3;

    Plane get_plane();
    bool valid() const;
    bool equal() const;
};

float calc_distance(Plane plane, Point point) {

}
bool triangle_intersection(Triangle t1, Triangle t2) {

}