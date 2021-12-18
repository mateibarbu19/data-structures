#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

struct node_t {
  void *data;
  struct node_t *next;
};
typedef struct node_t Node;

struct LinkedList_t {
  int size;
  Node *head, *tail;
};
typedef struct LinkedList_t LinkedList;

LinkedList *init_list(void);

/*
 * Acestea sunt functiile pe care trebuie sa le implementam.
 * Implementarea acestora se va face in LinkedList.c .
 */

void add_nth_node(LinkedList *list, int n, void *new_data);

Node *remove_nth_node(LinkedList *list, int n);

void purge_nth_node(LinkedList *list, int n);

int get_size_list(LinkedList *list);

void free_list(void *p);

void purge_list(void *p, void (*free_data)(void *));

void print_int_linkedlist(LinkedList *list);

void print_string_linkedlist(LinkedList *list);

void print_pointers_linkedlist(LinkedList *list);

#endif /* LINKEDLIST_H_ */
