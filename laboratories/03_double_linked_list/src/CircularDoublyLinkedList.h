#ifndef __CIRCULARDOUBLYLINKEDLIST_H__
#define __CIRCULARDOUBLYLINKEDLIST_H__

struct Node {
  void *data; /* Pentru ca datele stocate sa poata avea orice tip, folosim un
                 pointer la void. */
  struct Node *prev, *next;
};

struct CircularDoublyLinkedList {
  int size;
  struct Node *head;
};

void init_list(struct CircularDoublyLinkedList *list);

/*
 * Acestea sunt functiile pe care trebuie sa le implementam.
 * Implementarea acestora se va face in LinkedList.c .
 */

struct Node *get_nth_node(struct CircularDoublyLinkedList *list, int n);

void add_nth_node(struct CircularDoublyLinkedList *list, int n, void *new_data);

struct Node *remove_nth_node(struct CircularDoublyLinkedList *list, int n);

int get_size(struct CircularDoublyLinkedList *list);

void free_list(struct CircularDoublyLinkedList **pp_list);

void print_int_list(struct CircularDoublyLinkedList *list);

void print_string_list(struct CircularDoublyLinkedList *list);

void print_ints_left_circular(struct Node *start);

void print_ints_right_circular(struct Node *start);

struct CircularDoublyLinkedList *
merge_sorted_lists(struct CircularDoublyLinkedList *first,
                   struct CircularDoublyLinkedList *second,
                   int (*cmp)(void *a, void *b));

int compare_int(void *a, void *b);

#endif /* __CIRCULARDOUBLYLINKEDLIST_H__ */
