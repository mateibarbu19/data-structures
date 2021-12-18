#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ListGraph.h"
#include "Queue.h"
#include "Stack.h"

#define MAX_NODES 100

#define UNSEEN 0
#define SEEN 1
#define VISITED 2

#define WHITE 0
#define GRAY 1
#define BLACK 2

#define RED 4
#define BLUE (~RED)

void dfs_connected_comps(ListGraph *lg, int *node, int *visited,
                         LinkedList *component) {
  int top, has_unseen_neighbors, data;
  Node *elem;
  Stack *st;
  if (lg == NULL || lg->neighbors == NULL || node == NULL || visited == NULL) {
    return;
  }
  if (component == NULL || *node < 0 || *node >= lg->nodes) {
    return;
  }
  st = malloc(sizeof(Stack));
  if (st == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    return;
  }

  init_stack(st);
  visited[*node] = SEEN;
  push_stack(st, node);

  while (!is_empty_stack(st)) {
    top = *(int *)peek_stack(st);
    has_unseen_neighbors = 0;

    if (lg->neighbors[top] != NULL) {
      elem = lg->neighbors[top]->head;
      while (elem != NULL && !has_unseen_neighbors) {
        data = *(int *)elem->data;
        if (visited[data] == UNSEEN) {
          has_unseen_neighbors = 1;
          visited[data] = SEEN;
          push_stack(st, elem->data);
        }
        elem = elem->next;
      }
    }

    if (has_unseen_neighbors == 0) {
      visited[top] = VISITED;
      add_nth_node(component, component->size, peek_stack(st));
      pop_stack(st);
    }
  }

  purge_stack(st);
  free(st);
}

void connected_components(ListGraph *lg, int *nodes_index) {
  int *visited, i, nr_comp;
  LinkedList **components;
  if (lg == NULL || lg->neighbors == NULL || nodes_index == NULL) {
    return;
  }
  visited = calloc(lg->nodes, sizeof(int));
  if (visited == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    return;
  }
  components = calloc(lg->nodes, sizeof(LinkedList *));
  if (components == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    return;
  }
  for (i = 0; i < lg->nodes; i++) {
    visited[nodes_index[i]] = UNSEEN;
  }

  nr_comp = 0;
  for (i = 0; i < lg->nodes; i++) {
    if (visited[nodes_index[i]] == UNSEEN) {
      components[nr_comp] = calloc(1, sizeof(LinkedList));
      init_list(components[nr_comp]);
      dfs_connected_comps(lg, &nodes_index[i], visited, components[nr_comp]);
      nr_comp++;
    }
  }
  printf("%d\n", nr_comp);
  for (i = 0; i < nr_comp; i++) {
    print_int_linkedlist(components[i]);
    free_list(components + i);
  }
  free(components);
  free(visited);
}

void dfs_topo_sort(ListGraph *lg, int *node, int *visited, LinkedList *result) {
  /* TODO */
}

void topo_sort(ListGraph *lg, int *nodes_index) { /* TODO */
}

void bfs_list_graph(ListGraph *lg, int *node, int *color, int *parent) {
  int vertex, data;
  Node *elem;
  Queue *q;
  if (lg == NULL || lg->neighbors == NULL || color == NULL || parent == NULL) {
    return;
  }
  if (*node < 0 || *node >= lg->nodes) {
    return;
  }
  q = malloc(sizeof(Queue));
  if (q == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    return;
  }

  init_q(q);

  color[*node] = GRAY;
  enqueue(q, node);

  while (!is_empty_q(q)) {
    vertex = *(int *)front(q);
    dequeue(q);
    if (lg->neighbors[vertex] != NULL) {
      elem = lg->neighbors[vertex]->head;
      while (elem != NULL) {
        data = *(int *)elem->data;
        if (color[data] == WHITE) {
          color[data] = GRAY;
          parent[data] = vertex;
          enqueue(q, elem->data);
        }
        elem = elem->next;
      }
    }
    color[vertex] = BLACK;
  }
  purge_q(q);
  free(q);
}

void min_path(ListGraph *lg, int *nodes_index, int *color) {
  int i, A, B;
  int *parent;
  Stack *st;

  if (lg == NULL || nodes_index == NULL || color == NULL) {
    return;
  }

  st = malloc(sizeof(Stack));
  if (st == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    return;
  }
  init_stack(st);

  parent = calloc(lg->nodes, sizeof(int));
  if (parent == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    return;
  }

  for (i = 0; i < lg->nodes; ++i) {
    parent[i] = -1;
    color[i] = WHITE;
  }

  scanf("%d %d", &A, &B);
  bfs_list_graph(lg, &nodes_index[A], color, parent);
  i = B;
  while (i != -1) {
    push_stack(st, &nodes_index[i]);
    i = parent[i];
  }
  printf("Output pr. #3:\n");
  while (!is_empty_stack(st)) {
    printf("%d ", *(int*)peek_stack(st));
    pop_stack(st);
  }
  fputc('\n', stdout);
  purge_stack(st);
  free(st);
  free(parent);
}

int check_bipartite(ListGraph *lg, int *nodes_index, int *color) {
  int i, val, ok, data;
  Node *elem;
  LinkedList *first, *second;
  Queue *q;
  if (lg == NULL || lg->neighbors == NULL || nodes_index == NULL || color == NULL) {
    return -1;
  }
  q = malloc(sizeof(Queue));
  if (q == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    return -1;
  }
  first = calloc(1, sizeof(LinkedList));
  if (first == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    free(q);
    return -1;
  }
  second = calloc(1, sizeof(LinkedList));
  if (second == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    free(q);
    free_list(&first);
    return -1;
  }
  init_q(q);
  init_list(first);
  init_list(second);

  for (i = 0; i < lg->nodes; i++) {
    color[i] = WHITE;
  }

  ok = 1;
  i = 0;
  while (ok && i < lg->nodes) {
    if (color[i] == WHITE) {
      enqueue(q, &nodes_index[i]);
      color[nodes_index[i]] = RED;
    }
    while (!is_empty_q(q)) {
      val = *(int*)front(q);
      dequeue(q);
      if (color[val] == RED) {
        add_nth_node(first, first->size, &nodes_index[val]);
      }
      else if (color[val] == BLUE) {
        add_nth_node(second, second->size, &nodes_index[val]);
      }

      if (lg->neighbors[val] != NULL) {
        elem = lg->neighbors[val]->head;
        while (ok && elem != NULL) {
          data = *(int *)elem->data;
          if (color[data] == WHITE) {
            color[data] = ~color[val];
            enqueue(q, elem->data);
          }
          else if (color[data] == color[val]) {
            ok = 0;
          }
          elem = elem->next;
        }
      }
    }
    i++;
  }

  printf("Output pr. #4:\n");
  if (ok) {
    print_int_linkedlist(first);
    print_int_linkedlist(second);
  }
  else {
    printf("Graful nu este bipartit.\n");
  }
  free_list(&first);
  free_list(&second);
  purge_q(q);
  free(q);
  return ok;
}

int main() {
  int nodes, edges, i, t;
  int color[MAX_NODES];
  int nodes_index[MAX_NODES];
  int x[MAX_NODES], y[MAX_NODES];
  ListGraph *lg = malloc(sizeof(ListGraph));

  /** Ex 1 **/
  printf("Problema #1. Introduceti datele de intrare:\n");
  scanf("%d %d", &nodes, &edges);
  init_list_graph(lg, nodes);
  for (i = 0; i < nodes; ++i) {
    nodes_index[i] = i;
  }

  for (i = 0; i < edges; ++i) {
    scanf("%d %d", &x[i], &y[i]);
    add_edge_list_graph(lg, x[i], &y[i]);
    add_edge_list_graph(lg, y[i], &x[i]);
  }

  printf("Output pr. #1:\n");
  connected_components(lg, nodes_index);
  clear_list_graph(lg);

  /** Ex 3 **/
  printf("\nProblema #3. Introduceti datele de intrare:\n");
  scanf("%d %d %d", &nodes, &edges, &t);
  /* Obs: nu este necesara folosirea varaibile t pentru calcule corecte */

  init_list_graph(lg, nodes);

  for (i = 0; i < edges; ++i) {
    scanf("%d %d", &x[i], &y[i]);
    add_edge_list_graph(lg, x[i], &y[i]);
    add_edge_list_graph(lg, y[i], &x[i]);
  }

  min_path(lg, nodes_index, color);
  clear_list_graph(lg);

  /** Ex 4 **/
  printf("\nProblema #4. Introduceti datele de intrare:\n");
  scanf("%d %d", &nodes, &edges);
  init_list_graph(lg, nodes);

  for (i = 0; i < edges; ++i) {
    scanf("%d %d", &x[i], &y[i]);
    add_edge_list_graph(lg, x[i], &y[i]);
    add_edge_list_graph(lg, y[i], &x[i]);
  }

  check_bipartite(lg, nodes_index, color);
  clear_list_graph(lg);

  free(lg);
  return 0;
}
