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
    return fabsf(x - other.x) < epsilon &&
        fabsf(y - other.y) < epsilon &&
        fabsf(z - other.z) < epsilon;
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
    return len() < epsilon;
}

void Vec3::normilize() {
    float l = len();
    x /= l;
    y /= l;
    z /= l;
}

Vec3 cross_product(Vec3 v1, Vec3 v2) {
    return Vec3{
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

Plane::Plane(Vec3 p1, Vec3 p2, Vec3 p3) {
    Vec3 first = p2 - p1;
    Vec3 second = p3 - p1;
    Vec3 product = cross_product(first, second);
    a = product.x;
    b = product.y;
    c = product.z;
    d  = -a * p1.x - b * p1.y - c * p1.z;
}
