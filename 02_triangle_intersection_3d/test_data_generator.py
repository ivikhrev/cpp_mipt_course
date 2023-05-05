#!/usr/bin/env python3

from pathlib import Path
from argparse import ArgumentParser
from random import randint, uniform
import shutil


def build_argparser():
    parser = ArgumentParser(add_help=False)
    parser.add_argument('-d', '--dir', default='tests/test_data', help = 'Specify target directory')
    parser.add_argument('-n', '--num', type=int, default=10, help = 'Specify number of test cases to generate')
    parser.add_argument('-tn', '--triangles_num', type=int, default=10, help = 'Specify upper bound for number of triangles')
    return parser


def generate_data(triangles_num: int) -> list:
    data = [triangles_num]
    points_num = 3
    coords_num = 3
    for _ in range(triangles_num * points_num * coords_num):
        data.append(uniform(-100, 100))
    return data


def intersections_triangle_3d():
    pass


def main():
    args = build_argparser().parse_args()
    root_data_dir = Path(args.dir)
    if root_data_dir.exists():
        shutil.rmtree(root_data_dir)
    root_data_dir.mkdir(parents=True)

    print("Start generating test data for triangles intersection...")

    for i in range(args.num):
        data = generate_data(randint(2, args.triangles_num))
        file_name = f"{str(i)}.txt"
        with open(root_data_dir / file_name, 'w+') as f:
            for el in data[:-1]:
                f.write(str(el) + " ")
            f.write(str(data[-1]))

            # answer = cache_algo(data)
            # answers_dir =  root_data_dir / "answers"
            # answers_dir.mkdir(parents=True, exist_ok=True)
            # with open(answers_dir / file_name, 'w+') as f:
            #     f.write(str(answer))
    print(f"Test data files was written to {root_data_dir.resolve()}")

if __name__ == "__main__":
    main()