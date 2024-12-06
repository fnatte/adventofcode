#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 32 * 1024

bool str_rect_dim(char *str, size_t *dim) {
  size_t len = strlen(str);
  size_t width = (strchr(str, '\n') - str);
  size_t height = 0;
  for (size_t i = 0; i < len - 1; i += width + 1) {
    char *start = &str[i];
    size_t w = (strchr(start, '\n') - start);
    if (w != width) {
      return false;
    }
    height++;
  }

  dim[0] = width;
  dim[1] = height;

  return true;
}

char *str_rect_at(char *str, size_t line_length, size_t x, size_t y) {
  return &str[x + y * (1 + line_length)];
}

int main() {
  FILE *stream = stdin;
  char buf[BUF_SIZE];

  // Read whole stream into buf
  size_t nread = fread(buf, 1, BUF_SIZE, stream);
  assert(nread < BUF_SIZE);
  buf[++nread] = '\0';

  // Validate rectangular format of buf
  size_t dim[2];
  if (!str_rect_dim(buf, dim)) {
    printf("Invalid shape of input\n");
    return EXIT_FAILURE;
  }

  fprintf(stderr, "%s\n", buf);
  fprintf(stderr, "Dimensions: %ldx%ld\n", dim[0], dim[1]);

  int32_t count = 0;

  for (size_t y = 0; y < dim[1]; y++) {
    for (size_t x = 0; x < dim[0]; x++) {
      count += (*str_rect_at(buf, dim[0], x, y) == 'A') &&
               ((*str_rect_at(buf, dim[0], x - 1, y - 1) == 'M' &&
                 *str_rect_at(buf, dim[0], x + 1, y + 1) == 'S') ||
                (*str_rect_at(buf, dim[0], x - 1, y - 1) == 'S' &&
                 *str_rect_at(buf, dim[0], x + 1, y + 1) == 'M')) &&
               ((*str_rect_at(buf, dim[0], x - 1, y + 1) == 'M' &&
                 *str_rect_at(buf, dim[0], x + 1, y - 1) == 'S') ||
                (*str_rect_at(buf, dim[0], x - 1, y + 1) == 'S' &&
                 *str_rect_at(buf, dim[0], x + 1, y - 1) == 'M'));
    }
  }

  fprintf(stderr, "Count: ");
  printf("%d\n", count);

  return EXIT_SUCCESS;
}
