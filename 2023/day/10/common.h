#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Memory allocation
 */

void *smalloc(size_t size);
void *scalloc(size_t nmemb, size_t size);
void *srealloc(void *ptr, size_t size);


/*
 * Math
 */

uint64_t gcd(uint64_t a, uint64_t b);
uint64_t lcm(uint64_t a, uint64_t b);
uint64_t lcmn(uint64_t arr[], size_t n);


/*
 * Data structures
 */

// HashMap

typedef struct HashMapNode {
  char *key;
  void *value;
  struct HashMapNode *next;
} HashMapNode;

typedef struct HashMap {
  HashMapNode **node;
  size_t size;
} HashMap;

unsigned long hash(char *str);
void hashmap_init(HashMap *map, size_t size);
void hashmap_set(HashMap *map, char *key, void *value);
void *hashmap_get(HashMap *map, char *key);



// Static array (void *)

typedef struct StaticArray {
  void **arr;
  size_t len;
  size_t max_len;
} StaticArray;

void static_array_push(StaticArray *arr, void *value);
void *static_array_get(StaticArray *arr, size_t index);
void static_array_set(StaticArray *arr, size_t index, void *value);


// Static array (int64_t)

typedef struct StaticArrayInt64 {
  int64_t *arr;
  size_t len;
  size_t max_len;
} StaticArrayInt64;

void static_array_int64_push(StaticArrayInt64 *arr, int64_t value);
int64_t static_array_int64_get(StaticArrayInt64 *arr, size_t index);
void static_array_int64_set(StaticArrayInt64 *arr, size_t index, int64_t value);


