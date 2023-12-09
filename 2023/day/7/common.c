#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define NUMBERS_OF_KINDS 13
#define SCORE_BITS_PER_CARD 4
#define SCORE_BITS_FOR_TYPE 24

char kinds[] = "J23456789TQKA";

typedef struct Play {
  char hand[5];
  uint32_t bid;
  uint64_t score;
} Play;

// Data structures

typedef struct StaticArrayPlay {
  Play *arr;
  size_t len;
  size_t max_len;
} StaticArrayPlay;

void static_array_push(StaticArrayPlay *arr, Play value) {
  assert(arr->len < arr->max_len);
  arr->arr[arr->len] = value;
  arr->len++;
}

Play *static_array_get(StaticArrayPlay *arr, size_t index) {
  assert(index < arr->len);
  return &arr->arr[index];
}

size_t get_type_index(char card) {
  for (size_t i = 0; i < NUMBERS_OF_KINDS; i++) {
    if (card == kinds[i]) {
      return i;
    }
  }

  assert(false);

  return -1;
}

int cmp_play_score(const void *a, const void *b) {
  Play *play_a = (Play *)a;
  Play *play_b = (Play *)b;

  if (play_a->score > play_b->score) {
    return 1;
  } else if (play_a->score < play_b->score) {
    return -1;
  } else {
    return 0;
  }
}

uint64_t calculate_score(char *hand) {
  uint64_t score = 0;
  bool is_five_of_a_kind = false;
  bool is_four_of_a_kind = false;
  bool is_full_house = false;
  bool is_three_of_a_kind = false;
  bool is_two_pairs = false;
  bool is_one_pair = false;

  int32_t counts[NUMBERS_OF_KINDS] = {0};
  for (size_t i = 0; i < 5; i++) {
    counts[get_type_index(hand[i])]++;
  }

  int32_t jokers = counts[0];

  for (size_t i = 1; i < NUMBERS_OF_KINDS; i++) {
    if (counts[i] >= 5 - jokers) {
      is_five_of_a_kind = true;
      int32_t jokers_used = 5 - counts[i];
      counts[i] -= 5 - jokers_used;
    }
  }

  for (size_t i = 1; i < NUMBERS_OF_KINDS; i++) {
    if (counts[i] >= 4 - jokers) {
      is_four_of_a_kind = true;
      int32_t jokers_used = 4 - counts[i];
      counts[i] -= 4 - jokers_used;
    }
  }

  for (size_t i = 1; i < NUMBERS_OF_KINDS; i++) {
    if (counts[i] >= 3 - jokers) {
      is_three_of_a_kind = true;
      int32_t jokers_used = 3 - counts[i];
      counts[i] -= 3 - jokers_used;

      assert(jokers >= jokers_used);
      jokers -= jokers_used;
    }
  }

  for (size_t i = 1; i < NUMBERS_OF_KINDS; i++) {
    if (counts[i] >= 2 - jokers) {
      int32_t jokers_used = 2 - counts[i];
      counts[i] -= 2 - jokers_used;

      assert(jokers >= jokers_used);
      jokers -= jokers_used;

      if (is_one_pair) {
        is_two_pairs = true;
      }
      is_one_pair = true;
    }
  }

  is_full_house = is_one_pair && is_three_of_a_kind;

  assert(jokers >= 0);
  assert(jokers <= 5);

  if (is_five_of_a_kind) {
    score |= 6 << SCORE_BITS_FOR_TYPE;
  } else if (is_four_of_a_kind) {
    score |= 5 << SCORE_BITS_FOR_TYPE;
  } else if (is_full_house) {
    score |= 4 << SCORE_BITS_FOR_TYPE;
  } else if (is_three_of_a_kind) {
    score |= 3 << SCORE_BITS_FOR_TYPE;
  } else if (is_two_pairs) {
    score |= 2 << SCORE_BITS_FOR_TYPE;
  } else if (is_one_pair) {
    score |= 1 << SCORE_BITS_FOR_TYPE;
  }

  for (size_t i = 0; i < 5; i++) {
    // Every score_from_order is 4 bits wide, to cover the 13 kinds
    uint64_t score_from_order = (get_type_index(hand[i]) + 1)
                                << (4 - i) * SCORE_BITS_PER_CARD;
    assert(score_from_order < (1 << SCORE_BITS_FOR_TYPE));
    score |= score_from_order;
  }

  return score;
}
