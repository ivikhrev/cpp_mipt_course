#!/usr/bin/env python3

from argparse import ArgumentParser
import numpy as np
from pathlib import Path
import random
import shutil


def build_argparser():
    parser = ArgumentParser(add_help=False)
    parser.add_argument('-d', '--dir', default='test_data', help = 'Specify target directory')
    parser.add_argument('-n', '--num', type=int, default=10, help = 'Specify number of test cases to generate')
    parser.add_argument('--elems_num', type=int, default=10, help = 'Specify number of input elements')
    parser.add_argument('--upper_bound', type=int, default=10, help = 'Specify upper bound for input elements')
    return parser


def generate_data(elems_number: int, upper_bound: int) -> list:
    data = []
    i = 0
    values = random.sample(range(upper_bound), random.randint(elems_number, upper_bound))
    data = ['k', values[0]]
    values.pop(0)
    while len(values) != 0:
        op = random.choice(['k', 'm', 'n'])
        data.append(op)
        if op == 'k':
            i += 1
            data.append(values[-1])
            values = values[:-1]
        elif op == 'n':
            data.append(random.randrange(upper_bound))
        elif op == 'm':
            data.append(random.randint(1, i + 1))
    return data


def search(data: list):
    result = []
    values = []
    for i in range(len(data) // 2):
        op = data[2 * i]
        number = data[2 * i + 1]
        if op == 'k':
            values.append(number)
            values = sorted(values)
        elif op == 'm':
            result.append(values[number - 1])
        elif op == 'n':
            i = 0
            while i < len(values):
                if values[i] >= number:
                    break
                i += 1
            result.append(i)

    return result


def main():
    args = build_argparser().parse_args()
    if (args.upper_bound < args.elems_num):
        args.upper_bound = args.elems_num;

    root_data_dir = Path(args.dir)
    if root_data_dir.exists():
        shutil.rmtree(root_data_dir)
    root_data_dir.mkdir(parents=True)

    print("Start generating test data for searching trees...")
    # with open('/home/ivikhrev/code/cpp_mipt_course/02_triangle_intersection_3d/tests/test_data/0.txt') as f:
    #     data = []
    #     for line in f: # read rest of lines
    #         data.extend([float(x) for x in line.split()])
    #     data[0] = int(data[0])
    # print(data)
    for i in range(args.num):
        data = generate_data(random.randint(3, args.elems_num), random.randint(args.elems_num, args.upper_bound))
        file_name = f"{str(i)}.txt"
        with open(root_data_dir / file_name, 'w+') as f:
            for el in data[:-1]:
                f.write(str(el) + " ")
            f.write(str(data[-1]))

        answer = search(data)
        answers_dir =  root_data_dir / "answers"
        answers_dir.mkdir(parents=True, exist_ok=True)
        with open(answers_dir / file_name, 'w+') as f:
            for el in answer[:-1]:
                f.write(str(el) + " ")
            f.write(str(answer[-1]))
    print(f"Test data files were written to {root_data_dir.resolve()}")

if __name__ == "__main__":
    main()
