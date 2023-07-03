#!/usr/bin/env python3

from argparse import ArgumentParser
from pathlib import Path
import random
import shutil


def build_argparser():
    parser = ArgumentParser(add_help=False)
    parser.add_argument('-d', '--dir', default='tests/test_data', help = 'Specify target directory')
    parser.add_argument('-n', '--num', type=int, default=10, help = 'Number of test cases to generate')
    parser.add_argument('-s', '--size', type=int, default=2, help = 'Size of matrices to generate')
    parser.add_argument('-u', '--upper_bound', type=int, default=10, help = 'Specify upper bound for matrix elements')
    return parser


def generate_data(matrix_size: int, upper_bound: int) -> list:
    data = [matrix_size]

    matrix = []
    values = list(range(1, upper_bound + 1))
    values.extend(list(range(-upper_bound, 0)))
    for i in range(matrix_size):
        for j in range(matrix_size):
            if (i <= j):
                matrix.append(random.choice(values))
            else:
                matrix.append(0)

    det = 1;
    for i in range(matrix_size):
        det *= matrix[i * matrix_size + i]

    data.extend(mix_matrix(matrix, matrix_size))
    return data, det


def mix_matrix(matrix: list, size: int):
    num_iter = random.randint(10, 50)
    for _ in range(num_iter):
        row1, row2 = random.sample(range(size), 2)
        sign = random.choice([1, -1])
        # matrix[row1 * size : (row1 + 1) * size], matrix[row2 * size : (row2 + 1) * size] = matrix[row2 * size : (row2 + 1) * size], matrix[row1 * size : (row1 + 1) * size]
        matrix[row1 * size : (row1 + 1) * size] = [a + sign * b for a, b in zip(matrix[row1 * size : (row1 + 1) * size], matrix[row2 * size : (row2 + 1) * size])]

    return matrix


def main():
    args = build_argparser().parse_args()
    root_data_dir = Path(args.dir)
    if root_data_dir.exists():
        shutil.rmtree(root_data_dir)
    root_data_dir.mkdir(parents=True)

    print("Start generating test data...")
    for i in range(args.num):
        data, answer = generate_data(random.randint(2, args.size), random.randint(1, args.upper_bound))
        file_name = f"{str(i)}.txt"
        with open(root_data_dir / file_name, 'w+') as f:
            for el in data[:-1]:
                f.write(str(el) + " ")
            f.write(str(data[-1]))

        # answer = get_answer(data)
        answers_dir =  root_data_dir / "answers"
        answers_dir.mkdir(parents=True, exist_ok=True)
        with open(answers_dir / file_name, 'w+') as f:
            f.write(str(answer))
    print(f"Test data files were written to {root_data_dir.resolve()}")

if __name__ == "__main__":
    main()
