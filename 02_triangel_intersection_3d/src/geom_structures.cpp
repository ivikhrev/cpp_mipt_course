#include "geom_structures.hpp"

#include <cmath>

Point Point::operator+(const Point& other) const {
    return {x + other.x, y + other.y, z + other.z};
}

Point& Point::operator+=(const Point& other) {
    *this = *this + other;
    return *this;
}

Point Point::operator-(const Point& other) const {
    return {x - other.x, y - other.y, z - other.z};
}

Point& Point::operator-=(const Point& other) {
    *this = *this - other;
    return *this;
}

bool Point::operator==(const Point& other) const {
    return fabsf(x - other.x) < epsilon &&
        fabsf(y - other.y) < epsilon &&
        fabsf(z - other.z) < epsilon;
}

bool Point::operator!=(const Point& other) const {
    return !(*this == other);
}

bool Point::valid() const {
    return !std::isnan(x) && !std::isnan(y) && !std::isnan(z);
}

Plane::Plane(Point p1, Point p2, Point p3) {

}
