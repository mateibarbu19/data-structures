/* Ex 2 */
topo_sort(lg, nodes_index);

/* Ex 3 */
min_path(lg, nodes_index);

/* Ex 4 */
if (check_bipartite(lg, nodes_index, color)) {
  for (int i = 0; i < lg->nodes; ++i) {
    if (color[i] == 1) {
      printf("%d ", i);
    }
  }
  printf("\n");
  for (int i = 0; i < lg->nodes; ++i) {
    if (color[i] == 0) {
      printf("%d ", i);
    }
  }
  printf("\n");
} else {
  printf("The graph is not bipartite\n");
}
