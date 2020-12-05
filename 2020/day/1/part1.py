import sys

print("Reading from stdin", file=sys.stderr)

arr = []

for line in sys.stdin:
    try:
        a = int(line)
    except ValueError:
        continue

    for b in arr:
        if a + b == 2020:
            c = a * b
            print("Found", a, b, file=sys.stderr)
            print(a, "*", b, "=", c, file=sys.stderr)
            print(c)
            sys.exit(0)

    arr.append(a)


print("No match", file=sys.stderr)
sys.exit(1)
