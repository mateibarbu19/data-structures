#include "./DoublyLinkedList.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Functie care trebuie apelata dupa alocarea unei liste, pentru a o initializa.
 * (Setare valori initiale pentru campurile specifice structurii
 * DoublyLinkedList).
 */
void init_list(struct DoublyLinkedList *list) {
  if (list == NULL) {
    fprintf(stderr, "Nu s-a apelat functia init_list corect.\n");
    return;
  }
  list->size = 0;
  list->head = list->tail = NULL;
}

/*
 * Functia intoarce un pointer la nodul de pe pozitia n din lista.
 * Pozitiile din lista sunt indexate incepand cu 0 (i.e. primul nod din lista se
 * afla pe pozitia n=0). Daca n >= nr_noduri, atunci se intoarce nodul de pe
 * ultima pozitie. Daca n < 0, eroare.
 */
struct Node *get_nth_node(struct DoublyLinkedList *list, int n) {
  struct Node *curr;
  int i;

  if (list == NULL || list->size == 0 || n < 0 || n >= list->size) {
    return NULL;
  }

  if (n < list->size / 2) {
    curr = list->head;
    for (i = 0; i < n; i++)
      curr = curr->next;
  } else {
    curr = list->tail;
    for (i = list->size - 1; i > n; i--)
      curr = curr->prev;
  }
  return curr;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, eroare.
 */
void add_nth_node(struct DoublyLinkedList *list, int n, void *new_data) {
  struct Node *new, *nth_node, *back;

  if (n < 0 || list == NULL) {
    return;
  }

  new = malloc(sizeof(struct Node));
  if (new == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.\n");
    return;
  }
  new->data = new_data;

  if (list->size == 0) {
    new->next = new->prev = NULL;
    list->head = list->tail = new;
  } else {
    if (n == 0) {
      new->next = list->head;
      new->prev = NULL;
      list->head->prev = new;
      list->head = new;
    } else if (n <= list->size - 1) {
      nth_node = get_nth_node(list, n);
      back = nth_node->prev;
      new->next = nth_node;
      new->prev = back;
      nth_node->prev = new;
      back->next = new;
    } else {
      new->next = NULL;
      new->prev = list->tail;
      list->tail->next = new;
      list->tail = new;
    }
  }
  list->size++;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Functia intoarce un pointer spre acest nod
 * proaspat eliminat din lista. Daca n >= nr_noduri - 1, se elimina nodul de la
 * finalul listei. Daca n < 0, eroare. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
struct Node *remove_nth_node(struct DoublyLinkedList *list, int n) {
  struct Node *nth_node, *back, *front;

  if (list == NULL || list->size == 0 || n < 0) {
    return NULL;
  }

  if (list->size == 1) {
    nth_node = list->head;
    list->head = NULL;
    list->tail = NULL;
  } else {
    if (n == 0) {
      nth_node = list->head;
      list->head = list->head->next;
      list->head->prev = NULL;
    } else if (n < list->size - 1) {
      nth_node = get_nth_node(list, n);
      back = nth_node->prev;
      front = nth_node->next;
      back->next = front;
      front->prev = back;
    } else {
      nth_node = list->tail;
      list->tail = list->tail->prev;
      list->tail->next = NULL;
    }
  }

  list->size--;
  return nth_node;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista.
 * Daca free_info == 1, se elibereaza memeoria din campul data, altfel nu.
 */
void free_list(struct DoublyLinkedList *list, int free_info) {
  struct Node *curr, *last;
  int i;

  if (list == NULL) {
    return;
  }

  if (list->size != 0) {
    curr = list->head;
    for (i = 0; i < list->size; i++) {
      last = curr;
      curr = curr->next;
      if (free_info == 1) {
        free(last->data);
      }
      free(last);
    }
  }
  free(list);
}
