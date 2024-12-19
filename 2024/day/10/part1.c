#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib.h"
#include "part1.h"

char *read_file_to_string(FILE *stream) {
  assert(stream != NULL);

  // Get file size by seeking to end and check position
  int seek_end_res = fseek(stream, 0, SEEK_END);
  assert(seek_end_res == 0);
  long len = ftell(stream);
  assert(len >= 0);

  char *str = malloc(len + 1); // +1 for null terminator
  assert(str != NULL);

  // Seek to beginning again and read stream into str
  rewind(stream);
  size_t nread = fread(str, sizeof(char), len, stream);
  if (nread != (size_t)len) {
    free(str);
    fprintf(stderr, "Expected nread (%zu) to be len (%ld)\n", nread, len);
    exit(EXIT_FAILURE);
  }

  str[len] = '\0';
  return str;
}

char *read_all_stream(FILE *stream) {
  size_t capacity = 1024;
  size_t length = 0;
  char *buffer = malloc(capacity);
  if (!buffer)
    return NULL;

  while (!feof(stream) && !ferror(stream)) {
    // If there's not enough space, grow the buffer
    if (length + 512 >= capacity) {
      capacity *= 2;
      char *new_buffer = realloc(buffer, capacity);
      if (!new_buffer) {
        free(buffer);
        return NULL;
      }
      buffer = new_buffer;
    }

    size_t to_read = capacity - length - 1; // Leave space for null terminator
    size_t n = fread(buffer + length, 1, to_read, stream);
    length += n;
    if (n == 0) {
      // Either EOF or error
      break;
    }
  }

  buffer[length] = '\0';
  return buffer;
}

bool read_map(FILE *stream, map_t *map) {

  map->data = read_all_stream(stream);

  size_t dim[2];
  if (!str_rect_dim(map->data, dim)) {
    return false;
  }

  map->width = dim[0];
  map->height = dim[1];

  return true;
}

static inline char map_at(map_t *map, pos_t pos) {
  return *str_rect_at(map->data, map->width, pos.x, pos.y);
}

static inline pos_t pos_left(pos_t pos) {
  assert(pos.x > 0);
  return (pos_t){.x = pos.x - 1, .y = pos.y};
}

static inline pos_t pos_right(pos_t pos) {
  return (pos_t){.x = pos.x + 1, .y = pos.y};
}

static inline pos_t pos_up(pos_t pos) {
  assert(pos.y > 0);
  return (pos_t){.x = pos.x, .y = pos.y - 1};
}

static inline pos_t pos_down(pos_t pos) {
  return (pos_t){.x = pos.x, .y = pos.y + 1};
}

static inline bool pos_equals(pos_t a, pos_t b) {
  return a.x == b.x && a.y == b.y;
}

int64_t measure_trail_score(map_t *map, pos_t pos) {
  int64_t score = 0;

  // depth first search
  pos_t visited[1024];
  size_t visited_len = 0;
  pos_t stack[1024];
  size_t stack_len = 0;

  pos_t cur;
  char cur_c;

  stack[stack_len++] = pos;
  // fprintf(stderr, "Check (%ld, %ld)\n", pos.x, pos.y);

  while (stack_len > 0) {
    cur = stack[--stack_len];
    cur_c = map_at(map, cur);

    // Skip if we've visited this pos before
    bool has_visited = false;
    for (size_t i = 0; i < visited_len; i++) {
      if (pos_equals(visited[i], cur)) {
        has_visited = true;
        break;
      }
    }
    if (has_visited) {
      continue;
    }

    // Mark as visited
    visited[visited_len++] = cur;

    // Check if we reached the top
    if (cur_c == '9') {
      // fprintf(stderr, "Found at (%ld, %ld)\n", cur.x, cur.y);
      score++;
      continue;
    }

    // Check left
    if (cur.x > 0 && map_at(map, pos_left(cur)) - cur_c == 1) {
      stack[stack_len++] = pos_left(cur);
    }

    // Check right
    if (cur.x < map->width && map_at(map, pos_right(cur)) - cur_c == 1) {
      stack[stack_len++] = pos_right(cur);
    }

    // Check up
    if (cur.y > 0 && map_at(map, pos_up(cur)) - cur_c == 1) {
      stack[stack_len++] = pos_up(cur);
    }

    // Check down
    if (cur.y < map->height && map_at(map, pos_down(cur)) - cur_c == 1) {
      stack[stack_len++] = pos_down(cur);
    }
  }

  return score;
}

int64_t run(map_t *map) {
  // fprintf(stderr, "\n");
  size_t sum = 0;
  for (size_t y = 0; y < map->height; y++) {
    for (size_t x = 0; x < map->width; x++) {
      pos_t pos = (pos_t){.x = x, .y = y};
      char c = map_at(map, pos);

      // If this is a trailhead, check the score.
      if (c == '0') {
        int64_t score = measure_trail_score(map, pos);
        // fprintf(stderr, "Score %ld\n", score);
        sum += score;
      }
    }
  }
  // fprintf(stderr, "\n");

  return sum;
}

#ifndef RUN_TESTS

int main() {
  map_t map = {0};
  if (!read_map(stdin, &map)) {
    fprintf(stderr, "Failed to read map\n");
    return false;
  }

  int64_t sum = run(&map);
  printf("%ld\n", sum);

  free(map.data);

  return EXIT_SUCCESS;
}

#endif
