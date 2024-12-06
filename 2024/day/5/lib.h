#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * IO
 */

typedef struct {
  FILE *stream;
  char* buf;
  size_t buf_cur;
  size_t buf_cap;
  size_t buf_len;
  bool eof;
} buf_reader_t;

void buf_reader_init(buf_reader_t *r, FILE *stream, char *buf, size_t buf_cap);

char buf_reader_getc(buf_reader_t *r);


/**
 * PARSING
 */

typedef struct {
  int32_t x, y;
} mul;

typedef struct {
  buf_reader_t *reader;
  char cur;
} parser_t;

char parser_next(parser_t *p);
bool parser_eat_if(parser_t *p, char ch);


size_t scan_many_int32(char *str, int32_t *arr, char* delim);

/**
 * DEBUG LOGGING
 */

void debug_enable();
void debug_disable();
void debug_printf(const char *format, ...) __attribute__((format(printf, 1, 2)));
void debug_printf(const char *format, ...);

