#include "common.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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

  bool pipe1_found = false;
  bool pipe2_found = false;
  Position map_end = pos(input_len - 1, line_len);

  // Check up
  if (p.y > 0) {
    size_t up_idx = idx(p.x, p.y - 1, line_len);
    if (input[up_idx] == '|' || input[up_idx] == '7' || input[up_idx] == 'F' ||
        input[up_idx] == 'S') {
      PipePosition **pipe = pipe1_found ? &pipe2 : &pipe1;
      if (pipe1_found) {
        pipe2_found = true;
      } else {
        pipe1_found = true;
      }
      **pipe = (PipePosition){.pos = {p.x, p.y - 1}, .pipe = input[up_idx]};
    }
  }

  // Check down
  if (p.y < map_end.y) {
    size_t down_idx = idx(p.x, p.y + 1, line_len);
    if (input[down_idx] == '|' || input[down_idx] == 'J' ||
        input[down_idx] == 'L' || input[down_idx] == 'S') {
      PipePosition **pipe = pipe1_found ? &pipe2 : &pipe1;
      if (pipe1_found) {
        pipe2_found = true;
      } else {
        pipe1_found = true;
      }
      **pipe = (PipePosition){.pos = {p.x, p.y + 1}, .pipe = input[down_idx]};
    }
  }

  // Check left
  if (p.x > 0) {
    size_t left_idx = idx(p.x - 1, p.y, line_len);
    if (input[left_idx] == '-' || input[left_idx] == 'F' ||
        input[left_idx] == 'L' || input[left_idx] == 'S') {
      PipePosition **pipe = pipe1_found ? &pipe2 : &pipe1;
      if (pipe1_found) {
        pipe2_found = true;
      } else {
        pipe1_found = true;
      }
      **pipe = (PipePosition){.pos = {p.x - 1, p.y}, .pipe = input[left_idx]};
    }
  }

  // Check right
  if (p.x < map_end.x) {
    size_t right_idx = idx(p.x + 1, p.y, line_len);
    if (input[right_idx] == '-' || input[right_idx] == 'J' ||
        input[right_idx] == '7' || input[right_idx] == 'S') {
      PipePosition **pipe = pipe1_found ? &pipe2 : &pipe1;
      if (pipe1_found) {
        pipe2_found = true;
      } else {
        pipe1_found = true;
      }
      **pipe = (PipePosition){.pos = {p.x + 1, p.y}, .pipe = input[right_idx]};
    }
  }

  if (pipe1_found) {
    printf("Found pipe %c at %zu, %zu\n", pipe1->pipe, pipe1->pos.x,
           pipe1->pos.y);
  }
  if (pipe2_found) {
    printf("Found pipe %c at %zu, %zu\n", pipe2->pipe, pipe2->pos.x,
           pipe2->pos.y);
  }

  if (!pipe1_found || !pipe2_found) {
    if (!pipe1_found) {
      printf("Failed to find pipe 1\n");
    }
    if (!pipe2_found) {
      printf("Failed to find pipe 2\n");
    }

    assert(false);
  }
}

Position follow(Position p, PipePosition target) {
  switch (target.pipe) {
  case '|':
    if (target.pos.y > p.y) {
      return (Position){.x = p.x, .y = p.y + 1};
    } else {
      return (Position){.x = p.x, .y = p.y - 1};
    }
  case '-':
    if (target.pos.x > p.x) {
      return (Position){.x = p.x + 1, .y = p.y};
    } else {
      return (Position){.x = p.x - 1, .y = p.y};
    }
  case 'L':
    if (target.pos.x > p.x) {
      return (Position){.x = p.x + 1, .y = p.y};
    } else {
      return (Position){.x = p.x, .y = p.y + 1};
    }
  case 'J':
    if (target.pos.x < p.x) {
      return (Position){.x = p.x - 1, .y = p.y};
    } else {
      return (Position){.x = p.x, .y = p.y + 1};
    }
  case '7':
    if (target.pos.x < p.x) {
      return (Position){.x = p.x - 1, .y = p.y};
    } else {
      return (Position){.x = p.x, .y = p.y - 1};
    }
  case 'F':
    if (target.pos.y < p.y) {
      return (Position){.x = p.x, .y = p.y - 1};
    } else {
      return (Position){.x = p.x + 1, .y = p.y};
    }
  case 'S':
    return target.pos;
  default:
    printf("Invalid pipe: %x\n", target.pipe);
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
      printf("Invalid character in input: %c\n", input[i]);
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
  PipePosition pipe[1000] = {0};
  size_t step = 0;
  pipe[0] = (PipePosition){.pos = start_pos, .pipe = 'S'};
  while (true) {
    PipePosition cur_pipe = pipe[step];
    PipePosition prev_pipe = pipe[step - 1];
    PipePosition pipe1 = {0}, pipe2 = {0};

    printf("Step %zu: %zu, %zu\n", step, cur_pipe.pos.x, cur_pipe.pos.y);

    connected_pipes(input, input_len, line_len, cur_pipe.pos, &pipe1, &pipe2);
    if (input[idx(cur_pipe.pos.x, cur_pipe.pos.y, line_len)] == 'S') {
      if (step != 0) {
        printf("Found start pipe at step %zu\n", step);
        return EXIT_FAILURE;
      }

      // Follow either pipe, so let's pick the first one
      Position next_pos = pipe1.pos;
      pipe[++step] = (PipePosition){.pos = next_pos, .pipe = pipe1.pipe};

      printf("Following pipe %c\n", pipe1.pipe);
    } else {
      // Follow the pipe that is not the one we came from
      if (pipe1.pos.x == prev_pipe.pos.x && pipe1.pos.y == prev_pipe.pos.y) {
        Position next_pos = pipe2.pos;
        pipe[++step] = (PipePosition){.pos = next_pos, .pipe = pipe2.pipe};
        printf("Following pipe %c\n", pipe2.pipe);
      } else {
        Position next_pos = pipe1.pos;
        pipe[++step] = (PipePosition){.pos = next_pos, .pipe = pipe1.pipe};
        printf("Following pipe %c\n", pipe1.pipe);
      }
    }
  }

  printf("%s\n", input);
  printf("%zu\n", start_index);

  free(input);

  return EXIT_SUCCESS;
}
