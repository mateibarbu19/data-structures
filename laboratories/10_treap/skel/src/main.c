#include "treap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 1000

int compar(void *value1, void *value2) {

  int *a = (int *)value1;
  int *b = (int *)value2;
  if ((*a) > (*b)) {
    return 1;
  } else if ((*a) < (*b)) {
    return -1;
  }
  return 0;
}

/* Obtinerea cheilor sortate crescator.
 *
 * @param1: Nodul curent in parcurgerea recursiva.
 * @param2: Array-ul prin care se intorc cheile sortate crescator.
 * @param3: Numarul de chei adaugate in array.
 */
void ascending_nodes(Node *node, int *keys, int *num_keys) {
  if (!node || !keys || !num_keys) {
    return;
  }
  ascending_nodes(node->left, keys, num_keys);
  keys[*num_keys] = *(int *)node->data;
  (*num_keys)++;
  ascending_nodes(node->right, keys, num_keys);
}

/*void debug(Node *node) {
  if (!node) {
    return;
  }
  Node *q[MAX_NODES];
  int nr = 0, h[MAX_NODES], start, stop, tata[MAX_NODES];
  char fiu[MAX_NODES];
  q[0] = node;
  h[0] = 0;
  start = 0;
  stop = 1;
  while (start < stop) {
    node = q[start];
    printf("Level %d: %d %d %d %c\n", h[start], *(int *)node->data,
node->priority, tata[start], fiu[start]); if (node->left) { q[stop] =
node->left; h[stop] = h[start] + 1; tata[stop] = *(int *)node->data; fiu[stop] =
'l'; stop++;
    }
    if (node->right) {
      q[stop] = node->right;
      h[stop] = h[start] + 1;
      tata[stop] = *(int *)node->data;
      fiu[stop] = 'r';
      stop++;
    }
    start++;
  }
}*/

int main() {
  Treap_tree *tree = treap_create(compar);

  int task, no_inserts, no_deletes, no_finds;
  int value, i;
  int keys[MAX_NODES];
  int num_keys;

  scanf("%d", &task);
  scanf("%d", &no_inserts);

  if (task == 1) {
    printf("%s\n", "------------- TASK 1 -------------");
  } else if (task == 2) {
    printf("%s\n", "------------- TASK 2 -------------");
  } else if (task == 3) {
    printf("%s\n", "------------- TASK 3 -------------");
  } else {
    printf("%s\n", "------------- TASK 4 -------------");
  }

  /* ------------- TASK 1 ------------- */
  for (i = 1; i <= no_inserts; ++i) {
    scanf("%d", &value);
    treap_insert(&tree->root, &value, sizeof(int), tree->compar);
    /* Used for checker. */
    if (task == 1) {
      if (check_treap(tree->root, tree->compar) == i) {
        printf("Corect insertion. Treap has %d nodes.\n",
               check_treap(tree->root, tree->compar));
      } else {
        printf("%s\n", "Wrong insertion.");
      }
    }
  }

  /* ------------- TASK 2 ------------ */
  if (task >= 2) {
    scanf("%d", &no_finds);
    for (i = 1; i <= no_finds; ++i) {
      scanf("%d", &value);
      if (task == 2) {
        if (get_key(tree->root, &value, sizeof(int), tree->compar)) {
          printf("%d %s\n", value, "is in Treap.");
        } else {
          printf("%d %s\n", value, "is NOT in Treap.");
        }
      }
    }
  }

  /* ------------- TASK 3 ------------- */
  if (task >= 3) {
    scanf("%d", &no_deletes);
    for (i = 1; i <= no_deletes; ++i) {
      scanf("%d", &value);
      if (task == 3) {
        treap_delete(&tree->root, &value, sizeof(int), tree->compar);
        /* Used for checker. */
        if (1) {
          printf("Corect deletion. Treap has %d nodes.\n",
                 check_treap(tree->root, tree->compar));
        } else {
          printf("%s\n", "Wrong deletion.");
        }
      }
    }
  }

  /* ------------- TASK 4 ------------- */
  if (task == 4) {
    memset(keys, 0, sizeof(int) * MAX_NODES);
    num_keys = 0;
    ascending_nodes(tree->root, keys, &num_keys);
    printf("Ascending keys: ");
    for (i = 0; i < num_keys; ++i) {
      printf("%d ", keys[i]);
    }
    printf("\n");
  }
  treap_free(tree);
  return 0;
}
