import sys

print("Reading from stdin", file=sys.stderr)

arr = []

for line in sys.stdin:
    try:
        a = int(line)
    except ValueError:
        continue

    for b in arr:
        for c in arr:
            if b != c and a + b + c == 2020:
                d = a * b * c
                print("Found", a, b, c, file=sys.stderr)
                print(a, "*", b, "*", c, "=", d, file=sys.stderr)
                print(d)
                sys.exit(0)

    arr.append(a)


print("No match", file=sys.stderr)
sys.exit(1)
