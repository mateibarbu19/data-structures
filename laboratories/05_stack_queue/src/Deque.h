#ifndef __DEQUE_H__
#define __DEQUE_H__

#include "DoublyLinkedList.h"

struct Deque {
    struct DoublyLinkedList *list;
};

void init_deque(struct Deque *deque);

int get_size_deque(struct Deque *deque);

int is_empty_deque(struct Deque *deque);

void* front_deque(struct Deque *deque);

void* back_deque(struct Deque *deque);

void pop_front_deque(struct Deque *deque);

void pop_back_deque(struct Deque *deque);

void push_front_deque(struct Deque *deque, void *new_data);

void push_back_deque(struct Deque *deque, void *new_data);

void clear_deque(struct Deque *deque);

void purge_deque(struct Deque *deque);

#endif /* __DEQUE_H__ */
