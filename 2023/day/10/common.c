#include "common.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Memory allocation
 */

void *smalloc(size_t size) {
  void *ptr = malloc(size);
  if (ptr == NULL) {
    printf("Failed to allocate memory\n");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void *scalloc(size_t nmemb, size_t size) {
  void *ptr = calloc(nmemb, size);
  if (ptr == NULL) {
    printf("Failed to allocate memory\n");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void *srealloc(void *ptr, size_t size) {
  void *new_ptr = realloc(ptr, size);
  if (new_ptr == NULL) {
    printf("Failed to allocate memory\n");
    exit(EXIT_FAILURE);
  }
  return new_ptr;
}

/*
 * Math
 */

uint64_t gcd(uint64_t a, uint64_t b) {
  while (b != 0) {
    uint64_t temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}

uint64_t lcm(uint64_t a, uint64_t b) { return (a * b) / gcd(a, b); }

uint64_t lcmn(uint64_t arr[], size_t n) {
  uint64_t result = arr[0];
  for (size_t i = 1; i < n; i++) {
    result = lcm(result, arr[i]);
  }
  return result;
}

/*
 * Data structures
 */

// HashMap

unsigned long hash(char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;

  return hash;
}

void hashmap_init(HashMap *map, size_t size) {
  map->size = size;
  map->node = scalloc(map->size, sizeof(HashMapNode *));
}

void hashmap_set(HashMap *map, char *key, void *value) {
  size_t index = hash(key) % map->size;
  HashMapNode *node = map->node[index];

  while (node != NULL) {
    if (strcmp(node->key, key) == 0) {
      node->value = value;
      return;
    }
    node = node->next;
  }

  node = smalloc(sizeof(HashMapNode));
  node->key = key;
  node->value = value;
  node->next = map->node[index];
  map->node[index] = node;
}

void *hashmap_get(HashMap *map, char *key) {
  size_t index = hash(key) % map->size;
  HashMapNode *node = map->node[index];

  while (node != NULL) {
    if (strcmp(node->key, key) == 0) {
      return node->value;
    }
    node = node->next;
  }

  return NULL;
}

// Static array (void *)

void static_array_push(StaticArray *arr, void *value) {
  assert(arr->len < arr->max_len);
  arr->arr[arr->len] = value;
  arr->len++;
}

void *static_array_get(StaticArray *arr, size_t index) {
  assert(index < arr->len);
  return arr->arr[index];
}

void static_array_set(StaticArray *arr, size_t index, void *value) {
  assert(index < arr->len);
  arr->arr[index] = value;
}

// Static array (int64_t)
void static_array_int64_push(StaticArrayInt64 *arr, int64_t value) {
  assert(arr->len < arr->max_len);
  arr->arr[arr->len] = value;
  arr->len++;
}

int64_t static_array_int64_get(StaticArrayInt64 *arr, size_t index) {
  assert(index < arr->len);
  return arr->arr[index];
}

void static_array_int64_set(StaticArrayInt64 *arr, size_t index,
                            int64_t value) {
  assert(index < arr->len);
  arr->arr[index] = value;
}

// ArrayList (void *)

void array_list_init(ArrayList *list, size_t cap) {
  list->arr = calloc(sizeof(void *), cap);
  list->len = 0;
  list->cap = cap;
}

void array_list_push(ArrayList *list, void *value) {
  if (list->len >= list->cap) {
    list->cap *= 2;
    list->arr = realloc(list->arr, sizeof(void *) * list->cap);
    for (size_t i = list->len; i < list->cap; i++) {
      list->arr[i] = (void *){0};
    }
  }

  list->arr[list->len] = value;
  list->len++;
}

void *array_list_pop(ArrayList *list) {
  assert(list->len > 0);
  list->len--;
  return list->arr[list->len];
}

void *array_list_peak(ArrayList *list) {
  assert(list->len > 0);
  return list->arr[list->len - 1];
}

void *array_list_get(ArrayList *list, size_t index) {
  assert(index < list->len);
  return list->arr[index];
}

void array_list_clear(ArrayList *list) { list->len = 0; }

// ArrayList (size_t)

void array_list_sizet_init(ArrayListSizeT *list, size_t cap) {
  list->arr = calloc(sizeof(void *), cap);
  list->len = 0;
  list->cap = cap;
}

void array_list_sizet_push(ArrayListSizeT *list, size_t value) {
  if (list->len >= list->cap) {
    list->cap *= 2;
    list->arr = realloc(list->arr, sizeof(void *) * list->cap);
    for (size_t i = list->len; i < list->cap; i++) {
      list->arr[i] = 0;
    }
  }

  list->arr[list->len] = value;
  list->len++;
}

size_t array_list_sizet_pop(ArrayListSizeT *list) {
  assert(list->len > 0);
  list->len--;
  return list->arr[list->len];
}

size_t array_list_sizet_peak(ArrayListSizeT *list) {
  assert(list->len > 0);
  return list->arr[list->len - 1];
}

size_t array_list_sizet_get(ArrayListSizeT *list, size_t index) {
  assert(index < list->len);
  return list->arr[index];
}

void array_list_sizet_clear(ArrayListSizeT *list) { list->len = 0; }

bool array_list_sizet_contains(ArrayListSizeT *list, size_t value) {
  for (size_t i = 0; i < list->len; i++) {
    if (list->arr[i] == value) {
      return true;
    }
  }
  return false;
}

// Object pool (void)

void object_pool_init(ObjectPool *pool, size_t obj_size, size_t cap, ObjectPoolObjectInitFn create_fn) {
  pool->obj_init_fn = create_fn;
  pool->cap = cap;
  pool->len = 0;
  pool->obj_size = obj_size;
  pool->arr = scalloc(obj_size, cap);
  array_list_sizet_init(&pool->free_list, cap);
}

void *object_pool_get(ObjectPool *pool) {
  if (pool->free_list.len > 0) {
    size_t index = array_list_sizet_pop(&pool->free_list);
    return &pool->arr[index];
  }

  if (pool->len >= pool->cap) {
    pool->cap *= 2;
    pool->arr = realloc(pool->arr, pool->obj_size * pool->cap);
    for (size_t i = pool->len; i < pool->cap; i++) {
      pool->obj_init_fn(&pool->arr[i]);
    }
  }

  void *obj = &pool->arr[pool->len];
  pool->len++;
  return obj;
}

void object_pool_put(ObjectPool *pool, void *obj) {
}
