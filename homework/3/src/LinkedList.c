#include <stdio.h>
#include <stdlib.h>

#include "LinkedList.h"
#include "tools.h"

/* Implementarea se bazeaza pe faptul ca retinem mereu si numarul de elemente
 * din lista. In cazul in care nu putem face asta, ca sa verificam de exemplu
 * ca lista este goala verificam daca list->head == NULL, in loc de
 * list->size == 0
 */

/*
 * Functie care trebuie apelata dupa alocarea unei liste simplu inlantuite,
 * pentru a o initializa. (Setare valori initiale pentru campurile specifice
 * structurii LinkedList).
 */
LinkedList *init_list(void) {
  LinkedList *list = malloc(sizeof(LinkedList));
  DIE(list == NULL, "List malloc");
  list->head = list->tail = NULL;
  list->size = 0;
  return list;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, eroare.
 */
void add_nth_node(LinkedList *list, int n, void *new_data) {
  Node *new, *curr;
  int i;
  if (n < 0 || list == NULL || !new_data) {
    return;
  }

  new = malloc(sizeof(Node));
  DIE(new == NULL, "Node LinkedList malloc");
  new->data = new_data;
  new->next = NULL;

  if (list->size == 0) {
    list->head = list->tail = new;
  } else if (n == 0) {
    new->next = list->head;
    list->head = new;
  } else if (n >= list->size) {
    list->tail->next = new;
    list->tail = new;
  } else {
    curr = list->head;
    i = 0;
    while (i < n - 1) {
      curr = curr->next;
      i++;
    }
    new->next = curr->next;
    curr->next = new;
  }
  list->size++;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, eroare. Functia intoarce un pointer spre acest
 * nod proaspat eliminat din lista. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
Node *remove_nth_node(LinkedList *list, int n) {
  Node *old, *curr;
  int i;
  if (n < 0 || !list || !list->size) {
    return NULL;
  }
  if (list->size == 1) {
    old = list->head;
    list->head = list->tail = NULL;
  } else if (n == 0) {
    old = list->head;
    list->head = list->head->next;
  } else {
    curr = list->head;
    i = 0;
    while (i < n - 1 && i < list->size - 2) {
      curr = curr->next;
      i++;
    }
    old = curr->next;
    curr->next = curr->next->next;
    if (n >= list->size - 1) {
      list->tail = curr;
    }
  }
  list->size--;
  return old;
}

void purge_nth_node(LinkedList *list, int n) {
  Node *old;
  if (n < 0 || !list || !list->size) {
    return;
  }
  old = remove_nth_node(list, n);
  free(old);
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
int get_size_list(LinkedList *list) {
  if (list == NULL) {
    return -1;
  }
  return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void free_list(void *p) {
  LinkedList *list = (LinkedList *)p;
  Node *curr, *last;
  int i;

  if (list == NULL) {
    return;
  }

  if (list->size != 0) {
    curr = list->head;
    for (i = 0; i < list->size; i++) {
      last = curr;
      curr = curr->next;
      free(last);
    }
  }
  free(list);
}

void purge_list(void *p, void (*free_data)(void *)) {
  LinkedList *list = (LinkedList *)p;
  Node *curr, *last;
  int i;

  if (!list || !free_data) {
    return;
  }

  if (list->size != 0) {
    curr = list->head;
    for (i = 0; i < list->size; i++) {
      last = curr;
      free_data(curr->data);
      curr = curr->next;
      free(last);
    }
  }
  free(list);
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista inlantuita separate printr-un spatiu.
 */
void print_int_linkedlist(LinkedList *list) {
  Node *curr;
  int i;

  if (list == NULL) {
    fprintf(stderr, "LinkedList is empty.\n");
  }

  curr = list->head;
  for (i = 0; i < list->size; i++) {
    printf("%d ", *((int *)(curr->data)));
    curr = curr->next;
  }
  printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista inlantuita, separate printr-un spatiu.
 */
void print_string_linkedlist(LinkedList *list) {
  Node *curr;
  int i;

  if (list == NULL) {
    fprintf(stderr, "LinkedList is empty.\n");
  }

  curr = list->head;
  for (i = 0; i < list->size; i++) {
    fputs(((char *)(curr->data)), stdout);
    fputc(' ', stdout);
    curr = curr->next;
  }
  printf("\n");
}

void print_pointers_linkedlist(LinkedList *list) {
  Node *curr;

  if (list == NULL) {
    fprintf(stderr, "LinkedList is empty.\n");
  }

  for (curr = list->head; curr; curr = curr->next) {
    printf("%p ", curr->data);
  }
  printf("\n");
}
