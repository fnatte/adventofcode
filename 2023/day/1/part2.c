#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *digits[] = {"zero", "one", "two",   "three", "four",
                  "five", "six", "seven", "eight", "nine"};

int parse_digit(char *line) {
  for (int i = 0; i < 10; i++) {
    if (strncmp(line, digits[i], strlen(digits[i])) == 0) {
      return i;
    }
  }

  return -1;
}

int main(int argc, char *argv[]) {

  FILE *stream = stdin;
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  int n = 0;
  uint32_t x = 0, y = 0, sum = 0;
  while ((nread = getline(&line, &len, stream)) != -1) {
    ssize_t i = 0;

    x = y = 0;

    for (i = 0; i < nread; i++) {
      if (isdigit(line[i])) {
        x = y = line[i] - '0';
        break;
      } else if ((n = parse_digit(&line[i])) != -1) {
        x = y = n;
        break;
      }
    }

    for (i = 0; i < nread; i++) {
      if (isdigit(line[i])) {
        y = line[i] - '0';
      } else if ((n = parse_digit(&line[i])) != -1) {
        y = n;
      }
    }

    sum += 10 * x + y;
  }

  printf("%u\n", sum);

  return EXIT_SUCCESS;
}
