#include "part2.h"
#include <stdlib.h>
#include <string.h>

bool test_run() {
  FILE *stream = fopen("./example_input.txt", "r");
  if (stream == NULL) {
    perror("Failed to open input file");
  }

  char map[4096];

  int32_t got = run(stream, map, 4096);

  int32_t expected_count = 34;
  const char *expected_map = "##....#....#\n"
                             ".#.#....0...\n"
                             "..#.#0....#.\n"
                             "..##...0....\n"
                             "....0....#..\n"
                             ".#...#A....#\n"
                             "...#..#.....\n"
                             "#....#.#....\n"
                             "..#.....A...\n"
                             "....#....A..\n"
                             ".#........#.\n"
                             "...#......##\n";

  size_t expected_len = strlen(expected_map);
  size_t map_len = strlen(map);
  if (expected_len != map_len) {
    fprintf(stderr, "Test failed: map length: got %zu, expected %zu\n", map_len,
            expected_len);
    return false;
  }

  for (size_t i = 0; i < map_len; i++) {
    if (map[i] != expected_map[i]) {
      fprintf(stderr, "Test failed: map mismatch at %zu: got %c, expected %c\n",
              i, map[i], expected_map[i]);
      fprintf(stderr, "\nGot:\n%s\nExpected:\n%s\n", map, expected_map);
      return false;
    }
  }

  if (got != expected_count) {
    fprintf(stderr, "Test failed: count: got %d, expected %d\n", got,
            expected_count);
    return false;
  }

  return true;
}

int main() {
  if (!test_run()) {
    return EXIT_FAILURE;
  }

  fprintf(stderr, "All tests passed.\n");

  return EXIT_SUCCESS;
}
