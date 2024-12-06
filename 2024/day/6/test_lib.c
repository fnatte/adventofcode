#include "lib.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void test_simple() {
  char data[] = "This is a test";

  FILE *stream = fmemopen(data, strlen(data), "r");
  if (stream == NULL) {
    perror("fmemopen failed");
    exit(EXIT_FAILURE);
  }

  char buf[4];

  buf_reader_t r;
  buf_reader_init(&r, stream, buf, 4);

  for (size_t i = 0; i < strlen(data); i++) {
    char c = buf_reader_getc(&r);
    assert(c == data[i]);
  }

  char c = buf_reader_getc(&r);
  assert(c == EOF);

  fclose(stream);
}

void test_str_rect_get_idx() {
  size_t line_length = 10;
  size_t got, expected;

  got = str_rect_get_idx(line_length, 0, 0);
  expected = 0;
  if (got != expected) {
    fprintf(stderr, "str_rect_get_idx: got %zu expected %zu\n", got, expected);
    exit(EXIT_FAILURE);
  }

  got = str_rect_get_idx(line_length, 0, 1);
  expected = 11;
  if (got != expected) {
    fprintf(stderr, "str_rect_get_idx: got %zu expected %zu\n", got, expected);
    exit(EXIT_FAILURE);
  }

  got = str_rect_get_idx(line_length, 9, 1);
  expected = 20;
  if (got != expected) {
    fprintf(stderr, "str_rect_get_idx: got %zu expected %zu\n", got, expected);
    exit(EXIT_FAILURE);
  }
}

void test_str_rect_get_pos() {
  size_t line_length = 10;
  size_t x, y;
  str_rect_get_pos(line_length, 0, &x, &y);
  if (x != 0 || y != 0) {
    fprintf(stderr, "str_rect_get_pos: expected 0,0 got %zu, %zu\n", x, y);
    exit(EXIT_FAILURE);
  }

  str_rect_get_pos(line_length, 11, &x, &y);
  if (x != 0 || y != 1) {
    fprintf(stderr, "str_rect_get_pos: expected 0,1 got %zu, %zu\n", x, y);
    exit(EXIT_FAILURE);
  }

  str_rect_get_pos(line_length, 20, &x, &y);
  if (x != 9 || y != 1) {
    fprintf(stderr, "str_rect_get_pos: expected 9,1 got %zu, %zu\n", x, y);
    exit(EXIT_FAILURE);
  }
}

int main() {
  test_simple();

  test_str_rect_get_idx();
  test_str_rect_get_pos();

  return EXIT_SUCCESS;
}
