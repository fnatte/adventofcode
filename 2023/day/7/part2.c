#include "common.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  FILE *stream = stdin;
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  StaticArrayPlay plays =
      (StaticArrayPlay){.arr = (Play[1000]){0}, .len = 0, .max_len = 1000};

  // Parse input
  while ((nread = getline(&line, &len, stream)) != -1) {
    Play play = {0};

    if (sscanf(line, "%c%c%c%c%c %u", &play.hand[0], &play.hand[1],
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
  uint64_t total_winnings = 0;
  for (size_t i = 0; i < plays.len; i++) {
    Play *play = static_array_get(&plays, i);
    uint64_t rank = i + 1;
    total_winnings += play->bid * rank;
  }

  printf("%lu\n", total_winnings);

  return EXIT_SUCCESS;
}
