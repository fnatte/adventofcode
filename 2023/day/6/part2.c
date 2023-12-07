#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// String functions

bool str_starts_with(char *str, char *prefix) {
  size_t prefix_len = strlen(prefix);
  return strncmp(str, prefix, prefix_len) == 0;
}

// Data structures

typedef struct StaticArrayUInt64 {
  int64_t *arr;
  size_t len;
  size_t max_len;
} StaticArray;

void static_array_push(StaticArray *arr, int64_t value) {
  assert(arr->len < arr->max_len);
  arr->arr[arr->len] = value;
  arr->len++;
}

int64_t static_array_get(StaticArray *arr, size_t index) {
  assert(index < arr->len);
  return arr->arr[index];
}

// Parsing functions
int64_t read_int64_ignore_other_characters(char *str) {
  char s[100];
  memset(s, '\0', 100);
  int i = 0;

  while (*str != '\n' && *str != '\0') {
    if (*str >= '0' && *str <= '9') {
      s[i] = *str;
      i++;
    }
    str++;
  }

  return atol(s);
}

int main(int argc, char *argv[]) {
  FILE *stream = stdin;
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  int64_t race_time = 0, record_distance = 0;

  // Parse input
  while ((nread = getline(&line, &len, stream)) != -1) {
    if (str_starts_with(line, "Time:")) {
      race_time = read_int64_ignore_other_characters(&line[5]);
    } else if (str_starts_with(line, "Distance:")) {
      record_distance = read_int64_ignore_other_characters(&line[9]);
    }
  }

  // Formula:
  // constants: race_time, record_distance
  // variables: charge_time
  // travel_time = race_time - charge_time
  // speed = charge_rate * charge_time
  // traveled_distance = speed * travel_time
  //                   = charge_rate * charge_time * (race_time - charge_time)
  //
  // record_distance + 1 = traveled_distance =>
  // record_distance + 1 = charge_rate * charge_time * (race_time - charge_time)
  // =>
  //
  // With charge_time = x
  //
  // (record_distance + 1) / charge_rate = race_time * x - x^2 =>
  // x^2 - race_time * x + (record_distance + 1) / charge_rate = 0
  //
  // x^2 + ax + b = 0 => {
  //   a = - race_time
  //   b = (record_distance + 1) / charge_rate
  // }
  //
  // Quadratic formula:
  // x = (-a +- sqrt(a^2 - 4b)) / 2
  //
  // charge_time = (race_time +- sqrt(race_time^2 - 4 * (record_distance + 1) /
  // charge_rate)) / 2
  //
  // Examlpe:
  // race_time = 7, record_distance = 9, charge_rate = 1 =>
  // charge_time = (7 +- sqrt(7^2 - 4 * (9 + 1) / 1)) / 2 =>
  // charge_time = (7 +- sqrt(49 - 4 * 10)) / 2 =>
  // charge_time = (7 +- sqrt(9)) / 2 =>
  // charge_time = (7 +- 3) / 2 =>
  // charge_time = 5 or charge_time = 2

  // Calculations

  const double charge_rate = 1.0; // 1 mm/s charged per second

  // Find charging times
  double race_timef = (double)race_time;
  double record_distancef = (double)record_distance;
  double charge_times[2] = {0};
  for (size_t j = 0; j < 2; j++) {
    charge_times[j] =
        (race_timef + (j == 0 ? -1.0 : 1.0) *
                          sqrt(race_timef * race_timef -
                               4.0 * (record_distancef + 1.0) / charge_rate)) /
        2.0;
  }

  double min_charge_timef =
      charge_times[0] < charge_times[1] ? charge_times[0] : charge_times[1];
  double max_charge_timef =
      charge_times[0] > charge_times[1] ? charge_times[0] : charge_times[1];

  int64_t min_charge_time = (int64_t)ceil(min_charge_timef);
  int64_t max_charge_time = (int64_t)floor(max_charge_timef);

  int64_t range = max_charge_time - min_charge_time + 1;

  printf("%ld\n", range);

  return EXIT_SUCCESS;
}
