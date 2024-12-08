#include "part2.h"
#include <stdlib.h>

bool test_run() {
  FILE *stream = fopen("./example_input.txt", "r");
  if (stream == NULL) {
    perror("Failed to open input file");
  }

  int32_t got = run(stream);
  int32_t expected = 11387;

  if (got != expected) {
      fprintf(stderr, "Test failed: Got %d, expected %d\n", got, expected);
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
