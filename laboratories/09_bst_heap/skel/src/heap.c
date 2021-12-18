/**
 * SD, 2020
 *
 * Lab #9, BST & Heap
 *
 * Task #2 - Heap implementation
 */

#include "heap.h"
#include "utils.h"

heap_t *heap_create(int (*cmp_f)(const team_t *, const team_t *)) {
  heap_t *heap;

  if (cmp_f == NULL) {
    return NULL;
  }

  heap = malloc(sizeof(heap_t));
  DIE(heap == NULL, "heap malloc");

  heap->cmp = cmp_f;
  heap->size = 0;
  heap->capacity = 2;
  heap->arr = malloc(heap->capacity * sizeof(team_t *));
  DIE(heap->arr == NULL, "heap->arr malloc");

  return heap;
}

static void __heap_insert_fix(heap_t *heap, int pos) {
  team_t *tmp_team;
  int p = GO_UP(pos);

  if (!heap || !heap->arr || !heap->cmp || p >= heap->size) {
    return;
  }

  while (pos != 0 && heap->cmp(heap->arr[p], heap->arr[pos]) > 0) {
    tmp_team = heap->arr[p];
    heap->arr[p] = heap->arr[pos];
    heap->arr[pos] = tmp_team;

    pos = p;
    p = GO_UP(pos);
  }
}

void heap_insert(heap_t *heap, team_t *team) {
  char *rc;

  if (!heap || !heap->arr || !heap->cmp || team == NULL) {
    return;
  }

  heap->arr[heap->size] = malloc(sizeof(team_t));
  DIE(heap->arr[heap->size] == NULL, "heap_insert malloc");

  heap->arr[heap->size]->name = calloc(TEAM_NAME_LEN, sizeof(char));
  DIE(heap->arr[heap->size]->name == NULL, "heap_insert name calloc");
  rc = strncpy(heap->arr[heap->size]->name, team->name, TEAM_NAME_LEN - 1);
  DIE(rc != heap->arr[heap->size]->name, "heap_insert name strncpy");
  heap->arr[heap->size]->score = team->score;

  __heap_insert_fix(heap, heap->size);

  heap->size++;
  if (heap->size == heap->capacity) {
    heap->capacity *= 2;

    heap->arr = realloc(heap->arr, heap->capacity * sizeof(team_t *));
    DIE(heap->arr == NULL, "heap->arr realloc");
  }
}

team_t heap_top(heap_t *heap) {
  team_t garbeage;
  garbeage.name = NULL;
  garbeage.score = 0;
  if (heap == NULL || heap->arr == NULL) {
    return garbeage;
  }
  return *(heap->arr[0]);
}

static void __heap_pop_fix(heap_t *heap, int pos) {
  team_t *tmp_team;
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
      tmp_team = heap->arr[pos_move];
      heap->arr[pos_move] = heap->arr[pos];
      heap->arr[pos] = tmp_team;
      pos = pos_move;
      ok = 0;
    }
    l = GO_LEFT(pos);
    r = GO_RIGHT(pos);
  }
}

void heap_pop(heap_t *heap) {
  if (!heap || !heap->arr || !heap->cmp) {
    return;
  }

  free(heap->arr[0]->name);
  free(heap->arr[0]);

  heap->arr[0] = heap->arr[heap->size - 1];

  heap->size--;

  __heap_pop_fix(heap, 0);
}

int heap_empty(heap_t *heap) {
  if (heap == NULL || heap->arr == NULL) {
    return -1;
  }
  return heap->size <= 0;
}

void heap_free(heap_t *heap) {
  int i;
  if (heap == NULL) {
    return;
  }
  if (heap->arr != NULL) {
    for (i = 0; i < heap->size; i++) {
      free(heap->arr[i]->name);
      free(heap->arr[i]);
    }
  }
  free(heap->arr);
  free(heap);
}
