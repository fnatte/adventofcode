#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

#define MAP_SIZE 10 * 1024
#define ANTENNAS_SIZE 512
#define TILE_EMPTY '.'
#define TILE_ANTINODE '#'

typedef struct {
  char *buf;
  size_t *dim;
} map_t;

typedef struct {
  char freq;
  size_t x, y;
} antenna_t;

static inline bool is_antenna_ch(char ch) {
  return (ch != TILE_EMPTY && ch != TILE_ANTINODE);
}

static void find_antennas(map_t *map, antenna_t *antennas,
                          size_t *antennas_len) {
  for (size_t y = 0; y < map->dim[0]; y++) {
    for (size_t x = 0; x < map->dim[1]; x++) {
      char *p = str_rect_at(map->buf, map->dim[0], x, y);
      if (is_antenna_ch(*p)) {
        antennas[(*antennas_len)++] = (antenna_t){
            .freq = *p,
            .x = x,
            .y = y,
        };
      }
    }
  }
}

static int64_t mark_antinodes_for_antenna(map_t *map, antenna_t *antennas,
                                          size_t antennas_len,
                                          antenna_t *antenna) {
  antenna_t *cur = antenna;
  antenna_t *other;
  int64_t count = 0;

  for (size_t i = 0; i < antennas_len; i++) {
    other = &antennas[i];
    // Skip if same antenna
    if (cur->x == other->x && cur->y == other->y) {
      continue;
    }

    // Skip if not same frequency
    if (cur->freq != other->freq) {
      continue;
    }

    // Mark an antinode
    ssize_t dx = (ssize_t)other->x - cur->x;
    ssize_t dy = (ssize_t)other->y - cur->y;
    ssize_t dst_x = cur->x + dx * 2;
    ssize_t dst_y = cur->y + dy * 2;
    if (dst_x >= 0 && dst_x < (ssize_t)map->dim[0] && dst_y >= 0 &&
        dst_y < (ssize_t)map->dim[0]) {
      char *dst = str_rect_at(map->buf, map->dim[0], dst_x, dst_y);
      if (*dst != TILE_ANTINODE) {
        count++;
        (*dst) = TILE_ANTINODE;
      }
    }
  }

  return count;
}

static int64_t mark_antinodes_all(map_t *map, antenna_t *antennas,
                                  size_t antennas_len) {
  int64_t count = 0;
  for (size_t i = 0; i < antennas_len; i++) {
    count +=
        mark_antinodes_for_antenna(map, antennas, antennas_len, &antennas[i]);
  }

  return count;
}

static void restore_antennas(map_t *map, antenna_t *antennas,
                             size_t antennas_len) {
  for (size_t i = 0; i < antennas_len; i++) {
    char *dst =
        str_rect_at(map->buf, map->dim[0], antennas[i].x, antennas[i].y);
    *dst = antennas[i].freq;
  }
}

int64_t run(FILE *stream, char *map_buf, size_t map_len) {
  size_t nread;
  if ((nread = fread(map_buf, 1, map_len, stream)) >= map_len) {
    fprintf(stderr, "Failed to read map.\n");
    exit(EXIT_FAILURE);
  }
  map_buf[nread + 1] = '\0';

  size_t dim[2];
  if (!str_rect_dim(map_buf, dim)) {
    fprintf(stderr, "Invalid map format. Could not read dimensions.\n");
    exit(EXIT_FAILURE);
  }

  map_t map = {
      .buf = map_buf,
      .dim = dim,
  };

  fprintf(stderr, "Map dimensions: %zux%zu.\n", dim[0], dim[1]);

  antenna_t antennas[ANTENNAS_SIZE];
  size_t antennas_len = 0;
  find_antennas(&map, antennas, &antennas_len);

  fprintf(stderr, "Found %zu antennas.\n", antennas_len);

  int64_t count = mark_antinodes_all(&map, antennas, antennas_len);

  restore_antennas(&map, antennas, antennas_len);

  return count;
}

#ifndef RUN_TESTS

int main() {
  char map[MAP_SIZE];
  int64_t count = run(stdin, map, MAP_SIZE);

  fprintf(stderr, "\n");
  fprintf(stderr, "Count: ");
  printf("%ld\n", count);

  return EXIT_SUCCESS;
}

#endif
