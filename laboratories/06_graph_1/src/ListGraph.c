#include <stdio.h>
#include <stdlib.h>

#include "ListGraph.h"

void init_list_graph(ListGraph *graph, int nodes) {
  if (graph == NULL || nodes < 0) {
    return;
  }
  /* initializam o matrice alocata dinamic patratica */
  graph->neighbors = calloc(nodes, sizeof(LinkedList *));
  if (graph->neighbors == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    graph->nodes = 0;
    return;
  }
  graph->nodes = nodes;
}

void add_edge_list_graph(ListGraph *graph, int src, int *dest) {
  if (graph == NULL || graph->neighbors == NULL || dest == NULL || src < 0) {
    return;
  }
  if (src >= graph->nodes || *dest < 0 || *dest >= graph->nodes) {
    return;
  }
  if (graph->neighbors[src] == NULL) {
    graph->neighbors[src] = calloc(1, sizeof(LinkedList));
    if (graph->neighbors[src] == NULL) {
      fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
      return;
    }
    init_list(graph->neighbors[src]);
  }
  add_nth_node(graph->neighbors[src], graph->neighbors[src]->size, dest);
}

int has_edge_list_graph(ListGraph *graph, int src, int dest) {
  Node *elem;
  if (graph == NULL || graph->neighbors == NULL || src < 0 || dest < 0) {
    return -1;
  }
  if (src >= graph->nodes || dest >= graph->nodes) {
    return 0;
  }
  elem = graph->neighbors[src]->head;
  while (elem != NULL && *(int *)elem->data == dest) {
    elem = elem->next;
  }
  return elem != NULL;
}

LinkedList *get_neighbours_list_graph(ListGraph *graph, int node) {
  if (graph == NULL || graph->neighbors == NULL || node < 0) {
    return NULL;
  }
  if (node >= graph->nodes) {
    return NULL;
  }
  return graph->neighbors[node];
}

void remove_edge_list_graph(ListGraph *graph, int src, int dest) {
  int i;
  Node *elem;
  if (graph == NULL || graph->neighbors == NULL || src < 0 || dest < 0) {
    return;
  }
  if (src >= graph->nodes || dest >= graph->nodes) {
    return;
  }
  i = 0;
  elem = graph->neighbors[src]->head;
  while (elem != NULL && *(int *)elem->data == dest) {
    i++;
    elem = elem->next;
  }
  if (elem != NULL) {
    remove_nth_node(graph->neighbors[src], i);
  }
}

void clear_list_graph(ListGraph *graph) {
  int i;
  if (graph == NULL) {
    return;
  }
  for (i = 0; i < graph->nodes; i++) {
    free_list(graph->neighbors + i);
  }
  free(graph->neighbors);
  graph->neighbors = NULL;
  graph->nodes = 0;
}

void print_list_graph(ListGraph *g) {
  int i;
  if (g == NULL || g->neighbors == NULL) {
    return;
  }
  printf("Numarul de noduri: %d\n", g->nodes);
  for (i = 0; i < g->nodes; i++) {
    printf("%d: ", i);
    print_int_linkedlist(g->neighbors[i]);
    if (g->neighbors[i] == NULL) {
      fputc('\n', stdout);
    }
  }
}
