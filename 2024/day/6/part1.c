#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

#define BUF_SIZE 32 * 1024
#define MAX_STATE_STEPS 10000

#define GUARD_UP '^'
#define GUARD_RIGHT '>'
#define GUARD_DOWN 'v'
#define GUARD_LEFT '<'
#define TILE_EMPTY '.'
#define TILE_OBSTRUCTION '#'
#define TILE_VISITED 'X'

typedef struct {
  char *map;
  size_t dim[2];
  size_t x;
  size_t y;
  int32_t count;
  bool done;
} state_t;

bool find_starting_position(char *map, size_t *dim, size_t *x, size_t *y) {
  char *ptr = strchr(map, GUARD_UP);
  if (ptr == NULL) {
    return false;
  }

  size_t idx = ptr - map;
  str_rect_get_pos(dim[0], idx, x, y);

  return true;
}

char guard_rotate(char c) {
  switch (c) {
  case GUARD_UP:
    return GUARD_RIGHT;
  case GUARD_RIGHT:
    return GUARD_DOWN;
  case GUARD_DOWN:
    return GUARD_LEFT;
  case GUARD_LEFT:
    return GUARD_UP;
  default:
    fprintf(stderr, "Invalid guard char\n");
    exit(EXIT_FAILURE);
  }
}

bool get_next_pos(char guard, size_t x, size_t y, size_t *new_x,
                  size_t *new_y) {
  *new_x = x;
  *new_y = y;

  switch (guard) {
  case GUARD_UP:
    if (y <= 0)
      return false;
    (*new_y)--;
    break;
  case GUARD_RIGHT:
    (*new_x)++;
    break;
  case GUARD_DOWN:
    (*new_y)++;
    break;
  case GUARD_LEFT:
    if (x <= 0)
      return false;
    (*new_x)--;
    break;
  default:
    fprintf(stderr, "Invalid guard char\n");
    exit(EXIT_FAILURE);
  }

  return true;
}

void state_step(state_t *s) {
  char *guard = str_rect_at(s->map, s->dim[0], s->x, s->y);

  size_t new_x, new_y;
  bool valid_pos = get_next_pos(*guard, s->x, s->y, &new_x, &new_y);

  // Check if done
  if (!valid_pos || new_y >= s->dim[1] || new_x >= s->dim[0]) {
    s->done = true;
    *guard = TILE_VISITED;
    s->count++;
    return;
  }

  char ch_at_new = *str_rect_at(s->map, s->dim[0], new_x, new_y);
  if (ch_at_new == TILE_EMPTY || ch_at_new == TILE_VISITED) {
    // Mark old pos and increase count
    char *new_guard = str_rect_at(s->map, s->dim[0], new_x, new_y);
    *new_guard = *guard;
    if (ch_at_new != TILE_VISITED) {
      s->count++;
    }
    *guard = TILE_VISITED;

    // Move to new pos
    s->x = new_x;
    s->y = new_y;
  } else if (ch_at_new == TILE_OBSTRUCTION) {

    // Rotate
    *guard = guard_rotate(*guard);
  } else {
    fprintf(stderr, "Encountered unknown title: %c\n", ch_at_new);
    exit(EXIT_FAILURE);
  }
}

int32_t run(FILE *stream) {
  char buf[BUF_SIZE];
  state_t s = {
      .map = buf,
      .x = 0,
      .y = 0,
      .count = 0,
  };

  // Read whole stream into buf
  size_t nread = fread(buf, 1, BUF_SIZE, stream);
  assert(nread < BUF_SIZE);
  buf[++nread] = '\0';

  // Validate rectangular format of buf
  if (!str_rect_dim(buf, s.dim)) {
    printf("Invalid shape of input\n");
    exit(EXIT_FAILURE);
  }

  fprintf(stderr, "%s\n", buf);
  fprintf(stderr, "Dimensions: %ldx%ld\n", s.dim[0], s.dim[1]);

  if (!find_starting_position(s.map, s.dim, &s.x, &s.y)) {
    fprintf(stderr, "Could not find starting position\n");
  }

  fprintf(stderr, "Starting position: %ld %ld\n", s.x, s.y);

  for (int32_t i = 0; i < MAX_STATE_STEPS; i++) {
    state_step(&s);
    if (s.done) {
      fprintf(stderr, "\n%s\n", s.map);
      break;
    }
  }

  return s.count;
}

#ifndef RUN_TESTS

int main() {
  int32_t sum = run(stdin);

  fprintf(stderr, "\n");
  fprintf(stderr, "Count: ");
  printf("%d\n", sum);

  return EXIT_SUCCESS;
}

#endif
