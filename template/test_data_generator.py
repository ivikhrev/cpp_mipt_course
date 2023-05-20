#!/usr/bin/env python3

from argparse import ArgumentParser
from pathlib import Path
import random
import shutil


def build_argparser():
    parser = ArgumentParser(add_help=False)
    parser.add_argument('-d', '--dir', default='tests/test_data', help = 'Specify target directory')
    parser.add_argument('-n', '--num', type=int, default=10, help = 'Specify number of test cases to generate')
    parser.add_argument('--elems_num', type=int, default=10, help = 'Specify number of input elements')
    parser.add_argument('--upper_bound', type=int, default=10, help = 'Specify upper bound for input elements')
    return parser


def generate_data(elems_number: int, upper_bound: int) -> list:
    data = []
    return data


def get_answer(data: list):
    result = 0
    return result


def main():
    args = build_argparser().parse_args()
    root_data_dir = Path(args.dir)
    if root_data_dir.exists():
        shutil.rmtree(root_data_dir)
    root_data_dir.mkdir(parents=True)

    print("Start generating test data...")
    for i in range(args.num):
        data = generate_data(random.randint(3, args.elems_num), random.randint(args.elems_num, args.upper_bound))
        file_name = f"{str(i)}.txt"
        with open(root_data_dir / file_name, 'w+') as f:
            for el in data[:-1]:
                f.write(str(el) + " ")
            f.write(str(data[-1]))

        answer = get_answer(data)
        answers_dir =  root_data_dir / "answers"
        answers_dir.mkdir(parents=True, exist_ok=True)
        with open(answers_dir / file_name, 'w+') as f:
            for el in answer[:-1]:
                f.write(str(el) + " ")
            f.write(str(answer[-1]))
    print(f"Test data files was written to {root_data_dir.resolve()}")

if __name__ == "__main__":
    main()
