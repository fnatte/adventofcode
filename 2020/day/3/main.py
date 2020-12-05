import sys

def get_hits(step_x, step_y, lines):
    hits = 0
    x = 0
    y = 0

    for idx, line in enumerate(lines):
        if idx < y:
            continue

        line = line.strip()
        if len(line) == 0:
            continue

        if line[x % len(line)] == '#':
            hits = hits + 1

        x = x + step_x
        y = y + step_y

    return hits


def main_part1():
    hits = get_hits(3, 1, sys.stdin)
    print("Hits:", file=sys.stderr)
    print(hits)

def main_part2():
    step_variants = [(1, 1), (3, 1), (5, 1), (7, 1), (1, 2) ]
    lines = [line for line in sys.stdin]
    code = 1
    for step_x, step_y in step_variants:
        res = get_hits(step_x, step_y, lines)
        code = code * res
        print("Right", step_x, "down", step_y, "hits", res, file=sys.stderr)

    print("Code:", file=sys.stderr)
    print(code)


if __name__ == '__main__':
    if len(sys.argv) > 1 and sys.argv[1] == "part1":
        main_part1()
    elif len(sys.argv) > 1 and sys.argv[1] == "part2":
        main_part2()
    else:
        print("Expected part1 or part2 as first argument", file=sys.stderr)
        sys.exit(1)
