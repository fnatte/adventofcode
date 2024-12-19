#include "part1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool test_measure_trail_score(char *data, pos_t pos, int64_t expected) {
  FILE *stream = fmemopen(data, strlen(data), "r");
  if (stream == NULL) {
    perror("fmemopen failed");
    return false;
  }

  map_t map = {0};
  if (!read_map(stream, &map)) {
    fprintf(stderr, "Failed to read map\n");
    return false;
  }

  int64_t score = measure_trail_score(&map, pos);

  if (score != expected) {
    fprintf(stderr, "Got %ld, expected %ld\n", score, expected);
    return false;
  }

  return true;
}

bool test_run(char *data, int64_t expected) {
  FILE *stream = fmemopen(data, strlen(data), "r");
  if (stream == NULL) {
    perror("fmemopen failed");
    return false;
  }

  map_t map = {0};
  if (!read_map(stream, &map)) {
    fprintf(stderr, "Failed to read map\n");
    return false;
  }

  int64_t sum = run(&map);

  if (sum != expected) {
    fprintf(stderr, "Got %ld, expected %ld\n", sum, expected);
    return false;
  }

  return true;
}

bool test_example_1() {
  char data[] = "0123\n"
                "1234\n"
                "8765\n"
                "9876\n";
  return test_measure_trail_score(data, (pos_t){.x = 0, .y = 0}, 1);
}

bool test_example_1_zero_score() {
  char data[] = "0123\n"
                "1234\n"
                "7765\n"
                "9676\n";
  return test_measure_trail_score(data, (pos_t){.x = 0, .y = 0}, 0);
}

bool test_example_2() {
  char data[] = "...0...\n"
                "...1...\n"
                "...2...\n"
                "6543456\n"
                "7.....7\n"
                "8.....8\n"
                "9.....9\n";
  return test_measure_trail_score(data, (pos_t){.x = 3, .y = 0}, 2);
}

bool test_example_3() {
  char data[] = "..90..9\n"
                "...1.98\n"
                "...2..7\n"
                "6543456\n"
                "765.987\n"
                "876....\n"
                "987....\n";
  return test_measure_trail_score(data, (pos_t){.x = 3, .y = 0}, 4);
}

bool test_example_4_trail_1() {
  char data[] = "10..9..\n"
                "2...8..\n"
                "3...7..\n"
                "4567654\n"
                "...8..3\n"
                "...9..2\n"
                ".....01\n";
  return test_measure_trail_score(data, (pos_t){.x = 1, .y = 0}, 1);
}

bool test_example_4_trail_2() {
  char data[] = "10..9..\n"
                "2...8..\n"
                "3...7..\n"
                "4567654\n"
                "...8..3\n"
                "...9..2\n"
                ".....01\n";
  return test_measure_trail_score(data, (pos_t){.x = 5, .y = 6}, 2);
}

bool test_example_4_run() {
  char data[] = "10..9..\n"
                "2...8..\n"
                "3...7..\n"
                "4567654\n"
                "...8..3\n"
                "...9..2\n"
                ".....01\n";
  return test_run(data, 3);
}

bool test_example_5_run() {
  char data[] = "89010123\n"
                "78121874\n"
                "87430965\n"
                "96549874\n"
                "45678903\n"
                "32019012\n"
                "01329801\n"
                "10456732\n";
  return test_run(data, 36);
}

bool test_part1_run() {
  FILE *stream = fopen("./input.txt", "r");
  if (stream == NULL) {
    perror("Failed to open input file");
    return false;
  }
  map_t map = {0};
  if (!read_map(stream, &map)) {
    fprintf(stderr, "Failed to read map\n");
    return false;
  }

  int64_t expected = 659;
  int64_t got = run(&map);

  if (got != expected) {
    fprintf(stderr, "Got %ld, expected %ld\n", got, expected);
    return false;
  }

  return true;
}

int main() {
  if (!test_example_1() || !test_example_1_zero_score() || !test_example_2() ||
      !test_example_3() || !test_example_4_trail_1() ||
      !test_example_4_trail_2() || !test_example_4_run() ||
      !test_example_5_run() || !test_part1_run()) {
    fprintf(stderr, "Test failed.\n");
    return EXIT_FAILURE;
  }

  fprintf(stderr, "All tests passed.\n");

  return EXIT_SUCCESS;
}
