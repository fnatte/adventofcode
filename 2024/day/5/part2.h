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

bool update_check_validity(int32_t *update, size_t update_len, rule_t *rules,
                           size_t rules_len);

void update_fix_order(int32_t *update, size_t update_len, rule_t *rules,
                      size_t rules_len);

int32_t run(FILE *stream);
