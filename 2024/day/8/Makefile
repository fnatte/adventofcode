all: part1 part2

OUT_DIR=out

.PHONY: out_dir
out_dir:
	mkdir -p $(OUT_DIR)

# PART 1

part1: part1.c lib.c out_dir
	gcc -o $(OUT_DIR)/part1 part1.c lib.c -Wall -Wextra -pedantic

part1_run: part1
	cat input.txt | $(OUT_DIR)/./part1

part1_test: part1_test.c part1.c lib.c out_dir
	gcc -o $(OUT_DIR)/part1_test part1_test.c lib.c part1.c -Wall -Wextra -pedantic -g -DRUN_TESTS

.PHONY: part1_test_run
part1_test_run: part1_test
	$(OUT_DIR)/part1_test

# PART 2

part2: part2.c lib.c out_dir
	gcc -o $(OUT_DIR)/part2 part2.c lib.c -Wall -Wextra -pedantic

part2_run: part2
	cat input.txt | $(OUT_DIR)/part2

part2_test: part2_test.c part2.c lib.c out_dir
	gcc -o $(OUT_DIR)/part2_test part2_test.c lib.c part2.c -Wall -Wextra -pedantic -DRUN_TESTS

.PHONY: part2_test_run
part2_test_run: part2_test
	$(OUT_DIR)/part2_test

# LIB

lib_test: lib_test.c lib.c out_dir
	gcc -o $(OUT_DIR)/lib_test lib_test.c lib.c -Wall -Wextra -pedantic -g

.PHONY: lib_test_run
lib_test_run: lib_test
	$(OUT_DIR)/lib_test


