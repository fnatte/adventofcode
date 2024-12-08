all: part1 part2

# PART 1

part1: part1.c lib.c
	gcc -o part1 part1.c lib.c -Wall -Wextra -pedantic

part1_run: part1
	cat input.txt | ./part1

part1_test: part1_test.c part1.c lib.c
	gcc -o part1_test part1_test.c lib.c part1.c -Wall -Wextra -pedantic -g -DRUN_TESTS

.PHONY: part1_test_run
part1_test_run: part1_test
	./part1_test

# PART 2

part2: part2.c lib.c
	gcc -o part2 part2.c lib.c -Wall -Wextra -pedantic

part2_run: part2
	cat input.txt | ./part2

part2_test: part2_test.c part2.c lib.c
	gcc -o part2_test part2_test.c lib.c part2.c -Wall -Wextra -pedantic -DRUN_TESTS

.PHONY: part2_test_run
part2_test_run: part2_test
	./part2_test

# LIB

lib_test: lib_test.c lib.c
	gcc -o lib_test lib_test.c lib.c -Wall -Wextra -pedantic -g

.PHONY: lib_test_run
lib_test_run: lib_test
	./lib_test


