#include "common.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_static_array_push() {
  StaticArrayPlay arr = {0};
  arr.max_len = 10;
  arr.arr = calloc(sizeof(Play), arr.max_len);

  Play p1 = {0};
  Play p2 = {0};
  Play p3 = {0};

  static_array_push(&arr, p1);
  static_array_push(&arr, p2);
  static_array_push(&arr, p3);

  assert(arr.len == 3);
  assert(arr.arr[0].bid == 0);
  assert(arr.arr[1].bid == 0);
  assert(arr.arr[2].bid == 0);

  printf("test_static_array_push passed\n");
}

void test_static_array_get() {
  StaticArrayPlay arr = {0};
  arr.max_len = 10;
  arr.arr = calloc(sizeof(Play), arr.max_len);

  Play p1 = {0};
  Play p2 = {0};
  Play p3 = {0};

  static_array_push(&arr, p1);
  static_array_push(&arr, p2);
  static_array_push(&arr, p3);

  assert(arr.len == 3);
  assert(static_array_get(&arr, 0)->bid == 0);
  assert(static_array_get(&arr, 1)->bid == 0);
  assert(static_array_get(&arr, 2)->bid == 0);

  printf("test_static_array_get passed\n");
}

void test_get_type_index() {
  assert(get_type_index('J') == 0);
  assert(get_type_index('2') == 1);
  assert(get_type_index('3') == 2);
  assert(get_type_index('4') == 3);
  assert(get_type_index('5') == 4);
  assert(get_type_index('6') == 5);
  assert(get_type_index('7') == 6);
  assert(get_type_index('8') == 7);
  assert(get_type_index('9') == 8);
  assert(get_type_index('T') == 9);
  assert(get_type_index('Q') == 10);
  assert(get_type_index('K') == 11);
  assert(get_type_index('A') == 12);

  printf("test_get_type_index passed\n");
}

void test_cmp_play_score() {
  Play p1 = {0};
  Play p2 = {0};
  Play p3 = {0};

  p1.score = 1;
  p2.score = 2;
  p3.score = 3;

  assert(cmp_play_score(&p1, &p2) == -1);
  assert(cmp_play_score(&p2, &p1) == 1);
  assert(cmp_play_score(&p1, &p1) == 0);
  assert(cmp_play_score(&p2, &p3) == -1);
  assert(cmp_play_score(&p3, &p2) == 1);

  printf("test_cmp_play_score passed\n");
}

void test_cmp_play_score_big_numbers() {
  Play p1 = {0};
  Play p2 = {0};
  Play p3 = {0};
  Play p4 = {0};

  p1.score = 1000000000000000000lu;
  p2.score = 2000000000000000000lu;
  p3.score = 3000000000000000000lu;
  p4.score = 1;

  assert(cmp_play_score(&p1, &p2) == -1);
  assert(cmp_play_score(&p2, &p1) == 1);
  assert(cmp_play_score(&p1, &p1) == 0);
  assert(cmp_play_score(&p2, &p3) == -1);
  assert(cmp_play_score(&p3, &p2) == 1);
  assert(cmp_play_score(&p4, &p1) == -1);
  assert(cmp_play_score(&p1, &p4) == 1);

  printf("test_cmp_play_score_big_numbers passed\n");
}

void test_calculate_score() {
  // Nothing
  assert(calculate_score("23456") == 0x0000000000023456lu);

  // One pair
  assert(calculate_score("22345") == 0x0000000001022345lu);
  // One pair with joker
  assert(calculate_score("2345J") == 0x0000000001023451lu);

  // Two pairs
  assert(calculate_score("22334") == 0x0000000002022334lu);
  // Two pairs with joker is not possible because it would
  // a three of a kind
  assert(calculate_score("2344J") == 0x0000000003023441lu);

  // Three of a kind
  assert(calculate_score("22245") == 0x0000000003022245lu);
  // Three of a kind with joker
  assert(calculate_score("2245J") == 0x0000000003022451lu);

  // Full house
  assert(calculate_score("22233") == 0x0000000004022233lu);
  // Full house with joker
  assert(calculate_score("2233J") == 0x0000000004022331lu);

  // Four of a kind
  assert(calculate_score("22225") == 0x0000000005022225lu);
  // Four of a kind with joker
  assert(calculate_score("2225J") == 0x0000000005022251lu);

  // Five of a kind
  assert(calculate_score("22222") == 0x0000000006022222lu);
  // Five of a kind with joker
  assert(calculate_score("2222J") == 0x0000000006022221lu);

  printf("test_calculate_score passed\n");
}

int main(int argc, char* argv[]) {
  test_static_array_push();
  test_static_array_get();
  test_get_type_index();
  test_cmp_play_score();
  test_cmp_play_score_big_numbers();
  test_calculate_score();

  return EXIT_SUCCESS;
}

