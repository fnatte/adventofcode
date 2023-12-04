#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_WINNING_NUMBERS 10
#define MAX_CARD_COPY_SLOTS 32

int main(int argc, char *argv[]) {
  FILE *stream = stdin;
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  uint32_t winning_numbers[MAX_WINNING_NUMBERS];
  size_t winning_numbers_len = 0;

  uint32_t total_card_count = 0;

  // We start with one of each card, but we only keep track of copies of cards
  // We use a circular array to keep track of the copies of cards.
  uint32_t card_copies[MAX_CARD_COPY_SLOTS] = {0};
  uint32_t card_copies_pos = 0;

  while ((nread = getline(&line, &len, stream)) != -1) {
    uint32_t matches = 0;
    uint32_t x = 0;
    int n = 0;
    char *str = line;

    winning_numbers_len = 0;

    uint32_t card_num = 0;
    if (sscanf(str, "Card %u:%n", &card_num, &n) != 1) {
      printf("Failed to read card number\n");
      return EXIT_FAILURE;
    }
    str += n;

    // Skip whitespace
    while (*str == ' ')
      str++;

    // Parse winning numbers
    while (sscanf(str, "%u%n", &x, &n) == 1) {
      str += n;

      assert(winning_numbers_len < MAX_WINNING_NUMBERS);
      winning_numbers[winning_numbers_len++] = x;

      // Skip whitespace
      while (*str == ' ')
        str++;
    }

    // Parse separator
    if (*str++ != '|') {
      printf("Failed to read separator\n");
      return EXIT_FAILURE;
    }

    // Parse our numbers
    while (sscanf(str, "%u%n", &x, &n) == 1) {
      str += n;

      for (size_t i = 0; i < winning_numbers_len; i++) {
        if (x == winning_numbers[i]) {
          matches++;
        }
      }

      // Skip whitespace
      while (*str == ' ')
        str++;
    }

    // Get reward cards
    for (size_t i = 0; i < matches; i++) {
      // Add copies to following cards, with the amount of copies of the current
      // card + the instance card
      card_copies[(i + card_copies_pos + 1) % MAX_CARD_COPY_SLOTS] +=
          card_copies[card_copies_pos] + 1;
    }

    // Add the current copies + the instance card to the total card count
    total_card_count += card_copies[card_copies_pos] + 1;

    // Clean up copies from the circular array
    card_copies[card_copies_pos] = 0;
    card_copies_pos = (card_copies_pos + 1) % MAX_CARD_COPY_SLOTS;
  }

  printf("%u\n", total_card_count);

  return EXIT_SUCCESS;
}
