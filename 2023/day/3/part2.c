#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_WIDTH 150

// Scan a number from str, but the number does not have to start at i.
// Returns true if a number was found, false otherwise.
bool sscanf_lr_int64(char *str, size_t i, int64_t *num, int32_t *pos) {
  bool found_digit = false;

  ssize_t j = i;
  for (j = i; j >= 0; j--) {
    if (str[j] >= '0' && str[j] <= '9') {
      found_digit = true;
    } else {
      break;
    }
  }

  if (found_digit) {
    sscanf(&str[j + 1], "%li%n", num, pos);

    // Make pos relative to i
    *pos -= (i - j) - 1;
  }

  return found_digit;
}

int main(int argc, char *argv[]) {
  FILE *stream = stdin;

  char *prev_line = malloc(LINE_WIDTH * sizeof(char));
  char *cur_line = malloc(LINE_WIDTH * sizeof(char));
  char *next_line = malloc(LINE_WIDTH * sizeof(char));
  char *tmp;
  size_t len = 0;
  ssize_t prev_nread = 0, cur_nread = 0, next_nread = 0;

  memset(prev_line, '\0', LINE_WIDTH);
  memset(cur_line, '\0', LINE_WIDTH);
  memset(next_line, '\0', LINE_WIDTH);

  int64_t gear_ratio_sum = 0;

  if ((next_nread = getline(&next_line, &len, stream)) == -1) {
    printf("Failed to read line\n");
    return EXIT_FAILURE;
  }

  while (next_nread != -1) {
    tmp = prev_line;
    prev_line = cur_line;
    prev_nread = cur_nread;
    cur_line = next_line;
    cur_nread = next_nread;
    next_line = tmp;
    next_nread = getline(&next_line, &len, stream);

    if (next_nread == -1) {
      memset(next_line, '\0', LINE_WIDTH);
    }

    for (size_t i = 0; i < (size_t)cur_nread; i++) {
      if (cur_line[i] == '*') {
        // Check for adjacent digits
        int64_t num1 = -1, num2 = -1, num3 = -1;
        int32_t pos;

        // Check left side of current line
        sscanf_lr_int64(cur_line, i - 1, &num1, &pos);

        // Check right side of current line
        sscanf_lr_int64(cur_line, i + 1, num1 >= 0 ? &num2 : &num1, &pos);

        // Check previous line
        for (ssize_t j = i - 1; j <= (ssize_t)(i + 1); j++) {
          if (j >= 0 && j < prev_nread) {
            int64_t *res = num1 >= 0 ? (num2 >= 0 ? &num3 : &num2) : &num1;
            if (sscanf_lr_int64(prev_line, j, res, &pos)) {
              j += pos;
            }
          }
        }

        // Check next line
        for (ssize_t j = i - 1; j <= (ssize_t)(i + 1); j++) {
          if (j >= 0 && j < next_nread) {
            int64_t *res = num1 >= 0 ? (num2 >= 0 ? &num3 : &num2) : &num1;
            if (sscanf_lr_int64(next_line, j, res, &pos)) {
              j += pos;
            }
          }
        }

        if (num1 >= 0 && num2 >= 0 && num3 == -1) {
          gear_ratio_sum += num1 * num2;
        }
      }
    }
  }

  printf("%li\n", gear_ratio_sum);

  free(prev_line);
  free(cur_line);
  free(next_line);

  return EXIT_SUCCESS;
}
