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

typedef struct Range {
  uint64_t from;
  uint64_t len;
} Range;

typedef struct RangeMatch {
  Range inside[2];
  Range outside[2];
} RangeMatch;

typedef struct Entry {
  uint64_t value;
  uint64_t from;
  uint64_t len;
} Entry;

typedef struct {
  Range *arr;
  size_t len;
  size_t cap;
} ArrayListRange;

void array_list_init(ArrayListRange *list, size_t cap) {
  list->arr = calloc(sizeof(Range), cap);
  list->len = 0;
  list->cap = cap;
}

void array_list_push(ArrayListRange *list, Range value) {
  if (list->len >= list->cap) {
    list->cap *= 2;
    list->arr = realloc(list->arr, sizeof(Range) * list->cap);
    for (size_t i = list->len; i < list->cap; i++) {
      list->arr[i] = (Range){0};
    }
  }

  list->arr[list->len] = value;
  list->len++;
}

Range *array_list_pop(ArrayListRange *list) {
  assert(list->len > 0);
  list->len--;
  return &list->arr[list->len];
}

Range *array_list_get(ArrayListRange *list, size_t index) {
  assert(index < list->len);
  return &list->arr[index];
}

void array_list_clear(ArrayListRange *list) { list->len = 0; }

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

static void check_range(Range r) {
  assert(r.from < UINT64_MAX - 10000000);
  assert(r.len < UINT64_MAX - 10000000);
}

// How much of a is inside b?
RangeMatch range_overlap(Range a, Range b) {
  // There are 6 cases.
  // 1. a is entirely before b
  // 2. a is entirely after b
  // 3. a is entirely inside b
  // 4. b is entirely inside a
  // 5. a and b overlap on the left
  // 6. a and b overlap on the right

  // Case 1 and 2
  if (a.from + a.len <= b.from || b.from + b.len <= a.from) {
    return (RangeMatch){
        .outside = {{a.from, a.len}, {0}},
        .inside = {{0}, {0}},
    };
  }

  // Case 3: a is entirely inside b
  if (b.from <= a.from && a.from + a.len <= b.from + b.len) {
    return (RangeMatch){
        .outside = {{0}, {0}},
        .inside = {{a.from, a.len}, {0}},
    };
  }

  // Case 4: b is entirely inside a
  if (a.from <= b.from && b.from + b.len <= a.from + a.len) {
    return (RangeMatch){
        .outside = {{a.from, b.from - a.from},
                    {b.from + b.len, a.from + a.len - b.from - b.len}},
        .inside = {{b.from, b.len}, {0}},
    };
  }

  // Case 5: a and b overlap on the left
  if (a.from <= b.from && b.from < a.from + a.len &&
      a.from + a.len < b.from + b.len) {
    return (RangeMatch){
        .outside = {{a.from, b.from - a.from}, {0}},
        .inside = {{b.from, a.from + a.len - b.from}, {0}},
    };
  }

  // Case 6: a and b overlap on the right
  if (b.from <= a.from && a.from < b.from + b.len &&
      b.from + b.len < a.from + a.len) {
    return (RangeMatch){
        .outside = {{b.from + b.len, a.from + a.len - b.from - b.len}, {0}},
        .inside = {{a.from, b.from + b.len - a.from}, {0}},
    };
  }

  assert(false);
}

ArrayListRange *lookup_range(Entry *arr, size_t arr_len, Range r) {
  ArrayListRange outside_list = {0};
  array_list_init(&outside_list, 10);
  array_list_push(&outside_list, r);

  ArrayListRange *inside_list = calloc(sizeof(ArrayListRange), 1);
  array_list_init(inside_list, 10);

  ArrayListRange more_outside_list = {0};
  array_list_init(&more_outside_list, 10);

  for (size_t i = 0; i < arr_len; i++) {
    while (outside_list.len > 0) {
      Range *outside = array_list_pop(&outside_list);

      RangeMatch match =
          range_overlap(*outside, (Range){arr[i].from, arr[i].len});
      check_range(match.inside[0]);

      if (match.inside[0].len > 0) {
        match.inside[0].from += arr[i].value - arr[i].from;
        array_list_push(inside_list, match.inside[0]);
      }
      if (match.inside[1].len > 0) {
        match.inside[1].from += arr[i].value - arr[i].from;
        array_list_push(inside_list, match.inside[1]);
      }
      if (match.outside[0].len > 0) {
        array_list_push(&more_outside_list, match.outside[0]);
      }
      if (match.outside[1].len > 0) {
        array_list_push(&more_outside_list, match.outside[1]);
      }
    }

    while (more_outside_list.len > 0) {
      Range *outside = array_list_pop(&more_outside_list);
      array_list_push(&outside_list, *outside);
    }
  }

  // Map remaining outside ranges to inside ranges (one to one)
  for (size_t i = 0; i < outside_list.len; i++) {
    Range *r = array_list_get(&outside_list, i);
    array_list_push(inside_list, *r);
  }

  return inside_list;
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

  // Seeds to ranges
  ArrayListRange *seed_list = &(ArrayListRange){0};
  array_list_init(seed_list, 10);
  for (size_t i = 0; i < seeds_len; i += 2) {
    array_list_push(seed_list, (Range){seeds[i], seeds[i + 1]});
  }

  ArrayListRange *from_list = seed_list;
  ArrayListRange *to_list = &(ArrayListRange){0};
  array_list_init(to_list, 10);

  for (size_t i = 0; i < NUMBER_OF_TYPES; i++) {

    for (size_t j = 0; j < from_list->len; j++) {
      Range *r = array_list_get(from_list, j);
      ArrayListRange *inside_list =
          lookup_range(entries[i], entries_len[i], *r);
      for (size_t k = 0; k < inside_list->len; k++) {
        Range *r = array_list_get(inside_list, k);
        array_list_push(to_list, *r);
      }
    }

    ArrayListRange *tmp = from_list;
    from_list = to_list;
    to_list = tmp;
    array_list_clear(to_list);
  }

  assert(from_list->len > 0);

  uint64_t lowest_loc = UINT64_MAX;
  for (size_t i = 0; i < from_list->len; i++) {
    Range *r = array_list_get(from_list, i);
    check_range(*r);
    if (r->from < lowest_loc) {
      lowest_loc = r->from;
    }
  }

  printf("%lu\n", lowest_loc);

  return EXIT_SUCCESS;
}
