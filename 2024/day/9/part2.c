#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_SIZE 20 * 1024
#define SLOTS_SIZE 100 * 1024
#define FREE_SPACE -1

typedef struct {
  int32_t file_id;
  int32_t space;
} slot_t;

typedef struct ll_node_t {
  void *val;
  struct ll_node_t *next;
  struct ll_node_t *prev;
} ll_node_t;

void print_nodes(ll_node_t *head) {
  for (ll_node_t *n = head; n->next != NULL; n = n->next) {
    slot_t *ns = (slot_t *)n->val;
    for (int32_t i = 0; i < ns->space; i++) {
      if (ns->file_id != FREE_SPACE) {
        fprintf(stderr, "%d", ns->file_id);
      } else {
        fprintf(stderr, ".");
      }
    }
  }
  fprintf(stderr, "\n");
}

static int64_t calculate_checksum(ll_node_t *head) {
  int64_t checksum = 0;
  int64_t pos = 0;
  for (ll_node_t *n = head; n->next != NULL; n = n->next) {
    slot_t *ns = (slot_t *)n->val;
    if (ns->file_id == FREE_SPACE) {
      pos += ns->space;
      continue;
    }
    for (int32_t i = 0; i < ns->space; i++) {
      checksum += pos * ns->file_id;
      pos++;
    }
  }

  return checksum;
}

int64_t run(FILE *stream) {
  char input[INPUT_SIZE];
  size_t input_len = fread(input, sizeof(char), INPUT_SIZE, stream);
  assert(input_len < INPUT_SIZE);
  assert(input_len > 0);
  input[input_len++] = '\0';

  ll_node_t *head = NULL;
  ll_node_t *tail = NULL;
  slot_t slots[SLOTS_SIZE] = {0};
  size_t slots_len = 0;
  for (size_t i = 0; i < input_len; i++) {
    if (input[i] < '0' || input[i] > '9') {
      continue;
    }

    if (tail == NULL) {
      tail = calloc(1, sizeof(ll_node_t));
      head = tail;
    } else {
      tail->next = calloc(1, sizeof(ll_node_t));
      tail->next->prev = tail;
      tail = tail->next;
    }

    assert(slots_len < 1024 * 100);
    slot_t *s = &slots[slots_len++];
    tail->val = s;
    s->space = input[i] - '0';
    s->file_id = (i % 2 == 0) ? (int32_t)i / 2 : FREE_SPACE;
    assert(s->space >= 0);
  }

  bool match;
  do {
    match = false;
    for (ll_node_t *right = tail; !match && right->prev != NULL;
         right = right->prev) {
      slot_t *rs = (slot_t *)right->val;
      if (rs->file_id == FREE_SPACE) {
        continue;
      }
      for (ll_node_t *left = head;
           !match && left != right && left->next != NULL; left = left->next) {
        slot_t *ls = (slot_t *)left->val;
        if (ls->file_id == FREE_SPACE && ls->space > 0) {
          if (ls->space == rs->space) {
            match = true;
            // If there's a perfect match in space, we can just move the
            // whole thing without splitting
            ls->file_id = rs->file_id;
            rs->file_id = FREE_SPACE;
          } else if (ls->space > rs->space) {
            match = true;
            //  There's enough free space to move the whole right, but we
            //  need to split the left node because it has free space left.
            ll_node_t *mid = calloc(1, sizeof(ll_node_t));
            ll_node_t *tmp = left->next;
            left->next = mid;
            mid->prev = left;
            mid->next = tmp;
            tmp->prev = mid;
            assert(slots_len < SLOTS_SIZE);
            slot_t *ms = &slots[slots_len++];
            mid->val = ms;

            int32_t space_to_move = rs->space;
            int32_t free_space_left = ls->space - rs->space;
            assert(space_to_move > 0);
            assert(free_space_left > 0);

            ms->file_id = FREE_SPACE;
            ms->space = free_space_left;
            ls->file_id = rs->file_id;
            ls->space = space_to_move;
            rs->file_id = FREE_SPACE;
          }
        }
      }
    }
  } while (match);

  return calculate_checksum(head);
}

#ifndef RUN_TESTS

int main() {
  int64_t checksum = run(stdin);
  printf("%ld\n", checksum);

  return EXIT_SUCCESS;
}

#endif
