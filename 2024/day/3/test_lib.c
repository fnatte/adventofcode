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

int main() {
  test_simple();

  return EXIT_SUCCESS;
}
