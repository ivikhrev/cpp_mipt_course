#include "geom_structures.hpp"
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    int n;
    int verts_num = 3, coords_num = 3;
    std::vector<Triangle> triangles;
    std::cin >> n;
    for (int i = 0; i < n * verts_num * coords_num; ++i) {
        std::vector<Vec3> v;
        for (int j = 0; j < verts_num; ++j) {
            float x, y, z;
            std::cin >> x >> y >> z;
            v.emplace_back(x, y, z);
        }
        triangles.emplace_back(v);
    }

    int intersections_num = 0;
    for (int i = 0; i < static_cast<int>(triangles.size()); ++i) {
        for (int j = i + 1; j < static_cast<int>(triangles.size()); ++j) {
            intersections_num += test_triangles_intersection_3d(triangles[i], triangles[j]);
        }
    }

    std::cout << intersections_num << std::endl;
    return 0;
}
