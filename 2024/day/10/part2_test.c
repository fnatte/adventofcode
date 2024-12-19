#include "part2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool test_measure_trail_rating(char *data, pos_t pos, int64_t expected) {
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

  int64_t score = measure_trail_rating(&map, pos);

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
  char data[] = ".....0.\n"
                "..4321.\n"
                "..5..2.\n"
                "..6543.\n"
                "..7..4.\n"
                "..8765.\n"
                "..9....\n";
  return test_measure_trail_rating(data, (pos_t){.x = 5, .y = 0}, 3);
}

bool test_example_2() {
  char data[] = "..90..9\n"
                "...1.98\n"
                "...2..7\n"
                "6543456\n"
                "765.987\n"
                "876....\n"
                "987....\n";
  return test_measure_trail_rating(data, (pos_t){.x = 3, .y = 0}, 13);
}

bool test_example_3() {
  char data[] = "012345\n"
                "123456\n"
                "234567\n"
                "345678\n"
                "4.6789\n"
                "56789.\n";

  return test_measure_trail_rating(data, (pos_t){.x = 0, .y = 0}, 227);
}

bool test_example_4() {
  char data[] = "89010123\n"
                "78121874\n"
                "87430965\n"
                "96549874\n"
                "45678903\n"
                "32019012\n"
                "01329801\n"
                "10456732\n";
  return test_run(data, 81);
}

bool test_part2_run() {
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

  int64_t expected = 1463;
  int64_t got = run(&map);

  if (got != expected) {
    fprintf(stderr, "Got %ld, expected %ld\n", got, expected);
    return false;
  }

  return true;
}

int main() {
  if (!test_example_1() || !test_example_2() || !test_example_3() ||
      !test_example_4() || !test_part2_run()) {
    fprintf(stderr, "Test failed.\n");
    return EXIT_FAILURE;
  }

  fprintf(stderr, "All tests passed.\n");

  return EXIT_SUCCESS;
}
