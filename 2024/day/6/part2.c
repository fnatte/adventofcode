#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

#define BUF_SIZE 32 * 1024
#define MAX_STATE_STEPS 10000
#define VISITED_SIZE 1024 * 10

#define GUARD_UP '^'
#define GUARD_RIGHT '>'
#define GUARD_DOWN 'v'
#define GUARD_LEFT '<'
#define TILE_EMPTY '.'
#define TILE_OBSTRUCTION '#'
#define TILE_CUSTOM_OBSTRUCTION 'O'
#define TILE_VISITED 'X'

typedef struct {
  size_t x;
  size_t y;
  char guard;
} visit_record;

typedef struct {
  char *map;
  size_t dim[2];
  size_t x;
  size_t y;
  int32_t count;
  visit_record *visited;
  size_t visited_len;
  bool done;
  bool stuck;
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

void state_init(state_t *s) {
  // Add start position as visited
  s->visited[s->visited_len++] = (visit_record){
      .x = s->x,
      .y = s->y,
      .guard = '^',
  };
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
  if (ch_at_new == TILE_VISITED) {
    // Since we've been here before, check if stuck in a loop
    for (size_t i = 0; i < s->visited_len; i++) {
      if (s->visited[i].x == new_x && s->visited[i].y == new_y &&
          s->visited[i].guard == *guard) {
        s->done = true;
        s->stuck = true;
        return;
      }
    }
  }

  if (ch_at_new == TILE_EMPTY || ch_at_new == TILE_VISITED) {
    // Mark old pos and increase count
    char *new_guard = str_rect_at(s->map, s->dim[0], new_x, new_y);
    *new_guard = *guard;
    if (ch_at_new != TILE_VISITED) {
      s->count++;
      assert(s->visited_len < VISITED_SIZE);
      s->visited[s->visited_len++] = (visit_record){
          .x = new_x,
          .y = new_y,
          .guard = *new_guard,
      };
    }
    *guard = TILE_VISITED;

    // Move to new pos
    s->x = new_x;
    s->y = new_y;

  } else if (ch_at_new == TILE_OBSTRUCTION ||
             ch_at_new == TILE_CUSTOM_OBSTRUCTION) {
    // Rotate
    *guard = guard_rotate(*guard);

  } else {
    fprintf(stderr, "Encountered unknown title: %c (%d) at %zu %zu\n",
            ch_at_new, ch_at_new, s->x, s->y);
    exit(EXIT_FAILURE);
  }
}

void state_run(state_t *s) {
  state_init(s);
  for (int32_t i = 0; i < MAX_STATE_STEPS; i++) {
    state_step(s);
    if (s->done) {
      break;
    }
  }
}

int32_t run(FILE *stream) {
  char buf[BUF_SIZE];
  char map[BUF_SIZE];
  visit_record visited[VISITED_SIZE];

  int32_t stuck_count = 0;
  size_t start_x, start_y;
  size_t dim[2];

  // Read whole stream into buf
  size_t nread = fread(buf, 1, BUF_SIZE, stream);
  assert(nread < BUF_SIZE);
  buf[++nread] = '\0';

  // Copy buf to current map
  memcpy(map, buf, sizeof(char) * BUF_SIZE);

  // Validate rectangular format of buf
  if (!str_rect_dim(buf, dim)) {
    printf("Invalid shape of input\n");
    exit(EXIT_FAILURE);
  }

  fprintf(stderr, "%s\n", buf);
  fprintf(stderr, "Dimensions: %ldx%ld\n", dim[0], dim[1]);

  if (!find_starting_position(map, dim, &start_x, &start_y)) {
    fprintf(stderr, "Could not find starting position\n");
  }

  fprintf(stderr, "Starting position: %ld %ld\n", start_x, start_y);

  // Do normal run to get all potential solutions, i.e. the canditates
  state_t s = {
      .map = map,
      .visited = visited,
      .dim = {dim[0], dim[1]},
      .x = start_x,
      .y = start_y,
  };
  state_run(&s);
  visit_record candidates[VISITED_SIZE];
  size_t candidates_len = s.visited_len;
  memcpy(candidates, s.visited, sizeof(s.visited[0]) * s.visited_len);
  fprintf(stderr, "Got %ld candidates to check\n", candidates_len);

  // Check all candidates
  for (size_t i = 0; i < candidates_len; i++) {
    // Skip starting position
    if (candidates[i].x == start_x && candidates[i].y == start_y) {
      continue;
    }

    // Reset state
    s.x = start_x;
    s.y = start_y;
    s = (state_t){
        .map = map,
        .visited = visited,
        .dim = {dim[0], dim[1]},
        .x = start_x,
        .y = start_y,
    };
    memcpy(map, buf, sizeof(char) * BUF_SIZE);

    // Add new obstruction at candidate position
    char *cand = str_rect_at(s.map, s.dim[0], candidates[i].x, candidates[i].y);
    *cand = TILE_CUSTOM_OBSTRUCTION;

    // Do the run
    state_run(&s);

    if (s.stuck) {
      stuck_count++;
    }
  }

  return stuck_count;
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
