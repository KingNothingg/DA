import sys
import random
from functools import cmp_to_key

TEST_COUNT = 3


class Phone:
    def __init__(self, num):
        self.num = num
    def __lt__(self, other):
        if len(self.num) < len(other.num):
            return True
        if len(self.num) > len(other.num):
            return False
        for i in range (0, len(self.num)):
            if self.num[i] == '+' or self.num[i] == '-':
                continue
            if self.num[i] < other.num[i]:
                return True
            if self.num[i] > other.num[i]:
                return False
        return False



def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <test directory>")
        sys.exit(1)

    test_dir = sys.argv[1]

    lines = []
    lines.extend([10 ** i for i in range(TEST_COUNT)])

    for enum, test_count in enumerate(range(1, TEST_COUNT + 1)):
        test = []

        line_count = lines[enum]
        for _ in range(line_count):
            key = Phone('+' + str(random.randint(1, 999)) + '-' + str(random.randint(100, 999)) + '-' + str(random.randint(1000000, 9999999)))
            value = random.randint(0, 2**64 - 1)
            test.append((key, value))

        test_name = "{}/{:02d}".format(test_dir, test_count)
        with open(f'{test_name}.t', 'w') as ftest:
            for key, value in test:
                ftest.write(f'{key.num} {value}\n')

        answer = sorted(test, key = lambda x: x[0])
        with open(f'{test_name}.a', 'w') as ftest:
            for key, value in answer:
                ftest.write(f'{key.num}\t{value}\n')

main()