import sys

def get_pos_from_bsp_code(line):
    row = 0
    for i in range(7):
        if line[i] == 'B':
            row = row | (1 << (6-i))

    col = 0
    for i in range(3):
        if line[i+7] == 'R':
            col = col | (1 << (2-i))

    return (row, col)

def get_seat_id_from_row_col(row, col):
    return row * 8 + col

def main_part1():
    highest_id = 0
    for line in sys.stdin:
        line = line.strip()
        if len(line) != 10:
            continue
        row, col = get_pos_from_bsp_code(line)
        id = get_seat_id_from_row_col(row, col)
        if id > highest_id:
            highest_id = id
    print(highest_id)

def main_part2():
    arr = [0] * (128 * 8 - 1)
    for line in sys.stdin:
        line = line.strip()
        if len(line) != 10:
            continue
        row, col = get_pos_from_bsp_code(line)
        id = get_seat_id_from_row_col(row, col)
        arr[id] = 1

    for i in range(8, len(arr)-8):
        if arr[i-1] == 1 and arr[i] == 0 and arr[i+1] == 1:
            print(i)

if __name__ == '__main__':
    if len(sys.argv) > 1 and sys.argv[1] == "part1":
        main_part1()
    elif len(sys.argv) > 1 and sys.argv[1] == "part2":
        main_part2()
    else:
        print("Expected part1 or part2 as first argument", file=sys.stderr)
        sys.exit(1)
