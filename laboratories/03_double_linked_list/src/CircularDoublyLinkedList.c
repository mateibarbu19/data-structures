#include <stdio.h>
#include <stdlib.h>

#include "CircularDoublyLinkedList.h"

/*
 * Functie care trebuie apelata dupa alocarea unei liste, pentru a o initializa.
 * (Setare valori initiale pentru campurile specifice structurii LinkedList).
 */
void init_list(struct CircularDoublyLinkedList *list) {
  if (list == NULL) {
    fprintf(stderr, "Nu s-a apelat functia init_list corect.\n");
    return;
  }
  list->size = 0;
  list->head = NULL;
}

/*
 * Functia intoarce un pointer la nodul de pe pozitia n din lista.
 * Pozitiile din lista sunt indexate incepand cu 0 (i.e. primul nod din lista se
 * afla pe pozitia n=0). Daca n >= nr_noduri, atunci se intoarce nodul de pe
 * pozitia rezultata daca am "cicla" (posibil de mai multe ori) pe lista si am
 * trece de la ultimul nod, inapoi la primul si am continua de acolo. Cum putem
 * afla pozitia dorita fara sa simulam intreaga parcurgere? Daca n < 0, eroare.
 */
struct Node *get_nth_node(struct CircularDoublyLinkedList *list, int n) {
  struct Node *curr;
  int i;

  if (list == NULL || list->size == 0 || n < 0) {
    return NULL;
  }

  n %= list->size;
  if (n < list->size / 2) {
    curr = list->head;
    for (i = 0; i < n; i++)
      curr = curr->next;
  } else {
    curr = list->head->prev; /* suntem la tail */
    for (i = list->size - 1; i > n; i--)
      curr = curr->prev;
  }
  return curr;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Cand indexam pozitiile nu "ciclam" pe lista circulara ca la
 * get, ci consideram nodurile in ordinea de la head la ultimul (adica acel nod
 * care pointeaza la head ca nod urmator in lista). Daca n >= nr_noduri, atunci
 * adaugam nodul nou la finalul listei. Daca n < 0, eroare.
 */
void add_nth_node(struct CircularDoublyLinkedList *list, int n,
                  void *new_data) {
  struct Node *new, *nth_node, *back;

  if (n < 0 || list == NULL) {
    return;
  }

  new = malloc(sizeof(struct Node));
  if (new == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei in RAM.\n");
    return;
  }
  new->data = new_data;

  if (list->size == 0) {
    new->next = new->prev = new;
    list->head = new;
  } else {
    if (n < list->size)
      nth_node = get_nth_node(list, n);
    else
      nth_node = list->head; /* adaugam un nod intre head si tail */
    back = nth_node->prev;   /* dc n=0 => back=tail */
    new->next = nth_node;
    new->prev = back;
    back->next = new;
    nth_node->prev = new;
    if (n == 0) {
      list->head = new;
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
struct Node *remove_nth_node(struct CircularDoublyLinkedList *list, int n) {
  struct Node *nth_node, *back, *front;

  if (list == NULL || list->size == 0 || n < 0) {
    return NULL;
  }

  if (list->size == 1) {
    nth_node = list->head;
    list->head = NULL;
  } else {
    if (n < list->size) {
      nth_node = get_nth_node(list, n);
    } else {
      nth_node = list->head->prev; /* pentru ca eliminam ultimul nod */
    }
    back = nth_node->prev;
    front = nth_node->next;
    back->next = front;
    front->prev = back;
    if (n == 0) {
      list->head = front;
    }
  }

  list->size--;
  return nth_node;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
int get_size(struct CircularDoublyLinkedList *list) {
  if (list == NULL) {
    return -1;
  }
  return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista.
 */
void free_list(struct CircularDoublyLinkedList **pp_list) {
  struct Node *curr, *last;
  struct CircularDoublyLinkedList *list = *pp_list;
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
  *pp_list = NULL;
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista separate printr-un spatiu, incepand de la primul nod din lista.
 */
void print_int_list(struct CircularDoublyLinkedList *list) {
  struct Node *curr;
  int i;

  if (list == NULL || list->size == 0) {
    printf("List este vida.\n");
    return;
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
 * nodurile din lista separate printr-un spatiu, incepand de la primul nod din
 * lista.
 */
void print_string_list(struct CircularDoublyLinkedList *list) {
  struct Node *curr;
  int i;

  if (list == NULL || list->size == 0) {
    printf("List este vida.\n");
    return;
  }

  curr = list->head;
  for (i = 0; i < list->size; i++) {
    fputs(((char *)(curr->data)), stdout);
    fputc(' ', stdout);
    curr = curr->next;
  }
  printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza o singura data toate valorile int
 * stocate in nodurile din lista, separate printr-un spatiu, incepand de la
 * nodul dat ca parametru si continuand la stanga in lista dublu inlantuita
 * circulara, pana cand sunt afisate valorile tuturor nodurilor.
 */
void print_ints_left_circular(struct Node *start) {
  struct Node *curr = start;

  if (start == NULL) {
    printf("Nodul de pornire nu exista.\n");
    return;
  }

  do {
    printf("%d ", *((int *)(curr->data)));
    curr = curr->prev;
  } while (curr != start);
  printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza o singura data toate valorile int
 * stocate in nodurile din lista, separate printr-un spatiu, incepand de la
 * nodul dat ca parametru si continuand la dreapta in lista dublu inlantuita
 * circulara, pana cand sunt afisate valorile tuturor nodurilor.
 */
void print_ints_right_circular(struct Node *start) {
  struct Node *curr = start;

  if (start == NULL) {
    printf("Nodul de pornire nu exista.\n");
    return;
  }

  do {
    printf("%d ", *((int *)(curr->data)));
    curr = curr->next;
  } while (curr != start);
  printf("\n");
}

int compare_int(void *a, void *b) {
  return *(int *)a - *(int *)b;
}

struct CircularDoublyLinkedList *
merge_sorted_lists(struct CircularDoublyLinkedList *first,
                   struct CircularDoublyLinkedList *second,
                   int (*cmp)(void *a, void *b)) {

  struct CircularDoublyLinkedList *res;
  struct Node *curr_f, *curr_s;
  int i, j, max;

  if (first == NULL || second == NULL || first->size == 0 ||
      second->size == 0) {
    return NULL;
  }

  res = malloc(sizeof(struct CircularDoublyLinkedList));
  init_list(res);

  max = first->size + second->size + 1;
  i = j = 0;
  curr_f = first->head;
  curr_s = second->head;
  while (i < first->size && j < second->size) {
    if ((*cmp)(curr_f->data, curr_s->data) < 0) {
      add_nth_node(res, max, curr_f->data);
      i++;
      curr_f = curr_f->next;
    } else {
      add_nth_node(res, max, curr_s->data);
      j++;
      curr_s = curr_s->next;
    }
  }
  while (i < first->size) {
    add_nth_node(res, max, curr_f->data);
    i++;
    curr_f = curr_f->next;
  }
  while (j < second->size) {
    add_nth_node(res, max, curr_s->data);
    j++;
    curr_s = curr_s->next;
  }
  return res;
}
