#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_SIZE 20 * 1024

int64_t run(FILE *stream) {
  char input[INPUT_SIZE];
  size_t input_len = fread(input, sizeof(char), INPUT_SIZE, stream);
  assert(input_len < INPUT_SIZE);
  assert(input_len > 0);
  input[input_len++] = '\0';

  int64_t checksum = 0;
  int32_t pos = 0;

  size_t left = 0;
  size_t right = input_len - 1;
  while (!isdigit(input[right]) && right > 0) {
    right--;
  }

  while (left <= right) {
    if (left % 2 == 0) {
      int32_t file_id = left / 2;
      while (input[left] > '0') {
        checksum += pos * file_id;
        pos++;
        input[left]--;
      }
    } else {
      while (input[left] > '0') {
        int32_t file_id = right / 2;
        while (input[right] > '0' && input[left] > '0') {
          checksum += pos * file_id;
          pos++;
          input[right]--;
          input[left]--;
        }
        if (input[right] <= '0') {
          right -= 2;
        }
      }
    }

    left++;
  }

  return checksum;
}

#ifndef RUN_TESTS

int main() {
  int64_t checksum = run(stdin);
  printf("%ld\n", checksum);

  return EXIT_SUCCESS;
}

#endif
