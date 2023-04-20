#include "geom_structures.hpp"

#include <algorithm>
#include <cmath>

Vec3 Vec3::operator+(const Vec3& other) const {
    return {x + other.x, y + other.y, z + other.z};
}

Vec3& Vec3::operator+=(const Vec3& other) {
    *this = *this + other;
    return *this;
}

Vec3 Vec3::operator-(const Vec3& other) const {
    return {x - other.x, y - other.y, z - other.z};
}

Vec3& Vec3::operator-=(const Vec3& other) {
    *this = *this - other;
    return *this;
}

bool Vec3::operator==(const Vec3& other) const {
    return fabsf(x - other.x) < numeric_utils::epsilon &&
        fabsf(y - other.y) < numeric_utils::epsilon &&
        fabsf(z - other.z) < numeric_utils::epsilon;
}

bool Vec3::operator!=(const Vec3& other) const {
    return !(*this == other);
}

bool Vec3::valid() const {
    return !std::isnan(x) && !std::isnan(y) && !std::isnan(z);
}

float Vec3::len() const {
    return sqrtf(x * x + y * y + z * z);
}

bool Vec3::normalized() const {
    return fabsf(len() - 1.f) < numeric_utils::epsilon;
}

void Vec3::normalize() {
    float l = len();
    x /= l;
    y /= l;
    z /= l;
}

Plane::Plane(const Vec3& p1, const Vec3& p2, const Vec3& p3) {
    Vec3 first = p2 - p1;
    Vec3 second = p3 - p1;
    Vec3 product = cross_product(first, second);
    a = product.x;
    b = product.y;
    c = product.z;
    d  = -a * p1.x - b * p1.y - c * p1.z;
}

bool Plane::operator==(const Plane& other) const {
    Vec3 n1 = normal();
    Vec3 n2 = other.normal();
    return cross_product(n1, n2) == Vec3(0.f, 0.f, 0.f) &&
        fabsf(d - other.d) < numeric_utils::epsilon;
}

bool Plane::operator!=(const Plane& other) const {
    return !(*this == other);
}

Vec3 Plane::normal() const {
    return {a, b, c};
}

float Plane::operator() (const Vec3& p) const {
    return a * p.x + b * p.y + c * p.z + d;
}

Triangle::Triangle(const Vec3& p1, const Vec3& p2, const Vec3& p3) : p1(p1), p2(p2), p3(p3) {}

Plane Triangle::get_plane() const {
    return Plane(p1, p2, p3);
}

bool Triangle::valid() const {
    return p1.valid() && p2.valid() && p3.valid();
}

bool Triangle::degenerate() const {
    float distance12 = calc_distance(p1, p2);
    float distance23 = calc_distance(p2, p3);
    float distance13 = calc_distance(p1, p3);
    float max_distance = std::max({distance12, distance13, distance23});
    float two_sum = distance12 + distance13 + distance23 - max_distance;
    return fabsf(max_distance - two_sum) < numeric_utils::epsilon;
}

float dot_product(const Vec3& v1, const Vec3& v2) {
    return  v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3 cross_product(const Vec3& v1, const Vec3& v2) {
    return Vec3{
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

bool point_belong_to_plane(const Plane& plane, const Vec3& p) {
    return fabsf(plane(p)) < numeric_utils::epsilon;
}

bool point_belong_to_line(const Line& line, const Vec3& p) {
    return point_belong_to_plane(line.plane1, p) && point_belong_to_plane(line.plane2, p);
}

float calc_signed_distance(const Plane& plane, const Vec3& point) {
    Vec3 plane_normal = plane.normal();
    return plane(point) / plane_normal.len();
}

float calc_distance(const Vec3& point1, const Vec3& point2) {
    return (point2 - point1).len();
}

bool triangle_intersection_2d(const Triangle& t1, const Triangle& t2) {
    return false;
}

bool planes_are_parallel(const Plane& plane1, const Plane& plane2){
    return cross_product(plane1.normal(), plane2.normal()) == Vec3(0.f, 0.f, 0.f);
}

bool triangle_intersection_3d(const Triangle& t1, const Triangle& t2) {
    if (t1.degenerate() || t2.degenerate()) {
        return false;
    }

    Plane plane1 = t1.get_plane();
    float d1 = calc_signed_distance(plane1, t2.p1);
    float d2 = calc_signed_distance(plane1, t2.p2);
    float d3 = calc_signed_distance(plane1, t2.p3);
    if ((d1 < 0 && d2 < 0 && d3 < 0) ||
        (d1 > 0 && d2 > 0 && d3 > 0)) {
        return false;
    }

    Plane plane2 = t2.get_plane();
    // same planes
    if (plane1 == plane2) {
        //compute 2d case
        return triangle_intersection_2d(t1, t2);
    }
    else if (planes_are_parallel(plane1, plane2)) {
        return false; // planes are parallel and not the same
    }

    d1 = calc_signed_distance(plane2, t1.p1);
    d2 = calc_signed_distance(plane2, t1.p2);
    d3 = calc_signed_distance(plane2, t1.p3);
    if ((d1 < 0 && d2 < 0 && d3 < 0) ||
        (d1 > 0 && d2 > 0 && d3 > 0)) {
        return false;
    }

    Line intersection_line{plane1, plane2};

    return false;
}
