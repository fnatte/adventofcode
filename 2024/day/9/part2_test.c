#include "part2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool test_run() {
  FILE *stream = fopen("./example_input.txt", "r");
  if (stream == NULL) {
    perror("Failed to open input file");
  }

  int64_t checksum = run(stream);
  int64_t expected = 2858;
  if (checksum != expected) {
    fprintf(stderr, "Got %ld, expected %ld\n", checksum, expected);
    return false;
  }

  return true;
}

bool test_run_full() {
  FILE *stream = fopen("./input.txt", "r");
  if (stream == NULL) {
    perror("Failed to open input file");
  }

  int64_t checksum = run(stream);
  int64_t expected = 6323761685944;
  if (checksum != expected) {
    fprintf(stderr, "Got %ld, expected %ld\n", checksum, expected);
    return false;
  }

  return true;
}

int main() {
  if (!test_run() || !test_run_full()) {
    return EXIT_FAILURE;
  }

  fprintf(stderr, "All tests passed.\n");

  return EXIT_SUCCESS;
}
