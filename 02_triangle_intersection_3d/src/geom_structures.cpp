#include "geom_structures.hpp"

#include <algorithm>
#include <cmath>
#include <cassert>

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

Vec3 Vec3::operator*(int f) const {
    return {x * f, y * f, z * f};
}

Vec3 Vec3::operator/(int f) const {
    return {x / f, y / f, z / f};
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
    return sqrtf(len_squared());
}

float Vec3::len_squared() const {
    return x * x + y * y + z * z;
}

bool Vec3::is_normalized() const {
    return fabsf(len_squared() - 1.f) < numeric_utils::epsilon;
}

Vec3 Vec3::normalize() {
    float l = len();
    return {x / l, y / l, z / l};
}

int Vec3::max_idx() const {
    auto v = {x, y ,z};
    return std::distance(v.begin(), std::max_element(v.begin(), v.end(), [](float a, float b) {
        return std::abs(a) < std::abs(b);
    }));
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

Line::Line(const Plane& plane1, const Plane& plane2) {
    // Intersection of 2-planes: a variation based on the 3-plane version.
    // https://stackoverflow.com/a/32410473
    direction = cross_product(plane1.normal(), plane2.normal());

    float det = direction.len_squared();
    if (det > numeric_utils::epsilon) {
        point = (cross_product(direction, plane2.normal()) * plane1.d +
            cross_product(plane1.normal(), direction) * plane2.d) / det;
        direction = direction.normalize();
    } else {
        point = Vec3{std::nanf("x"), std::nanf("y"), std::nanf("z")};
    }
}

bool Triangle::operator==(const Triangle& other) const {
    return other.vertices[0] == other.vertices[0] &&
        other.vertices[1] == other.vertices[1] &&
        other.vertices[2] == other.vertices[2];
}

bool Triangle::operator!=(const Triangle& other) const {
    return !(*this == other);
}

Plane Triangle::get_plane() const {
    assert(vertices.size() > 2);
    return Plane(vertices[0], vertices[1], vertices[2]);
}

bool Triangle::valid() const {
    return std::all_of(vertices.begin(), vertices.end(), [] (const auto& v) { return v.valid(); });
}

bool Triangle::degenerate() const {
    float distance12 = calc_distance(vertices[0], vertices[1]);
    float distance23 = calc_distance(vertices[1], vertices[2]);
    float distance13 = calc_distance(vertices[0], vertices[2]);
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
    return cross_product(line.point - p, line.direction) == Vec3(0.f, 0.f, 0.f);
}

float calc_signed_distance(const Plane& plane, const Vec3& point) {
    Vec3 plane_normal = plane.normal();
    return plane(point) / plane_normal.len();
}

float calc_distance(const Vec3& point1, const Vec3& point2) {
    return (point2 - point1).len();
}

float calc_angle(const Plane& plane1, const Plane& plane2){
    return std::acos(fabsf(dot_product(plane1.normal(), plane2.normal())) / (plane1.normal().len() * plane2.normal().len()));
}

Vec3 calc_projection(const Plane& plane, const Vec3& point) {
    float k = (plane.d - plane.a * point.x - plane.b * point.y - plane.c * point.z) /
        (plane.normal().len() * plane.normal().len());

    return {point.x + k * plane.a, point.y + k * plane.b, point.z + k * plane.c};
}

float calc_projection_1d(const Line& line, const Vec3& point) {
    return dot_product(line.direction, point - line.point);
}

bool planes_are_parallel(const Plane& plane1, const Plane& plane2){
    return cross_product(plane1.normal(), plane2.normal()) == Vec3(0.f, 0.f, 0.f);
}

Vec3 perp2d(const Vec3& v) {
    return {v.y, -v.x, 0.f};
}

std::pair<float, float> compute_interval(const Triangle& t, const Vec3& d) {
    float min, max;
    min = max = dot_product(d, t.vertices[0]);
    for (int i = 1; i < static_cast<int>(t.vertices.size()); ++i) {
        float val = dot_product(d, t.vertices[i]);
        if (val < min) {
            min = val;
        }
        else if (val > max) {
            max = val;
        }
    }
    return {min, max};
}

bool test_triangles_intersection_2d(const Triangle& t1, const Triangle& t2) {
    // test edge normals of t1 for separation
    for (int i0 = 0, i1 = t1.vertices.size() - 1; i0 <  static_cast<int>(t1.vertices.size()); i1 = i0, ++i0) {
        Vec3 edge = t1.vertices[i0] - t1.vertices[i1];
        Vec3 d = perp2d(edge);
        auto [min0, max0] = compute_interval(t1, d);
        auto [min1, max1] = compute_interval(t2, d);
        if (max1 < min0 || max0 < min1) {
            return false;
        }
    }

    // test edge normals of t2 for separation
    for (int i0 = 0, i1 = t2.vertices.size() - 1; i0 <  static_cast<int>(t2.vertices.size()); i1 = i0, ++i0) {
        Vec3 edge = t2.vertices[i0] - t2.vertices[i1];
        Vec3 d = perp2d(edge);
        auto [min0, max0] = compute_interval(t1, d);
        auto [min1, max1] = compute_interval(t2, d);
        if (max1 < min0 || max0 < min1) {
            return false;
        }
    }

    return true;
}

Vec3 convert_point_to_2d(const Vec3& p, int zero_coordinate) {
    if (zero_coordinate == 0)
        return {p.y, p.z, 0.f};
    else if (zero_coordinate == 1)
        return {p.x, p.z, 0.f};
    return p;
}

Triangle convert_to_2d(const Triangle& t, int zero_coordinate) {
    return {convert_point_to_2d(t.vertices[0], zero_coordinate),
        convert_point_to_2d(t.vertices[1], zero_coordinate),
        convert_point_to_2d(t.vertices[2], zero_coordinate)};
}

std::tuple<float, float> isect(float v0, float v1 , float v2, float d0, float d1, float d2) {
    return { v0 + (v1 - v0) * d0 / (d0 - d1),
             v0 + (v2 - v0) * d0 / (d0 - d2) };
}

std::pair<float, float> compute_interval(const Triangle& t, const Line& l, const std::vector<float>& d) {
    int d_max_idx = l.direction.max_idx();
    float v0 = t.vertices[0][d_max_idx]; //calc_projection_1d(l, t.vertices[0]);
    float v1 = t.vertices[1][d_max_idx]; // calc_projection_1d(l, t.vertices[1]);
    float v2 = t.vertices[2][d_max_idx]; // calc_projection_1d(l, t.vertices[2]);
    float t0, t1;
    if (d[0] * d[1] > 0.f) {
        std::tie(t0, t1) = isect(v2, v0, v1, d[2], d[0], d[1]);
    }
    else if (d[0] * d[2] > 0.f) {
        std::tie(t0, t1) = isect(v1, v0, v2, d[1], d[0], d[2]);
    }
    else if (d[1] * d[2] > 0.f || d[0] != 0.f) {
        std::tie(t0, t1) = isect(v0, v1, v2, d[0], d[1], d[2]);
    }
    else if (d[1] != 0.f) {
        std::tie(t0, t1) = isect(v1, v0, v2, d[1], d[0], d[2]);
    }
    else if (d[2] != 0.f) {
        std::tie(t0, t1) = isect(v2, v0, v1, d[2], d[0], d[1]);
    }

    if (t0 < t1)
        return {t0, t1};
    else
        return {t1, t0};
}

// Paper http://web.stanford.edu/class/cs277/resources/papers/Moller1997b.pdf
// https://github.dev/erich666/jgt-code/blob/e67d05e4398c737abc40744cf3984c64b7df1e84/Volume_02/Number_2/Moller1997b/tritri_isectline.c
bool test_triangles_intersection_3d(const Triangle& t1, const Triangle& t2) {
    if (t1.degenerate() || t2.degenerate()) {
        return false;
    }

    Plane plane1 = t1.get_plane();
    float d21 = plane1(t2.vertices[0]); //calc_signed_distance(plane1, t2.vertices[0]);
    float d22 = plane1(t2.vertices[1]); //calc_signed_distance(plane1, t2.vertices[1]);
    float d23 = plane1(t2.vertices[2]); //calc_signed_distance(plane1, t2.vertices[2]);
    if ((d21 < 0 && d22 < 0 && d23 < 0) ||
        (d21 > 0 && d22 > 0 && d23 > 0)) {
        return false;
    }

    Plane plane2 = t2.get_plane();
    // same planes
    if (plane1 == plane2) {
        //compute 2d case
        Plane plane_xy(0, 0, 1, 0);
        Plane plane_yz(1, 0, 0, 0);
        Plane plane_xz(0, 1, 0, 0);
        float angle_xy = calc_angle(plane1, plane_xy);
        float angle_yz = calc_angle(plane1, plane_yz);
        float angle_xz = calc_angle(plane1, plane_xz);
        Triangle t1_projection, t2_projection;
        int zero_coordinate{};
        if (angle_xy <= angle_yz && angle_xy <= angle_xz) {
            t1_projection = {calc_projection(plane_xy, t1.vertices[0]),
                calc_projection(plane_xy, t1.vertices[1]),
                calc_projection(plane_xy, t1.vertices[2])};
            t2_projection = {calc_projection(plane_xy, t2.vertices[0]),
                calc_projection(plane_xy, t2.vertices[1]),
                calc_projection(plane_xy, t2.vertices[2])};
            zero_coordinate = 2;
        }
        else if (angle_yz <= angle_xy && angle_yz <= angle_xz) {
            t1_projection = {calc_projection(plane_xy, t1.vertices[0]),
                calc_projection(plane_yz, t1.vertices[1]),
                calc_projection(plane_yz, t1.vertices[2])};
            t2_projection = {calc_projection(plane_xy, t2.vertices[0]),
                calc_projection(plane_yz, t2.vertices[1]),
                calc_projection(plane_yz, t2.vertices[2])};
            zero_coordinate = 0;
        }
        else if (angle_xz <= angle_xy && angle_xz <= angle_yz) {
            t1_projection = {calc_projection(plane_xy, t1.vertices[0]),
                calc_projection(plane_xz, t1.vertices[1]),
                calc_projection(plane_xz, t1.vertices[2])};
            t2_projection = {calc_projection(plane_xy, t2.vertices[0]),
                calc_projection(plane_xz, t2.vertices[1]),
                calc_projection(plane_xz, t2.vertices[2])};
            zero_coordinate = 1;
        }

        return test_triangles_intersection_2d(convert_to_2d(t1_projection,  zero_coordinate),
            convert_to_2d(t2_projection, zero_coordinate));
    }
    else if (planes_are_parallel(plane1, plane2)) { // mb no need to check
        return false; // planes are parallel and not the same
    }

    float d11 = plane2(t1.vertices[0]); //calc_signed_distance(plane2, t1.vertices[0]);
    float d12 = plane2(t1.vertices[1]); //calc_signed_distance(plane2, t1.vertices[1]);
    float d13 = plane2(t1.vertices[2]); //calc_signed_distance(plane2, t1.vertices[2]);
    if ((d11 < 0 && d12 < 0 && d13 < 0) ||
        (d11 > 0 && d12 > 0 && d13 > 0)) {
        return false;
    }

    Line intersection_line{plane1, plane2};

    auto [min0, max0] = compute_interval(t1, intersection_line, {d11, d12, d13});
    auto [min1, max1] = compute_interval(t2, intersection_line, {d21, d22, d23});

    if (max1 < min0 || max0 < min1) {
        return false;
    }

    return true;
}
