#include "geom_structures.hpp"

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

bool Vec3::is_normalized() const {
    return fabsf(len() - 1.f) < numeric_utils::epsilon;
}

void Vec3::normalize() {
    float l = len();
    x /= l;
    y /= l;
    z /= l;
}

Vec3 cross_product(const Vec3& v1, const Vec3& v2) {
    return Vec3{
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
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
    return fabsf(a - other.a) < numeric_utils::epsilon &&
        fabsf(b - other.b) < numeric_utils::epsilon &&
        fabsf(c - other.c) < numeric_utils::epsilon &&
        fabsf(d - other.d) < numeric_utils::epsilon;
}

Vec3 Plane::get_normal() const {
    return {a, b, c};
}

float Plane::operator() (const Vec3& p) const {
    return a * p.x + b * p.y + c * p.z + d;
}

bool point_belong_to_plane(const Plane& plane, const Vec3& p) {
    return fabsf(plane(p)) < numeric_utils::epsilon;
}

float calc_signed_distance(const Plane& plane, const Vec3& point) {
    Vec3 plane_normal = plane.get_normal();
    return plane(point) / plane_normal.len();
}

bool triangle_intersection(const Triangle& t1, const Triangle& t2) {

}