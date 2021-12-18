#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_

struct ArrayList_t {
  int size;
  int capacity;
  void **data; /* an array of pointers to pointers! */
};
typedef struct ArrayList_t ArrayList;

ArrayList *init_array_list(void);

void add_array_nth_node(ArrayList *list, int n, void *new_data);

void free_array_list(void *p);

void purge_array_list(void *p, void (*free_data)(void *));

void quicksort_kth_nodes(ArrayList *list, int inf, int sup, int k,
                         int (*cmp)(void *, void *));

#endif /* ARRAYLIST_H_ */
