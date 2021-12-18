#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"
#include "tools.h"

Queue *init_queue(void) {
  Queue *q = malloc(sizeof(Queue));
  DIE(q == NULL, "Queue malloc");
  q->list = init_list();
  return q;
}

int get_size_queue(Queue *q) {
  if (!q || !q->list) {
    return -1;
  }
  return q->list->size;
}

int is_empty_queue(Queue *q) {
  if (!q || !q->list) {
    return 1;
  }
  return q->list->size == 0;
}

void *front(Queue *q) {
  if (!q || !q->list) {
    return NULL;
  }
  return q->list->head->data;
}

void dequeue(Queue *q) {
  Node *elem;
  if (!q || !q->list) {
    return;
  }
  elem = remove_nth_node(q->list, 0);
  free(elem);
}

void enqueue(Queue *q, void *new_data) {
  if (!q || !q->list) {
    return;
  }
  add_nth_node(q->list, q->list->size, new_data);
}

void clear_queue(Queue *q) {
  int i;
  if (!q || !q->list) {
    return;
  }
  for (i = get_size_queue(q) - 1; i >= 0; i--) {
    dequeue(q);
  }
}

void free_queue(void *p) {
  Queue *q = (Queue *)p;
  if (!q || !q->list) {
    return;
  }
  free_list(q->list);
  free(q);
}

void purge_queue(void *p, void (*free_data)(void *)) {
  Queue *q = (Queue *)p;
  if (!q || !q->list) {
    return;
  }
  purge_list(q->list, free_data);
  free(q);
}
