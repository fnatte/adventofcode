#include "common.h"
#include <assert.h>
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

uint64_t lcm(uint64_t a, uint64_t b) {
    return (a * b) / gcd(a, b);
}

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

void static_array_int64_set(StaticArrayInt64 *arr, size_t index, int64_t value) {
  assert(index < arr->len);
  arr->arr[index] = value;
}
