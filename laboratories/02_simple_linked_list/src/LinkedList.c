/* Implementarea se bazeaza pe faptul ca retinem mereu si numarul de elemente
 * din lista. In cazul in care nu putem face asta, ca sa verificam de exemplu
 * ca lista este goala verificam daca list->head == NULL, in loc de
 * list->size == 0
*/

#include <stdio.h>
#include <stdlib.h>

#include "LinkedList.h"

/*
 * Functie care trebuie apelata dupa alocarea unei liste simplu inlantuite,
 * pentru a o initializa. (Setare valori initiale pentru campurile specifice
 * structurii LinkedList).
 */
void init_list(struct LinkedList *list) {
  if (list == NULL)
    return;
  list->head = list->tail = NULL;
  list->size = 0;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, eroare.
 */
void add_nth_node(struct LinkedList *list, int n, void *new_data) {
  struct Node *new, *curr;
  int i;
  if (n < 0 || list == NULL) {
    return;
  }

  new = malloc(sizeof(struct Node));
  new->data = new_data;
  if (list->size == 0) {
    new->next = NULL;
    list->head = list->tail = new;
  }
  else if (n == 0) {
    new->next = list->head;
    list->head = new;
  }
  else if (n >= list->size) {
    new->next = NULL;
    list->tail->next = new;
    list->tail = new;
  }
  else {
    curr = list->head;
    i = 0;
    while (i < n - 1 && i < list->size - 2 ) {
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
struct Node *remove_nth_node(struct LinkedList *list, int n) {
  struct Node *old, *curr;
  int i;
  if (n < 0 || list == NULL || list->size == 0) {
    fprintf(stderr, "Eroare\n");
    return NULL;
  }
  if (list->size == 1) {
    old = list->head;
    list->head = list->tail = NULL;
  }
  else if (n == 0) {
    old = list->head;
    list->head = list->head->next;
  }
  else {
    curr = list->head;
    i = 0;
    while (i < n - 1 && i < list->size - 2) {
      curr = curr->next;
      i++;
    }
    old = curr->next;
    curr->next = curr->next->next;
  }
  list->size--;
  return old;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
int get_size(struct LinkedList *list) {
  if (list == NULL)
    return -1;
  return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void free_list(struct LinkedList **pp_list) {
  struct Node *curr, *last;
  struct LinkedList *list = *pp_list;
  int i;

  if (list == NULL)
    return;

  if (list->size != 0) {
    curr = list->head;
    for (i = 0; i < list->size; i++) {
      last = curr;
      curr = curr->next;
      free(last);
    }
  }
  free(list);
  *pp_list = NULL;
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista inlantuita separate printr-un spatiu.
 */
void print_int_linkedlist(struct LinkedList *list) {
  struct Node *curr;
  int i;

  if (list == NULL)
    printf("List este vida.\n");

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
void print_string_linkedlist(struct LinkedList *list) {
  struct Node *curr;
  int i;

  if (list == NULL)
    printf("List este vida.\n");

  curr = list->head;
  for (i = 0; i < list->size; i++) {
    fputs(((char *)(curr->data)), stdout);
    fputc(' ', stdout);
    curr = curr->next;
  }
  printf("\n");
}

/*
 * Aceasta functie primeste ca parametru un pointer la inceputul unei liste si
 * construieste doua liste in care se afla toate elementele de pe pozitii pare,
 * respectiv impare. Listele odd si even trebuie doar alocate inainte, dar NU
 * si initializate.
*/
void split_list(struct LinkedList *list, struct LinkedList *odd,
    struct LinkedList *even) {
  int i;
  struct Node *curr;
  if (list == NULL || list->size == 0 || odd == NULL || even == NULL)
    return ;

  init_list(odd);
  init_list(even);

  curr = list->head;
  for (i = 0; i < list->size; i++) {
    if ((i + 1) % 2 == 1)
      add_nth_node(odd, i / 2, curr->data);
    else
      add_nth_node(even, i / 2, curr->data);
    curr = curr->next;
  }
}
