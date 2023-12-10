#include "common.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Parsing functions
bool read_many_int64(char *str, int64_t *arr, size_t *arr_len,
                     size_t arr_max_len) {
  int64_t num = 0;
  int n = 0;

  // Skip initial whitespace
  while (*str == ' ')
    str++;

  while (sscanf(str, "%ld%n", &num, &n) == 1) {
    str += n;

    assert((*arr_len) < arr_max_len);
    arr[*arr_len] = num;
    (*arr_len)++;

    // Skip whitespace
    while (*str == ' ')
      str++;
  }

  return true;
}

void parse_input(FILE *input, StaticArrayInt64 *arr, size_t *count) {
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  *count = 0;

  while ((nread = getline(&line, &len, input)) != -1) {
    if (!read_many_int64(line, arr->arr, &arr->len, arr->max_len)) {
      printf("Failed to parse input\n");
      return;
    }

    // Assert that all lines have the same number of values
    if (*count == 0) {
      *count = arr->len;
    }
    assert(arr->len % (*count) == 0);
  }

  free(line);
}

int64_t extrapolate(StaticArrayInt64 *arr, size_t index, size_t count) {
  // Note:
  // count is the number of original values in each row
  // count + 1 is the number of values including the extrapolated value

  // Calculation buffer
  StaticArrayInt64 buf =
      (StaticArrayInt64){.arr = (int64_t[1000]){0}, .len = 0, .max_len = 1000};

  // Copy initial values
  for (size_t i = 0; i < count; i++) {
    static_array_int64_push(&buf, arr->arr[index + i]);
  }

  // Reserve additional space for extrapolated value
  static_array_int64_push(&buf, -1);

  size_t row = 0;
  while (true) {
    bool is_all_zeroes = true;

    // Add new row
    for (size_t i = 0; i < count - 1 - row; i++) {
      size_t index = (count + 1) * row - ((row - 1) * row) / 2 + i;
      int64_t next = static_array_int64_get(&buf, index + 1);
      int64_t curr = static_array_int64_get(&buf, index);
      int64_t diff = next - curr;
      static_array_int64_push(&buf, diff);

      if (diff != 0) {
        is_all_zeroes = false;
      }
    }

    // Reserve additional space for extrapolated value
    static_array_int64_push(&buf, -1);

    row++;

    if (is_all_zeroes) {
      break;
    }
  }

  // Resolve extrapolations
  int64_t last_extrapoled = 0;
  for (size_t i = row; ; i--) {
    size_t index = (count + 1) * (i + 1) - (i * (i + 1)) / 2 - 1;
    int64_t value = static_array_int64_get(&buf, index);
    assert(value == -1);

    int64_t new_value;
    if (i == row) {
      new_value = 0;
    } else {
      int64_t prev_value = static_array_int64_get(&buf, index - 1);
      new_value = prev_value + last_extrapoled;
    }

    static_array_int64_set(&buf, index, new_value);
    last_extrapoled = new_value;

    // Avoid underflow
    if (i == 0) {
      break;
    }
  }

  return last_extrapoled;
}

int main(int argc, char *argv[]) {
  (void)argv;
  (void)argc;

  size_t count = 0;
  StaticArrayInt64 arr =
      (StaticArrayInt64){.arr = (int64_t[10000]){0}, .len = 0, .max_len = 10000};

  parse_input(stdin, &arr, &count);

  size_t rows = arr.len / count;

  int64_t sum = 0;
  for (size_t i = 0; i < rows; i++) {
    sum += extrapolate(&arr, i * count, count);
  }

  printf("%ld\n", sum);

  return EXIT_SUCCESS;
}
