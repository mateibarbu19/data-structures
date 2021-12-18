#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"

void init_q(struct Queue *q) {
    q->list = malloc(sizeof(struct LinkedList));
    if (q->list == NULL) {
        perror("Not enough memory to initialize the queue!");
        return;
    }

    init_list(q->list);
}

int get_size_q(struct Queue *q) {
    if (q == NULL || q->list == NULL) {
        return -1;
    }
    return q->list->size;
}

int is_empty_q(struct Queue *q) {
    if (q == NULL || q->list == NULL) {
        return 1;
    }
    return q->list->size == 0;
}

void* front(struct Queue *q) {
    if (q == NULL || q->list == NULL) {
        return NULL;
    }
    return q->list->head->data;
}

void dequeue(struct Queue *q) {
    struct Node *elem;
    if (q == NULL || q->list == NULL) {
        return;
    }
    elem = remove_nth_node(q->list, 0);
    free(elem);
}

void enqueue(struct Queue *q, void *new_data) {
    if (q == NULL || q->list == NULL) {
        return;
    }
    add_nth_node(q->list, q->list->size, new_data);
}

void clear_q(struct Queue *q) {
    int i;
    if (q == NULL || q->list == NULL) {
        return;
    }
    for (i = get_size_q(q) - 1; i >= 0; i--) {
        dequeue(q);
    }
}

void purge_q(struct Queue *q) {
    if (q == NULL || q->list == NULL) {
        return;
    }
    free_list(&(q->list));
}
