#!/usr/bin/env python3

from argparse import ArgumentParser
import numpy as np
from pathlib import Path
from random import randint, uniform
import shutil


def build_argparser():
    parser = ArgumentParser(add_help=False)
    parser.add_argument('-d', '--dir', default='test_data', help = 'Specify target directory')
    parser.add_argument('-n', '--num', type=int, default=10, help = 'Specify number of test cases to generate')
    parser.add_argument('-t', '--triangles_num', type=int, default=10, help = 'Specify upper bound for number of triangles')
    return parser


def generate_data(triangles_num: int) -> list:
    data = [triangles_num]
    points_num = 3
    coords_num = 3
    for _ in range(triangles_num * points_num * coords_num):
        data.append(uniform(-10, 10))
    return data


def signed_volume(a, b, c, d):
    """Computes the signed volume of a series of tetrahedrons defined by the vertices in
    a, b c and d. The ouput is an SxT array which gives the signed volume of the tetrahedron defined
    by the line segment 's' and two vertices of the triangle 't'."""

    return np.sum((a-d)*np.cross(b-d, c-d), axis=2)


# https://stackoverflow.com/a/59219749
def intersect_segment_triangle(s, t):
    """For each line segment in 's', this function computes whether it intersects any of the triangles
    given in 't'."""
    # compute the normals to each triangle
    normals = np.cross(t[2]-t[0], t[2]-t[1])
    normals /= np.linalg.norm(normals, axis=1)[:, np.newaxis]

    # get sign of each segment endpoint, if the sign changes then we know this segment crosses the
    # plane which contains a triangle. If the value is zero the endpoint of the segment lies on the
    # plane.
    # s[i][:, np.newaxis] - t[j] -> S x T x 3 array
    sign1 = np.sign(np.sum(normals*(s[0][:, np.newaxis] - t[2]), axis=2)) # S x T
    sign2 = np.sign(np.sum(normals*(s[1][:, np.newaxis] - t[2]), axis=2)) # S x T

    # determine segments which cross the plane of a triangle. 1 if the sign of the end points of s is
    # different AND one of end points of s is not a vertex of t
    cross = (sign1 != sign2) * (sign1 != 0) * (sign2 != 0) # S x T

    # get signed volumes
    v1 = np.sign(signed_volume(t[0], t[1], s[0][:, np.newaxis], s[1][:, np.newaxis])) # S x T
    v2 = np.sign(signed_volume(t[1], t[2], s[0][:, np.newaxis], s[1][:, np.newaxis])) # S x T
    v3 = np.sign(signed_volume(t[2], t[0], s[0][:, np.newaxis], s[1][:, np.newaxis])) # S x T

    same_volume = np.logical_and((v1 == v2), (v2 == v3)) # 1 if s and t have same sign in v1, v2 and v3

    return (np.sum(cross*same_volume, axis=1) > 0)


def intersections_triangle_3d(data: list):
    intersections_num = 0
    n = data[0]
    data = np.array(data[1:])
    for i in range(n):
        p11 = data[9 * i:9 * i + 3]
        p12 = data[9 * i + 3: 9 * i + 6]
        p13 = data[9 * i + 6:9 * i + 9]
        for j in range(i + 1, n):
            p21 = data[9 * j:9 * j + 3]
            p22 = data[9 * j + 3:9 * j + 6]
            p23 = data[9 * j + 6:9 * j + 9]
            segments_t2 = np.array([[p21, p22, p23], [p22, p23, p21]])
            triangle1 = np.array([[p11], [p12], [p13]])
            segments_t1 = np.array([[p11, p12, p13], [p12, p13, p11]])
            triangle2 = np.array([[p21], [p22], [p23]])

            if any(intersect_segment_triangle(segments_t2, triangle1)) or any(intersect_segment_triangle(segments_t1, triangle2)):
                intersections_num += 1

    return intersections_num


def main():
    args = build_argparser().parse_args()
    root_data_dir = Path(args.dir)
    if root_data_dir.exists():
        shutil.rmtree(root_data_dir)
    root_data_dir.mkdir(parents=True)

    print("Start generating test data for triangles intersection...")
    # with open('/home/ivikhrev/code/cpp_mipt_course/02_triangle_intersection_3d/tests/test_data/0.txt') as f:
    #     data = []
    #     for line in f: # read rest of lines
    #         data.extend([float(x) for x in line.split()])
    #     data[0] = int(data[0])
    # print(data)
    for i in range(args.num):
        data = generate_data(randint(2, args.triangles_num))
        file_name = f"{str(i)}.txt"
        with open(root_data_dir / file_name, 'w+') as f:
            for el in data[:-1]:
                f.write(str(el) + " ")
            f.write(str(data[-1]))

        answer = intersections_triangle_3d(data)
        answers_dir =  root_data_dir / "answers"
        answers_dir.mkdir(parents=True, exist_ok=True)
        with open(answers_dir / file_name, 'w+') as f:
            f.write(str(answer))
    print(f"Test data files were written to {root_data_dir.resolve()}")

if __name__ == "__main__":
    main()
