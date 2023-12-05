#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEEDS 20
#define NUMBER_OF_TYPES 7
#define MAX_ENTRIES_PER_TYPE 50

typedef enum {
  SEED_TO_SOIL = 0,
  SOIL_TO_FERTILIZER,
  FERTILIZER_TO_WATER,
  WATER_TO_LIGHT,
  LIGHT_TO_TEMPERATURE,
  TEMPERATURE_TO_HUMIDITY,
  HUMIDITY_TO_LOCATION
} MapType;

typedef struct Entry {
  uint64_t value;
  uint64_t from;
  uint64_t len;
} Entry;

bool read_many_uint64(char *line, uint64_t *arr, size_t *arr_len,
                  size_t arr_max_len) {
  uint64_t num = 0;
  int n = 0;
  char *str = line;

  // Skip initial whitespace
  while (*str == ' ')
    str++;

  while (sscanf(str, "%lu%n", &num, &n) == 1) {
    str += n;

    assert((*arr_len) < arr_max_len);
    arr[*arr_len] = num;
    (*arr_len)++;

    // Skip whitespace
    while (*str == ' ')
      str++;
  }

  return true;
}

bool str_starts_with(char *str, char *prefix) {
  size_t prefix_len = strlen(prefix);
  return strncmp(str, prefix, prefix_len) == 0;
}

uint64_t lookup(Entry *arr, size_t arr_len, uint64_t key) {
  for (size_t i = 0; i < arr_len; i++) {
    if (arr[i].from <= key && key <= arr[i].from + arr[i].len) {
      return arr[i].value + (key - arr[i].from);
    }
  }

  // If it's not in a range, the value is equal to the key
  return key;
}

uint64_t lookup_location(Entry entries[][MAX_ENTRIES_PER_TYPE],
                         size_t *entries_len, uint64_t key) {
  for (size_t i = 0; i < NUMBER_OF_TYPES; i++) {
    key = lookup(entries[i], entries_len[i], key);
  }
  return key;
}

int main(int argc, char *argv[]) {
  FILE *stream = stdin;
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  uint64_t seeds[MAX_SEEDS];
  size_t seeds_len = 0;

  Entry entries[NUMBER_OF_TYPES][MAX_ENTRIES_PER_TYPE] = {0};
  size_t entries_len[NUMBER_OF_TYPES] = {0};

  char *map_names[NUMBER_OF_TYPES] = {
      "seed-to-soil map:",         "soil-to-fertilizer map:",
      "fertilizer-to-water map:",  "water-to-light map:",
      "light-to-temperature map:", "temperature-to-humidity map:",
      "humidity-to-location map:"};

  Entry *cur_entries = NULL;
  size_t *cur_entries_len = 0;

  while ((nread = getline(&line, &len, stream)) != -1) {

    if (str_starts_with(line, "seeds:")) {
      read_many_uint64(line + 6, seeds, &seeds_len, MAX_SEEDS);
      continue;
    }

    for (size_t i = 0; i < NUMBER_OF_TYPES; i++) {
      if (str_starts_with(line, map_names[i])) {
        cur_entries = entries[i];
        cur_entries_len = &entries_len[i];
        goto outer_continue;
      }
    }

    if (cur_entries != NULL) {
      uint64_t nums[3] = {0};
      size_t nums_len = 0;
      read_many_uint64(line, nums, &nums_len, 3);

      if (nums_len != 3) {
        continue;
      }

      assert((*cur_entries_len) < MAX_ENTRIES_PER_TYPE);

      cur_entries[*cur_entries_len].value = nums[0];
      cur_entries[*cur_entries_len].from = nums[1];
      cur_entries[*cur_entries_len].len = nums[2];
      (*cur_entries_len)++;
    }

  outer_continue:;
  }

  // Find lowest location number
  uint64_t lowest_loc = UINT64_MAX;
  for (size_t i = 0; i < seeds_len; i++) {
    uint64_t loc = lookup_location(entries, entries_len, seeds[i]);
    if (loc < lowest_loc) {
      lowest_loc = loc;
    }
  }

  printf("%lu\n", lowest_loc);

  return EXIT_SUCCESS;
}
