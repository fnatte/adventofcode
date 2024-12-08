#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

#define NUMBERS_SIZE 32

typedef struct {
  int64_t value;
  int64_t numbers[NUMBERS_SIZE];
  size_t numbers_len;
} equation_t;

bool parse_equation(char *str, equation_t *result) {
  int pos;
  if (sscanf(str, "%ld:%n", &result->value, &pos) != 1) {
    return false;
  }
  assert(pos > 0);

  result->numbers_len = scan_many_int64(&str[pos], result->numbers, " ");
  assert(result->numbers_len > 0);

  return true;
}

bool equation_test(equation_t *e, char *ops) {
  int64_t value = e->numbers[0];
  for (size_t i = 1; i < e->numbers_len; i++) {
    if (ops[i - 1] == '+') {
      value += e->numbers[i];
    } else {
      value *= e->numbers[i];
    }
  }

  return value == e->value;
}

bool equation_brute_force_operations(equation_t *e) {
  char ops[NUMBERS_SIZE];
  size_t n = e->numbers_len - 1;

  // max = 2 ^ n
  int32_t max = 2;
  for (size_t a = 0; a < n; a++) {
    max *= 2;
  }

  for (int32_t v = 0; v < max; v++) {
    for (size_t i = 0; i < n; i++) {
      int32_t vi = v & (1 << i);
      ops[i] = vi == 0 ? '+' : '*';
    }

    if (equation_test(e, ops)) {
      return true;
    }
  }

  return false;
}

int64_t run(FILE *stream) {
  char *line = NULL;
  size_t line_cap = 0;
  ssize_t line_len;

  equation_t e;

  int64_t sum = 0;

  int32_t count = 0;
  int32_t num_valid = 0;

  while ((line_len = getline(&line, &line_cap, stream)) != -1) {
    if (!parse_equation(line, &e)) {
      fprintf(stderr, "Failed to parse equation: %s", line);
    }

    count++;

    if (equation_brute_force_operations(&e)) {
      sum += e.value;
      num_valid++;
    }
  }

  fprintf(stderr, "Count: %d\n", count);
  fprintf(stderr, "Num valid: %d\n", num_valid);

  free(line);

  return sum;
}

#ifndef RUN_TESTS

int main() {
  int64_t sum = run(stdin);

  fprintf(stderr, "\n");
  fprintf(stderr, "Sum: ");
  printf("%ld\n", sum);

  return EXIT_SUCCESS;
}

#endif
