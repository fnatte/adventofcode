#include "common.h"
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG 1

void debug(char *fmt, ...) {
  (void)fmt;
#if DEBUG
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
#endif
}

typedef struct Position {
  size_t x;
  size_t y;
} Position;

typedef struct PipePosition {
  Position pos;
  char pipe;
} PipePosition;

typedef enum Direction { UP, DOWN, LEFT, RIGHT } Direction;

typedef struct Flow {
  Position pos;
  Direction dir;
} Flow;

char *valid_map_chars = "|-LJ7F.S";

char *read_stream_to_string(FILE *stream) {
  int buffer_size = 1024;
  int position = 0;
  char *buffer = smalloc(buffer_size);

  int ch;
  while ((ch = fgetc(stream)) != EOF) {
    if (position == buffer_size - 1) {
      buffer_size *= 2;
      buffer = srealloc(buffer, buffer_size);
    }

    buffer[position++] = ch;
  }

  buffer[position] = '\0';

  return buffer;
}

size_t idx(size_t x, size_t y, size_t line_len) {
  return y * (line_len + 1) + x;
}

Position pos(size_t idx, size_t line_len) {
  return (Position){.x = idx % (line_len + 1), .y = idx / (line_len + 1)};
}

bool is_vertical(char pipe) {
  return pipe == '|' || pipe == 'J' || pipe == 'L' || pipe == 'F' ||
         pipe == '7' || pipe == 'S';
}

bool is_vertical_left(char pipe) {
  return pipe == '|' || pipe == 'J' || pipe == '7' || pipe == 'S';
}

bool is_vertical_right(char pipe) {
  return pipe == '|' || pipe == 'L' || pipe == 'F' || pipe == 'S';
}

void connected_pipes(char *input, size_t input_len, size_t line_len, Position p,
                     PipePosition *pipe1, PipePosition *pipe2) {
  // Need to find two pipes, but we might find a third S-pipe which is invalid
  // only if we find two other pipes.
  PipePosition pipes[3] = {0};
  size_t pipe_count = 0;

  Position map_end = pos(input_len - 1, line_len);
  char pipe = input[idx(p.x, p.y, line_len)];

  // Check up
  if (p.y > 0 && (pipe == '|' || pipe == 'J' || pipe == 'L' || pipe == 'S')) {
    size_t up_idx = idx(p.x, p.y - 1, line_len);
    if (input[up_idx] == '|' || input[up_idx] == '7' || input[up_idx] == 'F' ||
        input[up_idx] == 'S') {
      pipes[pipe_count++] =
          (PipePosition){.pos = {p.x, p.y - 1}, .pipe = input[up_idx]};
    }
  }

  // Check down
  if (p.y < map_end.y &&
      (pipe == '|' || pipe == 'F' || pipe == '7' || pipe == 'S')) {
    size_t down_idx = idx(p.x, p.y + 1, line_len);
    if (input[down_idx] == '|' || input[down_idx] == 'J' ||
        input[down_idx] == 'L' || input[down_idx] == 'S') {
      pipes[pipe_count++] =
          (PipePosition){.pos = {p.x, p.y + 1}, .pipe = input[down_idx]};
    }
  }

  // Check left
  if (p.x > 0 && (pipe == '-' || pipe == 'J' || pipe == '7' || pipe == 'S')) {
    size_t left_idx = idx(p.x - 1, p.y, line_len);
    if (input[left_idx] == '-' || input[left_idx] == 'F' ||
        input[left_idx] == 'L' || input[left_idx] == 'S') {
      pipes[pipe_count++] =
          (PipePosition){.pos = {p.x - 1, p.y}, .pipe = input[left_idx]};
    }
  }

  // Check right
  if (p.x < map_end.x &&
      (pipe == '-' || pipe == 'F' || pipe == 'L' || pipe == 'S')) {
    size_t right_idx = idx(p.x + 1, p.y, line_len);
    if (input[right_idx] == '-' || input[right_idx] == 'J' ||
        input[right_idx] == '7' || input[right_idx] == 'S') {
      pipes[pipe_count++] =
          (PipePosition){.pos = {p.x + 1, p.y}, .pipe = input[right_idx]};
    }
  }

  if (pipe_count == 3) {
    // Remove the S-pipe
    for (size_t i = 0; i < 3; i++) {
      if (pipes[i].pipe == 'S') {
        pipes[i] = pipes[2];
        pipe_count--;
        break;
      }
    }
  }

  if (pipe_count == 2) {
    *pipe1 = pipes[0];
    *pipe2 = pipes[1];
  } else {
    debug("Found %zu pipes at %zu, %zu (%c)\n", pipe_count, p.x, p.y,
          input[idx(p.x, p.y, line_len)]);
    for (size_t i = 0; i < pipe_count; i++) {
      debug("  %c at %zu, %zu\n", pipes[i].pipe, pipes[i].pos.x,
            pipes[i].pos.y);
    }
    assert(false);
  }
}

void get_pipeline(char *input, size_t input_len, size_t line_len,
                  Position start_pos, ArrayList *pipeline) {
  PipePosition *pipe = scalloc(1, sizeof(PipePosition));
  *pipe = (PipePosition){.pos = start_pos, .pipe = 'S'};
  array_list_push(pipeline, pipe);

  while (true) {
    PipePosition *cur_pipe = array_list_peak(pipeline);
    PipePosition pipe1 = {0}, pipe2 = {0};
    debug("Step %zu: %zu, %zu\n", pipeline->len, cur_pipe->pos.x,
          cur_pipe->pos.y);

    connected_pipes(input, input_len, line_len, cur_pipe->pos, &pipe1, &pipe2);
    if (cur_pipe->pipe == 'S') {
      if (pipeline->len != 1) {
        debug("Found start pipe at step %zu\n", pipeline->len);
        break;
      }

      // Follow either pipe, so let's pick the first one
      pipe = scalloc(1, sizeof(PipePosition));
      *pipe = pipe1;
      array_list_push(pipeline, pipe);
      debug("Following pipe %c\n", pipe1.pipe);
    } else {
      // Follow the pipe that is not the one we came from
      pipe = scalloc(1, sizeof(PipePosition));
      PipePosition *prev_pipe = array_list_get(pipeline, pipeline->len - 2);
      *pipe =
          (pipe1.pos.x == prev_pipe->pos.x && pipe1.pos.y == prev_pipe->pos.y)
              ? pipe2
              : pipe1;
      array_list_push(pipeline, pipe);
      debug("Following pipe %c\n", pipe2.pipe);
    }
  }
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  char *input = read_stream_to_string(stdin);
  size_t input_len = strlen(input);
  size_t line_len = 0;
  size_t line_len_counter = 0;
  size_t lines = 0;

  // Validate input
  for (size_t i = 0; i < input_len; i++) {
    // Validate characters
    if (strchr(valid_map_chars, input[i]) == NULL && input[i] != '\n') {
      debug("Invalid character in input: %c\n", input[i]);
      return EXIT_FAILURE;
    }

    // Validate line length
    line_len_counter++;
    if (input[i] == '\n' || i == input_len - 1) {
      lines++;
      if (line_len == 0) {
        line_len = i;
      } else if ((line_len_counter - 1) % line_len != 0) {
        debug("Expected: %zu, got: %zu\n", line_len, (line_len_counter - 1));
        return EXIT_FAILURE;
      }
      line_len_counter = 0;
    }
  }

  size_t start_index = strchr(input, 'S') - input;
  Position start_pos = pos(start_index, line_len);

  // Walk the pipe
  ArrayList pipeline = {0};
  array_list_init(&pipeline, 1000);
  get_pipeline(input, input_len, line_len, start_pos, &pipeline);

  // Mark all pipeline tiles
  void *pipe_by_idx[100000] = {0};
  for (size_t i = 0; i < pipeline.len; i++) {
    PipePosition *pipe = array_list_get(&pipeline, i);
    pipe_by_idx[idx(pipe->pos.x, pipe->pos.y, line_len)] = pipe;
  }

  // Find all tiles outside the pipeline
  ArrayListSizeT outside = {0};
  array_list_sizet_init(&outside, 1000);
  ArrayListSizeT stack = {0};
  array_list_sizet_init(&stack, 1000);
  ArrayListSizeT visited = {0};
  array_list_sizet_init(&visited, 1000);

  // Check all edges
  for (size_t x = 0; x < line_len - 1; x++) {
    size_t left = idx(x, 0, line_len);
    size_t right = idx(x, lines - 1, line_len);
    if (pipe_by_idx[left] == NULL) {
      array_list_sizet_push(&stack, left);
    }
    if (pipe_by_idx[right] == NULL) {
      array_list_sizet_push(&stack, right);
    }
  }
  for (size_t y = 0; y < lines - 1; y++) {
    size_t top = idx(0, y, line_len);
    size_t bottom = idx(line_len - 1, y, line_len);
    if (pipe_by_idx[top] == NULL) {
      array_list_sizet_push(&stack, top);
    }
    if (pipe_by_idx[bottom] == NULL) {
      array_list_sizet_push(&stack, bottom);
    }
  }

  // Flood fill
  while (stack.len > 0) {
    size_t i = array_list_sizet_pop(&stack);
    if (array_list_sizet_contains(&visited, i)) {
      continue;
    }
    array_list_sizet_push(&visited, i);
    if (pipe_by_idx[i] == NULL) {
      array_list_sizet_push(&outside, i);
    }

    Position p = pos(i, line_len);
    debug("Pop %zu, %zu (%c)\n", p.x, p.y, input[i]);

    if (p.x > 0) {
      // Left
      size_t left_idx = idx(p.x - 1, p.y, line_len);
      if (pipe_by_idx[left_idx] == NULL) {
        array_list_sizet_push(&stack, left_idx);
        debug("Push %zu, %zu (%c) (left)\n", p.x - 1, p.y, input[left_idx]);
      }
      // Check squeeze
      // else if (input[left_idx] == '-' &&
      //          ((p.y > 0 && input[idx(p.x - 1, p.y - 1, line_len)] == '-') ||
      //           (p.y < lines - 1 &&
      //            input[idx(p.x - 1, p.y + 1, line_len)] == '-'))

      // ) {
      //   array_list_sizet_push(&stack, left_idx);
      // }
    }
    // Right
    if (p.x < line_len - 1) {
      size_t right_idx = idx(p.x + 1, p.y, line_len);
      if (pipe_by_idx[right_idx] == NULL) {
        array_list_sizet_push(&stack, right_idx);
        debug("Push %zu, %zu (%c) (right)\n", p.x + 1, p.y, input[right_idx]);
      }
    }
    // Up
    if (p.y > 0) {
      size_t up_idx = idx(p.x, p.y - 1, line_len);
      if (pipe_by_idx[up_idx] == NULL) {
        array_list_sizet_push(&stack, up_idx);
        debug("Push %zu, %zu (%c) (up)\n", p.x, p.y - 1, input[up_idx]);
      }
      // Check squeeze
      else if ((p.x > 0 && is_vertical_right(input[up_idx]) &&
                is_vertical_left(input[idx(p.x - 1, p.y - 1, line_len)])) ||
               (p.x < line_len - 1 && is_vertical_left(input[up_idx]) &&
                is_vertical_right(input[idx(p.x + 1, p.y - 1, line_len)]))

      ) {
        printf("Squeeze at %zu, %zu (%c) (up)\n", p.x, p.y - 1, input[up_idx]);
        array_list_sizet_push(&stack, up_idx);
      }
    }
    // Down
    if (p.y < lines - 1) {
      size_t down_idx = idx(p.x, p.y + 1, line_len);
      if (pipe_by_idx[down_idx] == NULL) {
        array_list_sizet_push(&stack, down_idx);
        debug("Push %zu, %zu (%c) (down)\n", p.x, p.y + 1, input[down_idx]);
      }
    }
  }

  printf("%s\n", input);

  char *output = scalloc(input_len + 1, sizeof(char));
  memcpy(output, input, input_len);
  for (size_t i = 0; i < outside.len; i++) {
    output[outside.arr[i]] = 'O';
  }
  printf("%s", output);

  free(input);

  return EXIT_SUCCESS;
}
