#include <stdio.h>
#include <stdlib.h>

#include "ArrayList.h"
#include "tools.h"

#define START_CAPACITY 5

ArrayList *init_array_list(void) {
  ArrayList *list = malloc(sizeof(ArrayList));
  DIE(!list, "ArrayList malloc");
  list->size = 0;
  list->capacity = START_CAPACITY;
  list->data = malloc(START_CAPACITY * sizeof(void *));
  DIE(!list->data, "ArrayList data malloc");
  return list;
}

/* Indexing is done at 0 */
void add_array_nth_node(ArrayList *list, int n, void *new_data) {
  int i;
  size_t sz;
  if (!list || n < 0 || !new_data) {
    return;
  }
  list->size++;
  if (list->size >= list->capacity) {
    sz = sizeof(void *) * list->capacity * 2;
    list->data = realloc(list->data, sz);
    DIE(!list->data, "ArrayList data realloc");
    list->capacity *= 2;
  }
  for (i = list->size; i > n; i--) {
    list->data[i] = list->data[i - 1];
  }
  list->data[n] = new_data;
}

void free_array_list(void *p) {
  ArrayList *list = (ArrayList *)p;
  if (!list) {
    return;
  }
  free(list->data);
  free(list);
}

void purge_array_list(void *p, void (*free_data)(void *)) {
  ArrayList *list = (ArrayList *)p;
  int i;
  if (!list || !free_data) {
    return;
  }
  for (i = 0; i < list->size; i++) {
    free_data(list->data);
  }
  free(list->data);
  free(list);
}

static int pivot_array_list(ArrayList *list, int inf, int sup,
                            int (*cmp)(void *, void *)) {
  int st = inf, dr = sup;
  void **v, *pivot;
  if (!list || !cmp || !list->data || inf < 0 || sup >= list->size) {
    return -1;
  }
  v = list->data;
  pivot = v[inf];

  while (st < dr) {
    while (st < dr && cmp(pivot, v[dr]) <= 0) {
      dr--;
    }
    v[st] = v[dr];
    while (st < dr && cmp(v[st], pivot) <= 0) {
      st++;
    }
    v[dr] = v[st];
  }
  v[st] = pivot;
  return st;
}

/* Indexing, including k, is done at 0 */
void quicksort_kth_nodes(ArrayList *list, int inf, int sup, int k,
                         int (*cmp)(void *, void *)) {
  int pos;
  if (!list || !list->data || !cmp) {
    return;
  }

  pos = pivot_array_list(list, inf, sup, cmp);
  if (pos == -1) {
    return;
  }
  if (pos - 1 > inf && k >= inf) {
    quicksort_kth_nodes(list, inf, pos - 1, k, cmp);
  }
  if (pos + 1 < sup && k >= pos + 1) {
    quicksort_kth_nodes(list, pos + 1, sup, k, cmp);
  }
}
