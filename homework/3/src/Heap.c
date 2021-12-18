#include "Heap.h"
#include "tools.h"

Heap *init_heap(int (*cmp_f)(void *, void *)) {
  Heap *heap;
  if (cmp_f == NULL) {
    return NULL;
  }

  heap = malloc(sizeof(Heap));
  DIE(heap == NULL, "Heap malloc");

  heap->cmp = cmp_f;
  heap->size = 0;
  heap->capacity = 2;
  heap->arr = malloc(heap->capacity * sizeof(void *));
  DIE(heap->arr == NULL, "Heap arr malloc");

  return heap;
}

static void __heap_insert_fix(Heap *heap, int pos) {
  void *tmp;
  int p = GO_UP(pos);
  if (!heap || !heap->arr || !heap->cmp || p >= heap->size) {
    return;
  }

  while (pos != 0 && heap->cmp(heap->arr[p], heap->arr[pos]) > 0) {
    tmp = heap->arr[p];
    heap->arr[p] = heap->arr[pos];
    heap->arr[pos] = tmp;

    pos = p;
    p = GO_UP(pos);
  }
}

void heap_insert(Heap *heap, void *elem) {
  if (!heap || !heap->arr || !heap->cmp || elem == NULL) {
    return;
  }

  heap->arr[heap->size] = elem;
  __heap_insert_fix(heap, heap->size);

  heap->size++;
  if (heap->size == heap->capacity) {
    heap->capacity *= 2;
    heap->arr = realloc(heap->arr, heap->capacity * sizeof(void *));
    DIE(heap->arr == NULL, "heap->arr realloc");
  }
}

void *heap_top(Heap *heap) {
  if (heap == NULL || heap->arr == NULL) {
    return NULL;
  }
  return heap->arr[0];
}

static void __heap_pop_fix(Heap *heap, int pos) {
  void *tmp;
  /* int p = pos; */
  int l = GO_LEFT(pos);
  int r = GO_RIGHT(pos);
  int cmp_l, cmp_r, pos_move;
  int ok = 0;

  if (!heap || !heap->arr || !heap->cmp || l >= heap->size) {
    return;
  }
  while (l < heap->size && ok == 0) {
    ok = 1;
    cmp_l = heap->cmp(heap->arr[pos], heap->arr[l]);
    if (r < heap->size) {
      cmp_r = heap->cmp(heap->arr[pos], heap->arr[r]);
    } else {
      cmp_r = 1 << (8 * sizeof(int) - 1);
    }
    pos_move = -1;
    if (cmp_l > 0 || cmp_r > 0) {
      pos_move = heap->cmp(heap->arr[l], heap->arr[r]) < 0 ? l : r;
    }
    if (pos_move != -1) {
      tmp = heap->arr[pos_move];
      heap->arr[pos_move] = heap->arr[pos];
      heap->arr[pos] = tmp;
      pos = pos_move;
      ok = 0;
    }
    l = GO_LEFT(pos);
    r = GO_RIGHT(pos);
  }
}

void heap_pop(Heap *heap) {
  if (!heap || !heap->arr || !heap->cmp) {
    return;
  }

  heap->arr[0] = heap->arr[heap->size - 1];
  heap->size--;
  __heap_pop_fix(heap, 0);
}

int heap_empty(Heap *heap) {
  if (heap == NULL || heap->arr == NULL) {
    return -1;
  }
  return heap->size <= 0;
}

void heap_free(Heap *heap) {
  if (heap == NULL) {
    return;
  }
  free(heap->arr);
  free(heap);
}
