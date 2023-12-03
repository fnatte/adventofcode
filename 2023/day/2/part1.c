#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t color_t;

#define COLOR_RED 2
#define COLOR_GREEN 1
#define COLOR_BLUE 0

char *colors[] = {"red", "green", "blue"};

// Test for 12 red, 13 green, 14 blue
int test[] = {12, 13, 14};

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

  // Sum of possible games
  int id_sum = 0;

  while ((nread = getline(&line, &len, stream)) != -1) {
    // Example line:
    // Game 100: 7 blue, 9 green, 2 red; 5 red, 9 green; 1 blue, 8 red, 13 green
    assert(strncmp(line, "Game ", 5) == 0);
    size_t i = 5;

    // The game id is between "Game " and ":"
    uint32_t game_id;
    int pos;
    if (sscanf(&line[i], "%u%n", &game_id, &pos) != 1) {
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
      uint32_t count = 0;
      int res = 0;
      if ((res = sscanf(&line[i], "%u%n", &count, &pos)) != 1) {
        break;
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

      // Store the highest count for each color
      if (count > counts[color]) {
        counts[color] = count;
      }

      // Skip any comma, space or semicolon
      i += strspn(&line[i], ", ;");
    }

    bool possible = true;
    for (size_t i = 0; i < 3; i++) {
      if (counts[i] > test[i]) {
        possible = false;
        break;
      }
    }
    if (possible) {
      id_sum += game_id;
    }

    printf("Game %u: %u %u %u %b\n", game_id, counts[0], counts[1], counts[2],
           possible);
  }

  printf("Sum of possible game ids: %d\n", id_sum);
}
