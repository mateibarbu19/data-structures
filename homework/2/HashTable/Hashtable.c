/*
 * Hashtable.c
 * Alexandru-Cosmin Mihai
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Hashtable.h"

/*
 * Functii de comparare a cheilor:
 */
int compare_function_ints(void *a, void *b) {
  int int_a = *((int *)a);
  int int_b = *((int *)b);

  if (int_a == int_b) {
    return 0;
  } else if (int_a < int_b) {
    return -1;
  } else {
    return 1;
  }
}

int compare_function_strings(void *a, void *b) {
  char *str_a = (char *)a;
  char *str_b = (char *)b;

  return strcmp(str_a, str_b);
}

/*
 * Functii de hashing:
 */
unsigned int hash_function_int(void *a) {
  /*
   * Credits: https://stackoverflow.com/a/12996028/7883884
   */
  unsigned int uint_a = *((unsigned int *)a);

  uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
  uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
  uint_a = (uint_a >> 16u) ^ uint_a;
  return uint_a;
}

unsigned int hash_function_string(void *a) {
  /*
   * Credits: http://www.cse.yorku.ca/~oz/hash.html
   */
  unsigned char *puchar_a = (unsigned char *)a;
  unsigned long hash = 5381;
  int c;

  while ((c = *puchar_a++))
    hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

  return hash;
}

/*
 * Functie apelata dupa alocarea unui hashtable pentru a-l initializa.
 * Trebuie alocate si initializate si listele inlantuite.
 */
void init_ht(struct Hashtable *ht, int hmax,
             unsigned int (*hash_function)(void *),
             int (*compare_function)(void *, void *)) {
  int i;
  if (ht == NULL || hmax < 1) {
    printf("Nu s-a apelat functia de initializare corect.\n");
    return;
  }
  if (hash_function == NULL || compare_function == NULL) {
    printf("Nu s-a apelat functia de initializare corect.\n");
    return;
  }
  ht->buckets = malloc(hmax * sizeof(struct LinkedList));
  if (ht->buckets == NULL) {
    fprintf(stderr, "A aparut o eroare de memorie.\n");
    return;
  }
  ht->hmax = hmax;
  ht->size = 0;
  ht->hash_function = hash_function;
  ht->compare_function = compare_function;
  for (i = 0; i < hmax; i++) {
    init_list(ht->buckets + i);
  }
}

/*
 * Atentie! Desi cheia este trimisa ca un void pointer (deoarece nu se impune
 * tipul ei), in momentul in care se creeaza o noua intrare in hashtable (in
 * cazul in care cheia nu se gaseste deja in ht), trebuie creata o copie a
 * valorii la care pointeaza key si adresa acestei copii trebuie salvata in
 * structura info asociata intrarii din ht. Pentru a sti cati octeti trebuie
 * alocati si copiati, folositi parametrul key_size_bytes.
 *
 * Motivatie:
 * Este nevoie sa copiem valoarea la care pointeaza key deoarece dupa un apel
 * put(ht, key_actual, value_actual), valoarea la care pointeaza key_actual
 * poate fi alterata (de ex: *key_actual++). Daca am folosi direct adresa
 * pointerului key_actual, practic s-ar modifica din afara hashtable-ului cheia
 * unei intrari din hashtable. Nu ne dorim acest lucru, fiindca exista riscul sa
 * ajungem in situatia in care nu mai stim la ce cheie este inregistrata o
 * anumita valoare.
 */
void put(struct Hashtable *ht, void *key, int key_size_bytes, void *value) {
  unsigned int index;
  struct info *new;
  struct Node *curr;
  if (ht == NULL || key == NULL || key_size_bytes == 0 || value == NULL) {
    printf("Nu s-a apelat functia de indexare corect.\n");
    return;
  }

  index = ht->hash_function(key) % ht->hmax;
  curr = ht->buckets[index].head;
  while (curr != NULL &&
         ht->compare_function(((struct info *)curr->data)->key, key) != 0) {
    curr = curr->next;
  }
  if (curr != NULL) {
    ((struct info *)curr->data)->value = value;
  } else {
    new = malloc(sizeof(struct info));
    if (new == NULL) {
      fprintf(stderr, "A aparut o eroare de memorie.\n");
      return;
    }
    new->key = malloc(key_size_bytes);
    memcpy((char *)new->key, (char *)key, key_size_bytes);
    new->value = value;
    add_nth_node(ht->buckets + index, ht->buckets[index].size, (void *)new);
    ht->size++;
  }
}

void *get(struct Hashtable *ht, void *key) {
  unsigned int index;
  struct Node *curr;
  if (ht == NULL || key == NULL) {
    printf("Nu s-a apelat functia de cautare corect.\n");
    return NULL;
  }

  index = ht->hash_function(key) % ht->hmax;
  curr = ht->buckets[index].head;
  while (curr != NULL &&
         ht->compare_function(((struct info *)curr->data)->key, key) != 0) {
    curr = curr->next;
  }
  if (curr == NULL) {
    fprintf(stderr, "Nu s-a gasit cheia in dictionar.\n");
    return NULL;
  }
  return ((struct info *)curr->data)->value;
}

/*
 * Functie care intoarce:
 * 1, daca pentru cheia key a fost asociata anterior o valoare in hashtable
 * folosind functia put 0, altfel.
 */
int has_key(struct Hashtable *ht, void *key) { return get(ht, key) != NULL; }

/*
 * Procedura care elimina din hashtable intrarea asociata cheii key.
 * Atentie! Trebuie avuta grija la eliberarea intregii memorii folosite pentru o
 * intrare din hashtable (adica memoria pentru copia lui key --vezi observatia
 * de la procedura put--, pentru structura info si pentru structura Node din
 * lista inlantuita).
 */
void remove_ht_entry(struct Hashtable *ht, void *key) {
  unsigned int index, i;
  struct Node *curr;
  if (ht == NULL || key == NULL) {
    printf("Nu s-a apelat functia de cautare corect.\n");
    return;
  }

  index = ht->hash_function(key) % ht->hmax;
  curr = ht->buckets[index].head;
  i = 0;
  while (curr != NULL &&
         ht->compare_function(((struct info *)curr->data)->key, key) != 0) {
    curr = curr->next;
    i++;
  }
  if (curr != NULL) {
    free(((struct info *)curr->data)->key);
    free(curr->data);
    remove_nth_node(ht->buckets + index, i);
    free(curr);
    ht->size--;
  }
}

/*
 * Procedura care elibereaza memoria folosita de toate intrarile din hashtable,
 * dupa care elibereaza si memoria folosita pentru a stoca structura hashtable.
 */
void free_ht(struct Hashtable *ht) {
  int i;
  struct LinkedList *list;
  struct Node *curr, *past;
  for (i = 0; i < ht->hmax; i++) {
    list = ht->buckets + i;
    curr = list->head;
    while (curr != NULL) {
      free(((struct info *)curr->data)->key);
      free(curr->data);
      past = curr;
      curr = curr->next;
      free(past);
    }
  }
  free(ht->buckets);
  free(ht);
}

int get_ht_size(struct Hashtable *ht) {
  if (ht == NULL) {
    return -1;
  }

  return ht->size;
}

int get_ht_hmax(struct Hashtable *ht) {
  if (ht == NULL) {
    return -1;
  }

  return ht->hmax;
}
