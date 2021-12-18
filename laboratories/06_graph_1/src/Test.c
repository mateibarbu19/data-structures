#include <stdio.h>
#include <stdlib.h>

#include "ListGraph.h"
#include "MatrixGraph.h"
#include "Queue.h"
#include "Stack.h"

#define MAX_NODES 10
#define WHITE 11
#define GRAY 13
#define BLACK 17
#define INFINIT 10000000

void dfs_list_graph(ListGraph *lg, int node, int *color, int *t_desc,
                    int *t_fin) {
  int i, v, contor_timp, has_unseen_neighbors, data;
  Node *elem;
  Stack *st;
  int *node_nums;
  if (lg == NULL || lg->neighbors == NULL || color == NULL || t_desc == NULL ||
      t_fin == NULL) {
    return;
  }
  if (node < 0 || node >= lg->nodes) {
    return;
  }
  st = malloc(sizeof(Stack));
  if (st == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    return;
  }
  node_nums = malloc(lg->nodes * sizeof(int));
  if (node_nums == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    return;
  }

  init_stack(st);
  for (i = 0; i < lg->nodes; i++) {
    color[i] = WHITE;
    node_nums[i] = i;
    t_desc[i] = t_fin[i] = 0;
  }

  color[node] = GRAY;
  push_stack(st, &node_nums[node]);
  ;
  t_desc[node] = contor_timp = 0;

  while (!is_empty_stack(st)) {
    v = *(int *)peek_stack(st);
    has_unseen_neighbors = 0;
    if (lg->neighbors[v] != NULL) {
      elem = lg->neighbors[v]->head;
      while (elem != NULL && !has_unseen_neighbors) {
        data = *(int *)elem->data;
        if (color[data] == WHITE) {
          has_unseen_neighbors = 1;
          color[data] = GRAY;
          t_desc[data] = ++contor_timp;
          push_stack(st, &node_nums[data]);
        }
        elem = elem->next;
      }
    }
    if (has_unseen_neighbors == 0) {
      color[v] = BLACK;
      t_fin[v] = ++contor_timp;
      pop_stack(st);
    }
  }
  purge_stack(st);
  free(st);
  free(node_nums);
}

void dfs_matrix_graph(MatrixGraph *mg, int node, int *color, int *t_desc,
                      int *t_fin) {
  int i, v, contor_timp, has_unseen_neighbors;
  Stack *st;
  int *node_nums;
  if (mg == NULL || mg->matrix == NULL || color == NULL || t_desc == NULL ||
      t_fin == NULL) {
    return;
  }
  if (node < 0 || node >= mg->nodes) {
    return;
  }
  st = malloc(sizeof(Stack));
  if (st == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    return;
  }
  node_nums = malloc(mg->nodes * sizeof(int));
  if (node_nums == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    return;
  }

  init_stack(st);
  for (i = 0; i < mg->nodes; i++) {
    color[i] = WHITE;
    node_nums[i] = i;
    t_desc[i] = t_fin[i] = 0;
  }

  color[node] = GRAY;
  push_stack(st, &node_nums[node]);
  ;
  t_desc[node] = contor_timp = 0;

  while (!is_empty_stack(st)) {
    v = *(int *)peek_stack(st);
    has_unseen_neighbors = 0;
    for (i = 0; i < mg->nodes && !has_unseen_neighbors; i++) {
      if (mg->matrix[v][i] > 0 && color[i] == WHITE) {
        has_unseen_neighbors = 1;
        color[i] = GRAY;
        t_desc[i] = ++contor_timp;
        push_stack(st, &node_nums[i]);
      }
    }
    if (has_unseen_neighbors == 0) {
      color[v] = BLACK;
      t_fin[v] = ++contor_timp;
      pop_stack(st);
    }
  }
  purge_stack(st);
  free(st);
  free(node_nums);
}

void bfs_list_graph(ListGraph *lg, int node, int *color, int *parent) {
  int i, v, data;
  Node *elem;
  Queue *q;
  int *node_nums;
  if (lg == NULL || lg->neighbors == NULL || color == NULL || parent == NULL) {
    return;
  }
  if (node < 0 || node >= lg->nodes) {
    return;
  }
  q = malloc(sizeof(Queue));
  if (q == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    return;
  }
  node_nums = malloc(lg->nodes * sizeof(int));
  if (node_nums == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    return;
  }

  init_q(q);
  for (i = 0; i < lg->nodes; i++) {
    color[i] = WHITE;
    parent[i] = -1;
    node_nums[i] = i;
  }

  color[node] = GRAY;
  enqueue(q, &node_nums[node]);

  while (!is_empty_q(q)) {
    v = *(int *)front(q);
    dequeue(q);
    if (lg->neighbors[v] != NULL) {
      elem = lg->neighbors[v]->head;
      while (elem != NULL) {
        data = *(int *)elem->data;
        if (color[data] == WHITE) {
          color[data] = GRAY;
          parent[data] = v;
          enqueue(q, &node_nums[data]);
        }
        elem = elem->next;
      }
    }
    color[v] = BLACK;
  }
  purge_q(q);
  free(q);
  free(node_nums);
}

void bfs_matrix_graph(MatrixGraph *mg, int node, int *color, int *parent) {
  int i, v;
  Queue *q;
  int *node_nums;
  if (mg == NULL || mg->matrix == NULL || color == NULL || parent == NULL) {
    return;
  }
  if (node < 0 || node >= mg->nodes) {
    return;
  }
  q = malloc(sizeof(Queue));
  if (q == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    return;
  }
  node_nums = malloc(mg->nodes * sizeof(int));
  if (node_nums == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    return;
  }

  init_q(q);
  for (i = 0; i < mg->nodes; i++) {
    color[i] = WHITE;
    parent[i] = -1;
    node_nums[i] = i;
  }

  color[node] = GRAY;
  enqueue(q, &node_nums[node]);

  while (!is_empty_q(q)) {
    v = *(int *)front(q);
    dequeue(q);
    for (i = 0; i < mg->nodes; i++) {
      if (mg->matrix[v][i] > 0 && color[i] == WHITE) {
        color[i] = GRAY;
        parent[i] = v;
        enqueue(q, &node_nums[i]);
      }
    }
    color[v] = BLACK;
  }
  purge_q(q);
  free(q);
  free(node_nums);
}

void floyd_warshall(MatrixGraph *mg) {
  int i, j, k;
  if (mg == NULL || mg->matrix == NULL) {
    return;
  }
  int(*dist)[mg->nodes] = calloc(mg->nodes, sizeof(*dist));
  if (dist == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    return;
  }
  for (i = 0; i < mg->nodes; i++) {
    for (j = 0; j < mg->nodes; j++) {
      if (mg->matrix[i][j]) {
        dist[i][j] = mg->matrix[i][j];
      } else if (i != j) {
        dist[i][j] = INFINIT;
      }
    }
  }
  for (k = 0; k < mg->nodes; k++) {
    for (i = 0; i < mg->nodes; i++) {
      for (j = 0; j < mg->nodes; j++) {
        if (dist[i][j] > dist[i][k] + dist[k][j]) {
          dist[i][j] = dist[i][k] + dist[k][j];
        }
      }
    }
  }
  for (i = 0; i < mg->nodes; i++) {
    for (j = 0; j < mg->nodes; j++) {
      printf("%2d ", dist[i][j]);
    }
    printf("\n");
  }
  free(dist);
}

/* Afiseaza timpii de descoperire si de finalizare ai fiecarui nod din graf. */
void print_times(int *t_desc, int *t_fin, int nodes) {
  int i;
  if (t_desc == NULL || t_fin == NULL) {
    return;
  }
  for (i = 0; i < nodes; ++i) {
    printf("%d: %d/%d\n", i, t_desc[i], t_fin[i]);
  }
  printf("\n");
}

/* Afiseaza parintii nodurilor din graf. */
void print_parents(int *parents, int nodes) {
  int i;
  if (parents == NULL) {
    return;
  }
  for (i = 0; i < nodes; ++i) {
    printf("%d: %d\n", i, parents[i]);
  }
  printf("\n");
}

int main() {
  int nodes, edges;
  int x[MAX_NODES], y[MAX_NODES];
  int color[MAX_NODES], parents[MAX_NODES], t_desc[MAX_NODES], t_fin[MAX_NODES];
  ListGraph *lg = malloc(sizeof(ListGraph));
  MatrixGraph *mg = malloc(sizeof(MatrixGraph));

  scanf("%d %d", &nodes, &edges);

  init_list_graph(lg, nodes);
  init_matrix_graph(mg, nodes);

  for (int i = 0; i < edges; ++i) {
    scanf("%d %d", &x[i], &y[i]);
    add_edge_matrix_graph(mg, x[i], y[i]);
    add_edge_list_graph(lg, x[i], &y[i]);
  }

  printf("====== Graf modelat cu matrice de adiacenta ======\n");
  print_matrix_graph(mg);

  printf("====== Graf modelat cu liste de adiacenta ======\n");
  print_list_graph(lg);

  dfs_list_graph(lg, 0, color, t_desc, t_fin);
  printf("====== Timpi de descoperire si finalizare - graf modelat cu liste "
         "======\n");
  print_times(t_desc, t_fin, lg->nodes);

  dfs_matrix_graph(mg, 0, color, t_desc, t_fin);
  printf("====== Timpi de descoperire si finalizare - graf modelat cu matrice "
         "======\n");
  print_times(t_desc, t_fin, mg->nodes);

  printf("====== Parintii nodurilor - graf modelat cu matrice ======\n");
  bfs_matrix_graph(mg, 0, color, parents);
  print_parents(parents, mg->nodes);

  printf("====== Parintii nodurilor - graf modelat cu liste ======\n");
  bfs_list_graph(lg, 0, color, parents);
  print_parents(parents, lg->nodes);

  printf("====== Distantele dintre oricare doua noduri - graf modelat cu "
         "matrice ======\n");
  floyd_warshall(mg);

  clear_list_graph(lg);
  clear_matrix_graph(mg);
  free(lg);
  free(mg);
  return 0;
}
