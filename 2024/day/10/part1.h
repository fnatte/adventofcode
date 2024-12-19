#pragma once

#include "lib.h"
#include <stdlib.h>

typedef struct {
  size_t width;
  size_t height;
  char *data;
} map_t;

typedef struct {
  size_t x;
  size_t y;
} pos_t;

bool read_map(FILE *stream, map_t *map);
int64_t measure_trail_score(map_t *map, pos_t pos);
int64_t run(map_t *map);
