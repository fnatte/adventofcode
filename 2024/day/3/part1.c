#include "lib.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

bool read_num(parser_t *p, int32_t *n) {
  if (!isdigit(p->cur)) {
    return false;
  }

  (*n) = p->cur - '0';
  parser_next(p);

  if (isdigit(p->cur)) {
    (*n) = *n * 10 + p->cur - '0';
    parser_next(p);

    if (isdigit(p->cur)) {
      (*n) = (*n) * 10 + p->cur - '0';
      parser_next(p);
    }
  }

  return true;
}

bool read_mul(parser_t *p, mul *m) {
  if (parser_eat_if(p, 'm') && parser_eat_if(p, 'u') && parser_eat_if(p, 'l') &&
      parser_eat_if(p, '(')) {

    if (read_num(p, &(m->x)) && parser_eat_if(p, ',') && read_num(p, &(m->y)) &&
        parser_eat_if(p, ')')) {
      return true;
    }
  }

  return false;
}

int main() {
  // debug_enable();
  FILE *stream = stdin;

  char buf[512];
  buf_reader_t r;
  buf_reader_init(&r, stream, buf, 512);

  parser_t p = {.cur = '\0', .reader = &r};
  parser_next(&p);

  int32_t sum = 0;
  mul m;

  for (size_t n = 0; p.cur != EOF && n < 1000000; n++) {
    if (read_mul(&p, &m)) {
      debug_printf("mul(%d,%d)\n", m.x, m.y);
      sum += m.x * m.y;
    } else {
      parser_next(&p);
    }
  }
  debug_printf("\n");

  printf("%d\n", sum);

  return EXIT_SUCCESS;
}
