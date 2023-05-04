#!/usr/bin/env python3

from pathlib import Path
from argparse import ArgumentParser
from random import randrange, randint
import shutil


def build_argparser():
    parser = ArgumentParser(add_help=False)
    parser.add_argument('-d', '--dir', default='tests/test_data', help = 'Specify target directory')
    parser.add_argument('-n', '--number', type=int, default=10, help = 'Specify number of test cases to generate')
    return parser


def generate_data(cache_size: int, elems_number: int, upper_bound: int) -> list:
    data = [cache_size, elems_number]
    for _ in range(elems_number):
        data.append(randrange(upper_bound))
    return data


def intersections_triangle_3d():
    pass


def main():
    args = build_argparser().parse_args()
    root_data_dir = Path(args.dir)
    if root_data_dir.exists():
        shutil.rmtree(root_data_dir)
    root_data_dir.mkdir(parents=True)

    print("Start generating test data for caches...")

    algos = []
    if args.algo == 'lru':
        algos = [lru]
    elif args.algo == 'lfu':
        algos = [lfu]
    elif args.algo == 'perfect':
        algos = [perfect]
    elif args.algo == 'all':
        algos = [lru, perfect, lfu]
    else:
        raise ValueError(f"Unknown alorithm was provided {args.algo}")

    for i in range(args.number):
        data = generate_data(randint(1, args.cache_size), randint(1, args.elems_num), randint(1, args.upper_bound))
        file_name = f"{str(i)}.txt"
        with open(root_data_dir / file_name, 'w+') as f:
            for el in data[:-1]:
                f.write(str(el) + " ")
            f.write(str(data[-1]))

        for cache_algo in algos:
            answer = cache_algo(data)
            answers_dir =  root_data_dir / "answers" /cache_algo.__name__
            answers_dir.mkdir(parents=True, exist_ok=True)
            with open(answers_dir / file_name, 'w+') as f:
                f.write(str(answer))
    print(f"Test data files was written to {root_data_dir.resolve()}")

if __name__ == "__main__":
    main()