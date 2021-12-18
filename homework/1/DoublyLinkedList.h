// Copyright 2020 Barbu Matei (matei.barbu1905@stud.acs.upb.ro)
#ifndef DOUBLYLINKEDLIST_H_
#define DOUBLYLINKEDLIST_H_

struct Node {
  void *data;
  struct Node *next, *prev;
};

struct DoublyLinkedList {
  struct Node *head, *tail;
  int size;
  int cursor;
};

void init_list(struct DoublyLinkedList *list);

struct Node *get_nth_node(struct DoublyLinkedList *list, int n);

void add_nth_node(struct DoublyLinkedList *list, int n, void *new_data);

struct Node *remove_nth_node(struct DoublyLinkedList *list, int n);

void free_list(struct DoublyLinkedList *list, int free_info);

#endif  // DOUBLYLINKEDLIST_H_
