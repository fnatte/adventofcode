#include <stdio.h>
#include <stdlib.h>

#define MAX_ITEMS 1000

void sort(int32_t *arr, size_t len) {
  for (size_t i = 0; i < len; i++) {
    for (size_t j = 0; j < len - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        size_t tmp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = tmp;
      }
    }
  }
}

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

  sort(left, len);
  sort(right, len);

  int32_t sum = 0;
  for (size_t i = 0; i < len; i++) {
    int32_t diff = abs(left[i] - right[i]);
    sum += diff;
  }

  printf("%d\n", sum);

  free(line);

  return EXIT_SUCCESS;
}
