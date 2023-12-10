#include "common.h"
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0

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

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  char *input = read_stream_to_string(stdin);
  size_t input_len = strlen(input);
  size_t line_len = 0;

  // Validate input
  for (size_t i = 0; i < input_len; i++) {
    // Validate characters
    if (strchr(valid_map_chars, input[i]) == NULL && input[i] != '\n') {
      debug("Invalid character in input: %c\n", input[i]);
      return EXIT_FAILURE;
    }

    // Validate line length
    if (input[i] == '\n' || i == input_len - 1) {
      if (line_len == 0) {
        line_len = i;
      } else if (i == input_len - 1 ? (i + 1) % line_len
                                    : (i + 1) % (line_len + 1) != 0) {
        return EXIT_FAILURE;
      }
    }
  }

  size_t start_index = strchr(input, 'S') - input;
  Position start_pos = pos(start_index, line_len);

  // Walk the pipe
  PipePosition pipe[100000] = {0};
  size_t step = 0;
  pipe[0] = (PipePosition){.pos = start_pos, .pipe = 'S'};
  while (true) {
    PipePosition cur_pipe = pipe[step];
    PipePosition prev_pipe = pipe[step - 1];
    PipePosition pipe1 = {0}, pipe2 = {0};

    debug("Step %zu: %zu, %zu\n", step, cur_pipe.pos.x, cur_pipe.pos.y);

    connected_pipes(input, input_len, line_len, cur_pipe.pos, &pipe1, &pipe2);
    if (cur_pipe.pipe == 'S') {
      if (step != 0) {
        debug("Found start pipe at step %zu\n", step);
        break;
      }

      // Follow either pipe, so let's pick the first one
      Position next_pos = pipe1.pos;
      pipe[++step] = (PipePosition){.pos = next_pos, .pipe = pipe1.pipe};

      debug("Following pipe %c\n", pipe1.pipe);
    } else {
      // Follow the pipe that is not the one we came from
      if (pipe1.pos.x == prev_pipe.pos.x && pipe1.pos.y == prev_pipe.pos.y) {
        Position next_pos = pipe2.pos;
        pipe[++step] = (PipePosition){.pos = next_pos, .pipe = pipe2.pipe};
        debug("Following pipe %c\n", pipe2.pipe);
      } else {
        Position next_pos = pipe1.pos;
        pipe[++step] = (PipePosition){.pos = next_pos, .pipe = pipe1.pipe};
        debug("Following pipe %c\n", pipe1.pipe);
      }
    }
  }

  debug("Farthest away:");
  printf("%zu\n", step / 2);

  free(input);

  return EXIT_SUCCESS;
}
