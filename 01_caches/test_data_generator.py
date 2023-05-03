#!/usr/bin/env python3

from pathlib import Path
from argparse import ArgumentParser
from random import randrange, randint


def build_argparser():
    parser = ArgumentParser(add_help=False)
    parser.add_argument('-a', '--algo', required=True, help = 'Specify algorithm for generating test data')
    parser.add_argument('-d', '--dir', default='tests/test_data', help = 'Specify target directory')
    parser.add_argument('-n', '--number', type=int, default=10, help = 'Specify number of test cases to generate')
    return parser


def generate_data(cache_size: int, elems_number: int, upper_bound: int) -> list:
    data = [cache_size, elems_number]
    for _ in range(elems_number):
        data.append(randrange(upper_bound))
    return data


def lru(data: list) -> int:
    cache_size = data[0]
    cache = []
    hits = 0
    for i in data[2:]:
        if i in cache:
            hits += 1
            if (i != cache[0]):
                cache.remove(i)
                cache.insert(0, i)
        else:
            if len(cache) == cache_size:
                cache = cache[:-1]
            cache.insert(0,i)
    return hits


def main():
    args = build_argparser().parse_args()
    root_data_dir = Path(args.dir)
    answers_dir =  root_data_dir / "answers" /args.algo
    answers_dir.mkdir(parents=True, exist_ok=True)
    print("Start generating test data for caches...")
    if args.algo == 'lru':
        for i in range(args.number):
            data = generate_data(randint(1, 10000), randint(1, 1000000), randint(1, 1000)) #(cache_size=3, elems_number=5, upper_bound=6) #randint(1, 3), randint(1, 40), randint(3,10))
            answer = lru(data)
            file_name = f"{str(i)}.txt";
            with open(root_data_dir / file_name, 'w+') as f:
                for el in data[:-1]:
                    f.write(str(el) + " ")
                f.write(str(data[-1]))

            with open(answers_dir / file_name, 'w+') as f:
                f.write(str(answer))
        print(f"Test data files was written to {root_data_dir.resolve()}")
    else:
        raise ValueError(f"Unknown alorithm was provided {args.algo}")
    print("Done!")

if __name__ == "__main__":
    main()