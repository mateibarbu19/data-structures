#include <stdio.h>
#include <stdlib.h>

#include "MatrixGraph.h"

/**
 * Initializeaza graful cu numarul de noduri primit ca parametru si aloca
 * memorie pentru matricea de adiacenta a grafului.
 */
void init_matrix_graph(MatrixGraph *graph, int nodes) {
  int i, ok;
  if (graph == NULL || nodes < 0) {
    return;
  }
  /* initializam o matrice alocata dinamic patratica */
  graph->matrix = calloc(nodes, sizeof(int *));
  if (graph->matrix == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
    graph->nodes = 0;
    return;
  }
  i = 0;
  ok = 1;
  do {
    graph->matrix[i] = calloc(nodes, sizeof(int));
    if (graph->matrix[i] == NULL) {
      fprintf(stderr, "A aparut o eroare la alocarea memoriei.");
      free_nth_matrix_lines(graph->matrix, i - 1);
      free(graph->matrix);
      ok = 0;
    }
    i++;
  } while (i < nodes && ok == 1);
  if (ok == 0) {
    graph->matrix = NULL;
    graph->nodes = 0;
  } else {
    graph->nodes = nodes;
  }
}

/* Adauga o muchie intre nodurile sursa si destinatie */
void add_edge_matrix_graph(MatrixGraph *graph, int src, int dest) {
  if (graph == NULL || graph->matrix == NULL) {
    return;
  }
  if (src < 0 || dest < 0 || src >= graph->nodes || dest >= graph->nodes) {
    return;
  }
  graph->matrix[src][dest] = 1;
  graph->matrix[dest][src] = 1;
}

/* Returneaza 1 daca exista muchie intre cele doua noduri, 0 in caz contrar */
int has_edge_matrix_graph(MatrixGraph *graph, int src, int dest) {
  int ok;
  if (graph == NULL || graph->matrix == NULL) {
    return -1;
  }
  if (src < 0 || dest < 0 || src >= graph->nodes || dest >= graph->nodes) {
    return 0;
  }
  if (graph->matrix[src][dest] == graph->matrix[dest][src]) {
    ok = graph->matrix[src][dest] > 0;
  } else {
    printf("S-a produs o modificare neasteptata in graf:\nAcesta ");
    printf("nu mai este neorientat. Nu se foloseste biblioteca corect!\n");
    ok = -1;
  }
  return ok;
}

/* Elimina muchia dintre nodurile sursa si destinatie */
void remove_edge_matrix_graph(MatrixGraph *graph, int src, int dest) {
  if (graph == NULL || graph->matrix == NULL) {
    return;
  }
  if (src < 0 || dest < 0 || src >= graph->nodes || dest >= graph->nodes) {
    return;
  }
  graph->matrix[src][dest] = 0;
  graph->matrix[dest][src] = 0;
}

/* Elibereaza memoria folosita de matricea de adiacenta a grafului */
void clear_matrix_graph(MatrixGraph *graph) {
  if (graph == NULL) {
    return;
  }
  free_nth_matrix_lines(graph->matrix, graph->nodes);
  free(graph->matrix);
  graph->matrix = NULL;
  graph->nodes = 0;
}

/* Dealoca primele n linii ale unei matrici */
void free_nth_matrix_lines(int **mat, int n) {
  int i;
  if (mat == NULL) {
    return;
  }
  for (i = 0; i < n; i++) {
    free(mat[i]);
  }
}

void print_matrix_graph(MatrixGraph *g) {
  int i, j;
  if (g == NULL || g->matrix == NULL) {
    return;
  }
  printf("Numarul de noduri: %d\n", g->nodes);
  for (i = 0; i < g->nodes; i++) {
    for (j = 0; j < g->nodes; j++) {
      printf("%d ", g->matrix[i][j]);
    }
    fputc('\n', stdout);
  }
}
