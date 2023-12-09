#include "common.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_(X) #X
#define STR(X) STR_(X)
#define NODE_ID_SIZE 3
#define MAX_STEPS 10000000000

typedef struct NetworkNode {
  char id[NODE_ID_SIZE + 1];
  char to_l[NODE_ID_SIZE + 1];
  char to_r[NODE_ID_SIZE + 1];
  bool is_start;
  bool is_end;
  uint64_t steps_to_end;
} NetworkNode;

static bool is_start_node(NetworkNode *node) {
  size_t len = strlen(node->id);
  return node->id[len - 1] == 'A';
}

static bool is_end_node(NetworkNode *node) {
  size_t len = strlen(node->id);
  return node->id[len - 1] == 'Z';
}

static uint64_t steps_to_end(NetworkNode *node, HashMap *map,
                             char *instructions, size_t instruction_index) {
  size_t instruction_len = strlen(instructions);
  size_t step = 0;
  while (node != NULL && !node->is_end && step < MAX_STEPS) {
    char instruction =
        instructions[(instruction_index + step) % instruction_len];
    node = hashmap_get(map, instruction == 'L' ? node->to_l : node->to_r);
    step++;
  }
  assert(node != NULL);
  assert(node->is_end);
  return step;
}

int main(int argc, char *argv[]) {
  FILE *stream = stdin;
  char *line = NULL;
  size_t len = 1;
  ssize_t nread;

  char *instructions = NULL;
  if ((nread = getline(&instructions, &len, stream)) == -1) {
    printf("Failed to read instructions\n");
    return EXIT_FAILURE;
  }
  size_t instructions_len = strlen(instructions);
  // Remove newline
  assert(instructions[instructions_len - 1] == '\n');
  instructions[instructions_len - 1] = '\0';
  instructions_len--;

  NetworkNode node_pool[1000] = {0};
  size_t node_pool_index = 0;

  NetworkNode *nodes[10] = {0};
  size_t nodes_len = 0;

  HashMap map = {0};
  hashmap_init(&map, 1000);

  // Parse input
  while ((nread = getline(&line, &len, stream)) != -1) {
    char from[NODE_ID_SIZE + 1], to_l[NODE_ID_SIZE + 1], to_r[NODE_ID_SIZE + 1];
    if (sscanf(line,
               "%" STR(NODE_ID_SIZE) "s = (%" STR(NODE_ID_SIZE) "[^,], %" STR(
                   NODE_ID_SIZE) "[^)])",
               from, to_l, to_r) == 3) {
      NetworkNode *node = &node_pool[node_pool_index++];
      strcpy(node->id, from);
      strcpy(node->to_l, to_l);
      strcpy(node->to_r, to_r);
      node->is_start = is_start_node(node);
      node->is_end = is_end_node(node);
      hashmap_set(&map, node->id, node);

      if (node->is_start) {
        assert(nodes_len < 10);
        nodes[nodes_len++] = node;
      }
    }
  }

  // Calculate steps_to_end
  for (size_t i = 0; i < nodes_len; i++) {
    NetworkNode *node = nodes[i];
    node->steps_to_end = steps_to_end(node, &map, instructions, 0);
  }

  // Find lowest common muliple of steps to end
  uint64_t steps_to_end_arr[100] = {0};
  for (size_t i = 0; i < nodes_len; i++) {
    NetworkNode *node = nodes[i];
    steps_to_end_arr[i] = node->steps_to_end;
  }
  uint64_t result = lcmn(steps_to_end_arr, nodes_len);

  printf("%lu\n", result);

  return EXIT_SUCCESS;
}
