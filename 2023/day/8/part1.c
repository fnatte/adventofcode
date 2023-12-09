#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define STR_(X) #X
#define STR(X) STR_(X)
#define NODE_ID_SIZE 3
#define MAX_STEPS 100000

typedef struct NetworkNode {
  char id[NODE_ID_SIZE + 1];
  char to_l[NODE_ID_SIZE + 1];
  char to_r[NODE_ID_SIZE + 1];
} NetworkNode;

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

  HashMap map = {0};
  hashmap_init(&map, 1000);

  // Parse input
  while ((nread = getline(&line, &len, stream)) != -1) {
    char from[NODE_ID_SIZE + 1], to_l[NODE_ID_SIZE + 1], to_r[NODE_ID_SIZE + 1];
    if (sscanf(line,
               "%" STR(NODE_ID_SIZE) "s = (%" STR(NODE_ID_SIZE) "[^,], %" STR(
                   NODE_ID_SIZE) "[^)])",
               from, to_l, to_r) == 3) {
      NetworkNode *node = smalloc(sizeof(NetworkNode));
      strcpy(node->id, from);
      strcpy(node->to_l, to_l);
      strcpy(node->to_r, to_r);
      hashmap_set(&map, node->id, node);
    }
  }

  // Follow instructions
  NetworkNode *target_node = hashmap_get(&map, "ZZZ");
  NetworkNode *node = hashmap_get(&map, "AAA");
  size_t step;
  for (step = 0; node != NULL && node != target_node && step < MAX_STEPS; step++) {
    char instruction = instructions[step % instructions_len];
    if (instruction == 'L') {
      node = hashmap_get(&map, node->to_l);
    } else if (instruction == 'R') {
      node = hashmap_get(&map, node->to_r);
    } else {
      printf("Invalid instruction: %x\n", instruction);
      return EXIT_FAILURE;
    }
  }

  if (node == NULL) {
    printf("Failed to find node\n");
    return EXIT_FAILURE;
  }

  printf("%ld\n", step);

  return EXIT_SUCCESS;
}
