#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t color_t;

#define COLOR_BLUE 0
#define COLOR_GREEN 1
#define COLOR_RED 2

char *colors[] = {"blue", "green", "red"};

bool parse_color(char *line, color_t *color, size_t *pos) {
  for (size_t i = 0; i < 3; i++) {
    if (strncmp(line, colors[i], strlen(colors[i])) == 0) {
      *color = i;
      *pos = strlen(colors[i]);
      return true;
    }
  }

  return false;
}

int main(int argc, char *argv[]) {
  FILE *stream = stdin;
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  while ((nread = getline(&line, &len, stream)) != -1) {
    // Example line:
    // Game 100: 7 blue, 9 green, 2 red; 5 red, 9 green; 1 blue, 8 red, 13 green
    assert(strncmp(line, "Game ", 5) == 0);
    size_t i = 5;

    // The game id is between "Game " and ":"
    uint32_t game_id;
    int pos;
    if (sscanf(&line[i], "%" SCNu32 "%n", &game_id, &pos) != 1) {
      printf("Failed to parse game id\n");
      exit(EXIT_FAILURE);
    }
    i += pos;

    // Skip the ": "
    assert(len >= (size_t)(i + 1));
    assert(line[i] == ':');
    assert(line[i + 1] == ' ');
    i += 2;

    uint32_t counts[3] = {0};

    while (i < (size_t)nread) {

      // Read the count
      uint32_t count;
      if (sscanf(&line[i], "%" SCNu32 "%n", &count, &pos) != 1) {
        printf("Failed to parse count\n");
        exit(EXIT_FAILURE);
      }
      i += pos;

      // Skip the " "
      assert(line[i] == ' ');
      i++;

      // Read the color
      color_t color;
      size_t color_pos;
      if (!parse_color(&line[i], &color, &color_pos)) {
        printf("Failed to parse color\n");
        exit(EXIT_FAILURE);
      }
      i += color_pos;
      counts[color] = count;

      // Skip any comma, space or semicolon
      i += strspn(&line[i], ", ;");
    }

    printf("Game %" PRIu32 ": %u %u %u\n", game_id, counts[0], counts[1],
           counts[2]);
  }
}
