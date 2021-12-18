/**
 * SD, 2020
 *
 * Lab #8, Binary Tree
 *
 * Task #2 - <resume_or_paste_the_task_here>
 */

#include <stdio.h>
#include <stdlib.h>

#include "binary_tree.h"
#include "queue.h"
#include "utils.h"

void b_tree_print_nth_level(b_tree_t *b_tree, int nr_of_nodes, int n) {
  Queue *q, *qdist;
  b_node_t *b_node_tmp;
  int *dist, count;

  if (b_tree == NULL || nr_of_nodes == 0) {
    return;
  }

  dist = calloc(nr_of_nodes, sizeof(int));
  DIE(dist == NULL, "dist calloc");

  q = malloc(sizeof(Queue));
  DIE(q == NULL, "q malloc");
  qdist = malloc(sizeof(Queue));
  DIE(qdist == NULL, "qdist malloc");
  init_q(q);
  init_q(qdist);

  enqueue(q, b_tree->root);
  count = 0;
  enqueue(qdist, &dist[count]);

  while (!is_empty_q(q)) {
    b_node_tmp = (b_node_t *)front(q);
    if (*(int *)front(qdist) == n) {
      printf("%d ", *(int *)b_node_tmp->data);
    }

    if (b_node_tmp->left != NULL) {
      enqueue(q, b_node_tmp->left);
      count++;
      dist[count] = *(int *)front(qdist) + 1;
      enqueue(qdist, &dist[count]);
    }
    if (b_node_tmp->right != NULL) {
      enqueue(q, b_node_tmp->right);
      count++;
      dist[count] = *(int *)front(qdist) + 1;;
      enqueue(qdist, &dist[count]);
    }

    dequeue(q);
    dequeue(qdist);
  }
  printf("\n");

  purge_q(q);
  purge_q(qdist);
  free(q);
  free(qdist);
  free(dist);
}

int main(void) {
  b_tree_t *binary_tree;
  int i, data, N, level;

  binary_tree = b_tree_create();

  scanf("%d %d\n", &N, &level);

  for (i = 0; i < N; ++i) {
    scanf("%d ", &data);
    b_tree_insert(binary_tree, data);
  }

  b_tree_print_nth_level(binary_tree, N, level);

  b_tree_free(binary_tree);

  return 0;
}
