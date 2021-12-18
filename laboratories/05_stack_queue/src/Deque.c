#include <stdio.h>
#include <stdlib.h>

#include "Deque.h"

void init_deque(struct Deque *d) {
    d->list = malloc(sizeof(struct DoublyLinkedList));
    if (d->list == NULL) {
        perror("Not enough memory to initialize the queue!");
        return;
    }

    init_list(d->list);
}

int get_size_deque(struct Deque *d) {
    if (d == NULL || d->list == NULL) {
        return -1;
    }
    return d->list->size;
}

int is_empty_deque(struct Deque *d) {
    if (d == NULL || d->list == NULL) {
        return 1;
    }
    return d->list->size == 0;
}

void* front_deque(struct Deque *d) {
    if (d == NULL || d->list == NULL) {
        return NULL;
    }
    return d->list->head->data;
}

void* back_deque(struct Deque *d) {
    if (d == NULL || d->list == NULL) {
        return NULL;
    }
    return d->list->tail->data;
}

void pop_front_deque(struct Deque *d) {
    struct Node *elem;
    if (d == NULL || d->list == NULL) {
        return;
    }
    elem = remove_nth_node(d->list, 0);
    free(elem);
}

void pop_back_deque(struct Deque *d) {
    struct Node *elem;
    if (d == NULL || d->list == NULL) {
        return;
    }
    elem = remove_nth_node(d->list, d->list->size - 1);
    free(elem);
}

void push_front_deque(struct Deque *d, void *new_data) {
    if (d == NULL || d->list == NULL) {
        return;
    }
    add_nth_node(d->list, 0, new_data);
}

void push_back_deque(struct Deque *d, void *new_data) {
    if (d == NULL || d->list == NULL) {
        return;
    }
    add_nth_node(d->list, d->list->size, new_data);
}

void clear_deque(struct Deque *d) {
    int i;
    if (d == NULL || d->list == NULL) {
        return;
    }
    for (i = get_size_deque(d) - 1; i >= 0; i--) {
        pop_back_deque(d);
    }
}

void purge_deque(struct Deque *d) {
    if (d == NULL || d->list == NULL) {
        return;
    }
    free_list(d->list, 0);
}
