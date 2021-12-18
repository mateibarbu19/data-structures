#ifndef QUEUE_H_
#define QUEUE_H_

#include "LinkedList.h"

struct Queue_t {
  LinkedList *list;
};
typedef struct Queue_t Queue;

Queue *init_queue(void);

int get_size_queue(Queue *q);

int is_empty_queue(Queue *q);

void *front(Queue *q);

void dequeue(Queue *q);

void enqueue(Queue *q, void *new_data);

void clear_queue(Queue *q);

void free_queue(void *p);

void purge_queue(void *p, void (*free_data)(void *));

#endif /* QUEUE_H_ */
