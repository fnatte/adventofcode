#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

typedef struct {
  int32_t left;
  int32_t right;
} rule_t;

rule_t *find_matching_rule(rule_t *rules, size_t rules_len, int32_t left,
                           int32_t right) {
  for (size_t i = 0; i < rules_len; i++) {
    if (rules[i].left == left && rules[i].right == right) {
      return &rules[i];
    }
  }

  return NULL;
}

bool update_check_validity(int32_t *update, size_t update_len, rule_t *rules,
                           size_t rules_len) {
  for (size_t i = 0; i < update_len; i++) {
    for (size_t j = 0; j < update_len; j++) {
      // Skip self
      if (i == j) {
        continue;
      }

      int32_t ui = update[i];
      int32_t uj = update[j];

      // Check rules
      if (find_matching_rule(rules, rules_len, ui, uj) != NULL && j < i) {
        return false;
      }
      if (find_matching_rule(rules, rules_len, uj, ui) != NULL && i < j) {
        return false;
      }
    }
  }

  return true;
}

int32_t run(FILE *stream) {
  char *line = NULL;
  size_t len;
  ssize_t nread;

  rule_t rules[4080];
  size_t rules_len = 0;

  // Read rules
  while ((nread = getline(&line, &len, stream)) != -1 && line[0] != '\n') {
    rule_t *r = &rules[rules_len++];
    if (sscanf(line, "%d|%d", &(r->left), &(r->right)) != 2) {
      fprintf(stderr, "%d", line[0]);
      fprintf(stderr, "%s", line);
      fprintf(stderr, "Invalid rule");
      return EXIT_FAILURE;
    }
  }

  int32_t sum = 0;

  // Read updates
  while ((nread = getline(&line, &len, stream)) != -1) {
    int32_t update[64];
    size_t update_len = scan_many_int32(line, update, ",");

    for (size_t i = 0; i < update_len; i++) {
      fprintf(stderr, "%d ", update[i]);
    }

    if (update_check_validity(update, update_len, rules, rules_len)) {
      // Find middle update
      int32_t mid = update[update_len / 2];
      sum += mid;
    }

    fprintf(stderr, "\n");
  }

  free(line);

  return sum;
}

#ifndef RUN_TESTS

int main() {
  int32_t sum = run(stdin);

  fprintf(stderr, "\n");
  fprintf(stderr, "Sum: ");
  printf("%d\n", sum);

  return EXIT_SUCCESS;
}

#endif
