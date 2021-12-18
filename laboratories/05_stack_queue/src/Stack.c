#include <stdio.h>
#include <stdlib.h>

#include "Stack.h"

void init_stack(struct Stack *stack) {
    stack->list = malloc(sizeof(struct LinkedList));
    if (stack->list == NULL) {
        perror("Not enough memory to initialize the stack!");
        return;
    }

    init_list(stack->list);
}

int get_size_stack(struct Stack *stack) {
    if (stack == NULL || stack->list == NULL) {
        return -1;
    }
    return stack->list->size;
}

int is_empty_stack(struct Stack *stack) {
    if (stack == NULL || stack->list == NULL) {
        return 1;
    }
    return stack->list->size == 0;
}

void* peek_stack(struct Stack *stack) {
    if (stack == NULL || stack->list == NULL) {
        return NULL;
    }
    return stack->list->tail->data;
}

void pop_stack(struct Stack *stack) {
    struct Node *elem;
    if (stack == NULL || stack->list == NULL) {
        return;
    }
    elem = remove_nth_node(stack->list, stack->list->size - 1);
    free(elem);
}

void push_stack(struct Stack *stack, void *new_data) {
    if (stack == NULL || stack->list == NULL) {
        return;
    }
    add_nth_node(stack->list, stack->list->size, new_data);
}

void clear_stack(struct Stack *stack) {
    int i;
    if (stack == NULL || stack->list == NULL) {
        return;
    }
    for (i = stack->list->size - 1; i >= 0; i--) {
        pop_stack(stack);
    }
}

void purge_stack(struct Stack *stack) {
    if (stack == NULL || stack->list == NULL) {
        return;
    }
    free_list(&(stack->list));
}
