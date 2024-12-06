#include "part2.h"
#include <stdlib.h>

rule_t example_rules[] = {
    {47, 53}, {97, 13}, {97, 61}, {97, 47}, {75, 29}, {61, 13}, {75, 53},
    {29, 13}, {97, 29}, {53, 29}, {61, 53}, {97, 53}, {61, 29}, {47, 13},
    {75, 47}, {97, 75}, {47, 61}, {75, 61}, {47, 29}, {75, 13}, {53, 13},
};

typedef struct {
  int32_t *update;
  size_t update_len;
  bool expected;
} update_test;

update_test update_tests[] = {
    {
        .update = (int32_t[]){75, 47, 61, 53, 29},
        .update_len = 5,
        .expected = true,
    },
    {
        .update = (int32_t[]){97, 61, 53, 29, 13},
        .update_len = 5,
        .expected = true,
    },
    {
        .update = (int32_t[]){75, 29, 13},
        .update_len = 3,
        .expected = true,
    },
    {
        .update = (int32_t[]){75, 97, 47, 61, 53},
        .update_len = 5,
        .expected = false,
    },
    {
        .update = (int32_t[]){61, 13, 29},
        .update_len = 3,
        .expected = false,
    },
    {
        .update = (int32_t[]){97, 13, 75, 29, 4},
        .update_len = 5,
        .expected = false,
    },
};

typedef struct {
  int32_t *update;
  int32_t *expected;
  size_t len;
} update_fix_rule_test;

update_fix_rule_test update_fix_rule_tests[] = {
    {
        .update = (int32_t[]){75, 97, 47, 61, 53},
        .expected = (int32_t[]){97, 75, 47, 61, 53},
        .len = 5,
    },
    {
        .update = (int32_t[]){61,13,29},
        .expected = (int32_t[]){61,29,13},
        .len = 3,
    },
    {
        .update = (int32_t[]){97,13,75,29,47},
        .expected = (int32_t[]){97,75,47,29,13},
        .len = 5,
    },
};

bool test_update_check_validity() {
  size_t rules_len = sizeof(example_rules) / sizeof(example_rules[0]);
  size_t update_tests_len = sizeof(update_tests) / sizeof(update_tests[0]);

  for (size_t i = 0; i < update_tests_len; i++) {
    update_test *t = &update_tests[i];
    if (update_check_validity(t->update, t->update_len, example_rules,
                              rules_len) != t->expected) {
      fprintf(stderr, "Unexpected validity result on update %ld\n", i + 1);
      return false;
    }
  }

  return true;
}

bool test_update_fix_order() {
  size_t rules_len = sizeof(example_rules) / sizeof(example_rules[0]);
  size_t update_tests_len = sizeof(update_tests) / sizeof(update_tests[0]);

  for (size_t i = 0; i < update_tests_len; i++) {
    update_fix_rule_test *t = &update_fix_rule_tests[i];
    update_fix_order(t->update, t->len, example_rules, rules_len);

    for ( size_t j = 0; j < t->len; j++) {
      if (t->update[j] != t->expected[j]) {
      fprintf(stderr, "Unexpected order on update %ld\n", i + 1);
      return false;
      }
    }
  }

  return true;
}

bool test_run() {
  FILE *stream = fopen("./example_input_part2.txt", "r");
  if (stream == NULL) {
    perror("Failed to open input file");
  }

  int32_t got = run(stream);
  int32_t expected = 123;

  if (got != expected) {
    fprintf(stderr, "Test failed: Got %d, expected %d\n", got, expected);
    return false;
  }

  return true;
}

int main() {
  if (!test_update_check_validity()) {
    return EXIT_FAILURE;
  }

  if (!test_update_fix_order()) {
    return EXIT_FAILURE;
  }

  if (!test_run()) {
    return EXIT_FAILURE;
  }

  fprintf(stderr, "All tests passed.\n");

  return EXIT_SUCCESS;
}
