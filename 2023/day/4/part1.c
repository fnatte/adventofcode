#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_WINNING_NUMBERS 10

int main(int argc, char *argv[]) {
  FILE *stream = stdin;
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  uint32_t winning_numbers[MAX_WINNING_NUMBERS];
  size_t winning_numbers_len = 0;
  uint32_t score = 0;

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

    score += matches > 0 ? (1 << (matches - 1)) : 0;
  }

  printf("%u\n", score);

  return EXIT_SUCCESS;
}
