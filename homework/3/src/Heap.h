#ifndef HEAP_H_
#define HEAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEAM_NAME_LEN 30

#define GO_UP(x) (((x)-1) >> 1)
#define GO_LEFT(x) (((x) << 1) + 1)
#define GO_RIGHT(x) (((x) << 1) + 2)

typedef struct heap_t Heap;
struct heap_t {
  /* heap elements */
  void **arr;

  int size, capacity;

  /* function used for sorting the keys */
  int (*cmp)(void *key1, void *key2);
};

/**
 * Alloc memory for a new heap
 * @cmp_f: pointer to a function used for sorting
 * @return: pointer to the newly created heap
 */
Heap *init_heap(int (*cmp_f)(void *, void *));

/**
 * Insert a new element in a heap
 * @heap: the heap where to insert the new element
 * @team: the team to be inserted in heap
 */
void heap_insert(Heap *heap, void *elem);

/**
 * Get the top element
 * @heap: the heap where to search for the top element
 * @return: the top element
 */
void *heap_top(Heap *heap);

/**
 * Remove the top element
 */
void heap_pop(Heap *heap);

/**
 * Check if the heap is empty
 * @heap: the heap to be checked
 * @return: 1 if the heap is empty else 0
 */
int heap_empty(Heap *heap);

/**
 * Free a heap
 * @heap: the heap to be freed
 */
void heap_free(Heap *heap);

#endif /* HEAP_H_ */
