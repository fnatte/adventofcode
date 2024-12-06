all: part1 part2

part1: part1.c
	gcc -o part1 part1.c lib.c -Wall -Wextra -pedantic

test-part1: part1
	./test.sh part1

run-part1: part1
	cat input.txt | ./part1

part2: part2.c
	gcc -o part2 part2.c lib.c -Wall -Wextra -pedantic

test-part2: part2
	./test.sh part2

run-part2: part2
	cat input.txt | ./part2

test-lib: test_lib.c
	gcc -o test_lib test_lib.c lib.c -Wall -Wextra -pedantic -g && ./test_lib

