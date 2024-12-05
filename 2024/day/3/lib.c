#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

/**
 * IO
 */

void buf_reader_init(buf_reader_t *r, FILE *stream, char *buf, size_t buf_cap) {
  r->stream = stream;
  r->buf = buf;
  r->buf_cap = buf_cap;
  r->buf_cur = 0;
  r->buf_len = 0;
  r->eof = false;
}

char buf_reader_getc(buf_reader_t *r) {
  assert(r->buf != NULL);
  assert(r->buf_cap > 0);

  if (r->eof) {
    return EOF;
  }

  if (r->buf_cur >= r->buf_len) {
    r->buf_len = fread(r->buf, sizeof(*(r->buf)), r->buf_cap, r->stream);
    if (r->buf_len <= 0) {
      r->eof = true;
      return EOF;
    }

    r->buf_cur = 0;
  }

  assert(r->buf_cur < r->buf_len);
  assert(r->buf_len <= r->buf_cap);

  return r->buf[r->buf_cur++];
}


/**
 * PARSING
 */
char parser_next(parser_t *p) {
  p->cur = buf_reader_getc(p->reader);
  return p->cur;
}

bool parser_eat_if(parser_t *p, char ch) {
  if (p->cur == ch) {
    parser_next(p);
    return true;
  }

  return false;
}


/**
 * DEBUG LOGGING
 */
bool debug_enabled = false;

void debug_enable() { debug_enabled = true; }

void debug_disable() { debug_enabled = false; }

void debug_printf(const char *format, ...) {
  if (!debug_enabled) {
    return;
  }

  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}
