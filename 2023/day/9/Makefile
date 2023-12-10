all: part1 part2

.PHONY: test

part1: part1.c common.c
	gcc -g -o part1 part1.c common.c -Wall -Wextra -pedantic -lm

part2: part2.c common.c
	gcc -g -O3 -o part2 part2.c common.c -Wall -Wextra -pedantic -lm

testrunner: test.c common.c
	gcc -g -o test test.c common.c -Wall -Wextra -pedantic -lm

test: testrunner
	./test

