#pragma once

#include <stddef.h>
#include <stdint.h>

int cmp_play_score(const void *a, const void *b);

typedef struct Play {
  char hand[5];
  uint32_t bid;
  uint64_t score;
} Play;

typedef struct StaticArrayPlay {
  Play *arr;
  size_t len;
  size_t max_len;
} StaticArrayPlay;

void static_array_push(StaticArrayPlay *arr, Play value);
Play *static_array_get(StaticArrayPlay *arr, size_t index);
size_t get_type_index(char card);
uint64_t calculate_score(char *hand);
