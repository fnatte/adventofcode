#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


size_t scan_many_int32(char *str, int32_t *arr, char* delim) {
  char *rest = str;
  char *token;
  int32_t num;
  size_t arr_len = 0;

  while ((token = strtok_r(rest, delim, &rest))) {
    if (sscanf(token, "%d", &num) == 1) {
      arr[arr_len++] = num;
    }
  }

  return arr_len;
}


/**
 * Strings
 */

bool str_rect_dim(char *str, size_t *dim) {
  size_t len = strlen(str);
  size_t width = (strchr(str, '\n') - str);
  size_t height = 0;
  for (size_t i = 0; i < len - 1; i += width + 1) {
    char *start = &str[i];
    size_t w = (strchr(start, '\n') - start);
    if (w == 0) {
      break;
    }
    if (w != width) {
      fprintf(stderr, "Expected %zu to be %zu\n", w, width);
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

size_t str_rect_get_idx(size_t line_length, size_t x, size_t y) {
  return x + y * (1 + line_length);
}

void str_rect_get_pos(size_t line_length, size_t idx, size_t *x, size_t *y) {
  *x = idx % (1 + line_length);
  *y = idx / (1 + line_length);
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
