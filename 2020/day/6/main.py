import sys

MODE_EVERYONE = 1
MODE_ANYONE = 2

def sum_of_questions_per_group(lines, mode=MODE_ANYONE):
    group = None
    count = 0
    for line in lines:
        line = line.strip()
        if len(line) == 0:
            count = count + len(group)
            group = None
            continue
        if mode == MODE_ANYONE:
            group = (group or set()) | set(line)
        elif mode == MODE_EVERYONE:
            if group is None:
                group = set(line)
            else:
                group = group & set(line)
    return count

def main_part1():
    print(sum_of_questions_per_group(sys.stdin, MODE_ANYONE))

def main_part2():
    print(sum_of_questions_per_group(sys.stdin, MODE_EVERYONE))

if __name__ == '__main__':
    if len(sys.argv) > 1 and sys.argv[1] == "part1":
        main_part1()
    elif len(sys.argv) > 1 and sys.argv[1] == "part2":
        main_part2()
    else:
        print("Expected part1 or part2 as first argument", file=sys.stderr)
        sys.exit(1)
