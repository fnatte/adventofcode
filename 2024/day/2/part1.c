#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 100

size_t read_many_int32(char *str, int32_t *arr) {
  char *rest = str;
  char *token;
  int32_t num;
  size_t arr_len = 0;

  while ((token = strtok_r(rest, " ", &rest))) {
    if (sscanf(token, "%d", &num) == 1) {
      arr[arr_len++] = num;
    }
  }

  return arr_len;
}

int main() {
  FILE *stream = stdin;
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  int32_t nums[MAX_ITEMS];
  size_t nums_len;

  int32_t num_valid = 0;

  while ((nread = getline(&line, &len, stream)) != -1) {
    nums_len = read_many_int32(line, nums);
    assert(nums_len > 1);

    int32_t first = nums[0];
    int32_t second = nums[1];

    int32_t first_diff = second - first;
    int32_t first_dist = abs(first_diff);
    if (first_dist < 1 || first_dist > 3) {
      // Invalid
      continue;
    }

    for (size_t i = 2; i < nums_len; i++) {
      int32_t diff = nums[i] - nums[i - 1];
      int32_t dist = abs(diff);
      if (dist < 1 || dist > 3 || (diff ^ first_diff) < 0) {
        // Invalid
        goto next;
      }
    }

    num_valid++;

  next:
    continue;
  }

  printf("%d\n", num_valid);

  free(line);

  return EXIT_SUCCESS;
}
