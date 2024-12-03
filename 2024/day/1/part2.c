#include <stdio.h>
#include <stdlib.h>

#define MAX_ITEMS 1000

int main() {
  FILE *stream = stdin;
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  size_t current_line = 0;
  int32_t left[MAX_ITEMS];
  int32_t right[MAX_ITEMS];

  while ((nread = getline(&line, &len, stream)) != -1) {
    // Parse two numbers on each line separated by at least one space
    // Example line: 3  5
    int32_t num1 = 0;
    int32_t num2 = 0;

    if (sscanf(line, "%d %d", &num1, &num2) != 2) {
      fprintf(stderr, "Invalid input: %s", line);
      return EXIT_FAILURE;
    }

    left[current_line] = num1;
    right[current_line] = num2;
    current_line++;
  }

  len = current_line;

  int32_t sum = 0;
  for (size_t i = 0; i < len; i++) {
    int32_t matches = 0;
    for (size_t j = 0; j < len; j++) {
      if (left[i] == right[j]) {
        matches++;
      }
    }
    sum += left[i] * matches;
  }

  printf("%d\n", sum);

  free(line);

  return EXIT_SUCCESS;
}
