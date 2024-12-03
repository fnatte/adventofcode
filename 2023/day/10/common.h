#pragma once

#include <stdbool.h>
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


// ArrayList (void *)

typedef struct {
  void **arr;
  size_t len;
  size_t cap;
} ArrayList;

void array_list_init(ArrayList *list, size_t cap);
void array_list_push(ArrayList *list, void* value);
void *array_list_pop(ArrayList *list);
void *array_list_peak(ArrayList *list);
void *array_list_get(ArrayList *list, size_t index);
void array_list_clear(ArrayList *list);

// ArrayList (size_t)

typedef struct {
  size_t *arr;
  size_t len;
  size_t cap;
} ArrayListSizeT;

void array_list_sizet_init(ArrayListSizeT *list, size_t cap);
void array_list_sizet_push(ArrayListSizeT *list, size_t value);
size_t array_list_sizet_pop(ArrayListSizeT *list);
size_t array_list_sizet_peak(ArrayListSizeT *list);
size_t array_list_sizet_get(ArrayListSizeT *list, size_t index);
void array_list_sizet_clear(ArrayListSizeT *list);
bool array_list_sizet_contains(ArrayListSizeT *list, size_t value);

// Object pool (void *)

typedef void (*ObjectPoolObjectInitFn)(void* obj);

typedef struct {
  void *arr;
  size_t len;
  size_t cap;
  size_t obj_size;
  ObjectPoolObjectInitFn obj_init_fn;
  ArrayListSizeT free_list;
} ObjectPool;


void object_pool_init(ObjectPool *pool, size_t obj_size, size_t cap, ObjectPoolObjectInitFn obj_init_fn);
void *object_pool_get(ObjectPool *pool);
void object_pool_put(ObjectPool *pool, void *obj);

