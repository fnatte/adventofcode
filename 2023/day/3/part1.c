#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_WIDTH 150

int main(int argc, char *argv[]) {
  FILE *stream = stdin;

  char *prev_line = malloc(LINE_WIDTH * sizeof(char));
  char *cur_line = malloc(LINE_WIDTH * sizeof(char));
  char *next_line = malloc(LINE_WIDTH * sizeof(char));
  char *tmp;
  size_t len = 0;
  ssize_t prev_nread = 0, cur_nread = 0, next_nread = 0;

  memset(prev_line, '\0', LINE_WIDTH);
  memset(cur_line, '\0', LINE_WIDTH);
  memset(next_line, '\0', LINE_WIDTH);

  uint32_t part_sum = 0;

  if ((next_nread = getline(&next_line, &len, stream)) == -1) {
    printf("Failed to read line\n");
    return EXIT_FAILURE;
  }

  while (next_nread != -1) {
    tmp = prev_line;
    prev_line = cur_line;
    prev_nread = cur_nread;
    cur_line = next_line;
    cur_nread = next_nread;
    next_line = tmp;
    next_nread = getline(&next_line, &len, stream);
    if (next_nread == -1) {
      memset(next_line, '\0', LINE_WIDTH);
    }

    for (size_t i = 0; i < (size_t)cur_nread; i++) {
      if (cur_line[i] >= '0' && cur_line[i] <= '9') {
        // Read following digits and convert to number
        uint32_t num = 0;
        int pos = 0;
        if (sscanf(&cur_line[i], "%u%n", &num, &pos) != 1) {
          printf("Failed to read number\n");
          return EXIT_FAILURE;
        }

        // Check for adjacent symbols
        bool adjacent = false;
        char symbol = '\0';
        // Check left side of current line
        if (i > 0 && cur_line[i - 1] != '.') {
          adjacent = true;
          symbol = cur_line[i - 1];
        }
        // Check right side of current line
        else if (i + pos < (size_t)cur_nread && cur_line[i + pos] != '.' &&
                 cur_line[i + pos] != '\n') {
          adjacent = true;
          symbol = cur_line[i + pos];
        } else {
          // Check previous and next line
          for (ssize_t j = i - 1; j <= (ssize_t)(i + pos); j++) {
            if (j >= 0 && ((j < prev_nread && prev_line[j] != '.' &&
                            prev_line[j] != '\n') ||
                           (j < next_nread && next_line[j] != '.' &&
                            next_line[j] != '\n'))) {
              adjacent = true;
              symbol =
                  j < prev_nread && prev_line[j] != '.' && prev_line[j] != '\n'
                      ? prev_line[j]
                      : next_line[j];
              break;
            }
          }
        }

        if (adjacent) {
          assert(symbol == '*' || symbol == '+' || symbol == '-' ||
                 symbol == '/' || symbol == '%' || symbol == '=' ||
                 symbol == '#' || symbol == '&' || symbol == '|' ||
                 symbol == '$' || symbol == '@');
          part_sum += num;
        }

        i += pos - 1;
      }
    }
  }

  printf("%u\n", part_sum);

  free(prev_line);
  free(cur_line);
  free(next_line);

  return EXIT_SUCCESS;
}
