#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBERS_OF_KINDS 13

char kinds[] = "23456789TJQKA";

typedef struct Play {
  char hand[5];
  int32_t bid;
  int64_t score;
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

static int cmp_play_score(const void *a, const void *b) {
  Play *play_a = (Play *)a;
  Play *play_b = (Play *)b;

  return play_a->score - play_b->score;
}

int64_t calculate_score(char *hand) {
  int64_t score = 0;
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

  for (size_t i = 0; i < NUMBERS_OF_KINDS; i++) {
    if (counts[i] == 5) {
      is_five_of_a_kind = true;
    } else if (counts[i] == 4) {
      is_four_of_a_kind = true;
    } else if (counts[i] == 3) {
      is_three_of_a_kind = true;
    } else if (counts[i] == 2) {
      if (is_one_pair) {
        is_two_pairs = true;
      }
      is_one_pair = true;
    }
  }
  is_full_house = is_one_pair && is_three_of_a_kind;

  if (is_five_of_a_kind) {
    score |= 7 << 20;
  } else if (is_four_of_a_kind) {
    score = 6 << 20;
  } else if (is_full_house) {
    score = 5 << 20;
  } else if (is_three_of_a_kind) {
    score = 4 << 20;
  } else if (is_two_pairs) {
    score = 3 << 20;
  } else if (is_one_pair) {
    score = 2 << 20;
  } else {
    score = 1 << 20; // High card
  }

  for (size_t i = 0; i < 5; i++) {
    // Every score_from_order is 4 bits wide, to cover the 13 kinds
    int64_t score_from_order = (get_type_index(hand[i]) + 1) << (4 - i) * 4;
    assert(score_from_order < (1 << 20));
    score |= score_from_order;
  }

  return score;
}

int main(int argc, char *argv[]) {
  FILE *stream = stdin;
  char *line = NULL;
  size_t len = 1;
  ssize_t nread;

  StaticArrayPlay plays =
      (StaticArrayPlay){.arr = (Play[1000]){0}, .len = 0, .max_len = 1000};

  // Parse input
  while ((nread = getline(&line, &len, stream)) != -1) {
    Play play = {0};

    if (sscanf(line, "%c%c%c%c%c %d", &play.hand[0], &play.hand[1],
               &play.hand[2], &play.hand[3], &play.hand[4], &play.bid) != 6) {
      printf("Failed to parse input\n");
      return EXIT_FAILURE;
    }

    static_array_push(&plays, play);
  }

  // Calculate scores
  for (size_t i = 0; i < plays.len; i++) {
    Play *play = static_array_get(&plays, i);
    play->score = calculate_score(play->hand);
  }

  // Sort plays by score
  qsort(plays.arr, plays.len, sizeof(Play), cmp_play_score);

  // Calculate total winnings
  int64_t total_winnings = 0;
  for (size_t i = 0; i < plays.len; i++) {
    Play *play = static_array_get(&plays, i);
    int32_t rank = i + 1;
    total_winnings += play->bid * rank;
  }

  printf("%ld\n", total_winnings);

  return EXIT_SUCCESS;
}
