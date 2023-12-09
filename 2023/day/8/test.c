#include "common.h"
#include <assert.h>
#include <stdlib.h>

void test_hashmap_init() {
  HashMap map = {0};
  hashmap_init(&map, 1000);
  assert(map.size == 1000);
  assert(map.node != NULL);
  printf("test_hashmap_init passed\n");
}

void test_hashmap_set() {
  HashMap map = {0};
  hashmap_init(&map, 1000);

  char *key = "key";
  char *value = "value";
  hashmap_set(&map, key, value);

  assert(map.node[hash(key) % map.size] != NULL);
  assert(map.node[hash(key) % map.size]->key == key);
  assert(map.node[hash(key) % map.size]->value == value);
  assert(map.node[hash(key) % map.size]->next == NULL);

  printf("test_hashmap_set passed\n");
}

void test_hashmap_set_with_collision() {
  HashMap map = {0};
  hashmap_init(&map, 1);

  char *key1 = "key1";
  char *value1 = "value1";
  hashmap_set(&map, key1, value1);

  char *key2 = "key2";
  char *value2 = "value2";
  hashmap_set(&map, key2, value2);

  assert(map.node[hash(key1) % map.size] != NULL);
  assert(map.node[hash(key1) % map.size]->key == key2);
  assert(map.node[hash(key1) % map.size]->value == value2);
  assert(map.node[hash(key1) % map.size]->next != NULL);
  assert(map.node[hash(key1) % map.size]->next->key == key1);
  assert(map.node[hash(key1) % map.size]->next->value == value1);
  assert(map.node[hash(key1) % map.size]->next->next == NULL);

  printf("test_hashmap_set_with_collision passed\n");
}

void test_hashmap_get() {
  HashMap map = {0};
  hashmap_init(&map, 1000);

  char *key = "key";
  char *value = "value";
  hashmap_set(&map, key, value);

  assert(hashmap_get(&map, key) == value);

  printf("test_hashmap_get passed\n");
}

void test_hashmap_get_with_collision() {
  HashMap map = {0};
  hashmap_init(&map, 1);

  char *key1 = "key1";
  char *value1 = "value1";
  hashmap_set(&map, key1, value1);

  char *key2 = "key2";
  char *value2 = "value2";
  hashmap_set(&map, key2, value2);

  assert(hashmap_get(&map, key1) == value1);
  assert(hashmap_get(&map, key2) == value2);

  printf("test_hashmap_get_with_collision passed\n");
}

void test_staticarray_push() {
  StaticArray arr = (StaticArray){
    .arr = (void*[10]){0},
    .max_len = 10,
  };

  int64_t v1 = 1;
  int64_t v2 = 2;
  int64_t v3 = 3;

  static_array_push(&arr, &v1);
  static_array_push(&arr, &v2);
  static_array_push(&arr, &v3);

  assert(arr.len == 3);
  assert(*(int64_t*)arr.arr[0] == 1);
  assert(*(int64_t*)arr.arr[1] == 2);
  assert(*(int64_t*)arr.arr[2] == 3);

  printf("test_staticarray_push passed\n");
}

void test_staticarray_get() {
  StaticArray arr = (StaticArray){
    .arr = (void*[10]){0},
    .max_len = 10,
  };

  int64_t v1 = 1;
  int64_t v2 = 2;
  int64_t v3 = 3;

  static_array_push(&arr, &v1);
  static_array_push(&arr, &v2);
  static_array_push(&arr, &v3);

  assert(arr.len == 3);
  assert(*(int64_t*)static_array_get(&arr, 0) == 1);
  assert(*(int64_t*)static_array_get(&arr, 1) == 2);
  assert(*(int64_t*)static_array_get(&arr, 2) == 3);

  printf("test_staticarray_get passed\n");
}

int main(int argc, char *argv[]) {
  test_hashmap_init();
  test_hashmap_set();
  test_hashmap_set_with_collision();
  test_hashmap_get();
  test_hashmap_get_with_collision();

  test_staticarray_push();
  test_staticarray_get();

  return EXIT_SUCCESS;
}
