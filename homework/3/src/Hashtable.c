#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Hashtable.h"
#include "tools.h"

/*
 * Functie apelata dupa alocarea unui hashtable pentru a-l initializa.
 * Trebuie alocate si initializate si listele inlantuite.
 */
Hashtable *init_ht(int hmax, uint64_t (*hash_function)(void *),
                   int (*compare_function)(void *, void *)) {
  int i;
  Hashtable *ht;
  if (hmax < 1 || !hash_function || !compare_function) {
    fprintf(stderr, "Hashtable initialize: wrong parameters.\n");
    return NULL;
  }
  ht = malloc(sizeof(Hashtable));
  DIE(ht == NULL, "Hashtable malloc");

  ht->buckets = malloc(hmax * sizeof(LinkedList *));
  DIE(ht->buckets == NULL, "Hashtable buckets malloc");

  ht->hmax = hmax;
  ht->size = 0;
  ht->hash_function = hash_function;
  ht->compare_function = compare_function;
  for (i = 0; i < hmax; i++) {
    ht->buckets[i] = init_list();
  }
  return ht;
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
void put_ht_pair(Hashtable *ht, void *key, int key_size_bytes, void *value) {
  unsigned int index;
  info *new;
  Node *curr;
  if (!ht || !key || key_size_bytes < 1 || !value) {
    fprintf(stderr, "Hashtable put: wrong parameters.\n");
    return;
  }

  index = ht->hash_function(key) % ht->hmax;
  curr = ht->buckets[index]->head;
  while (curr && ht->compare_function(((info *)curr->data)->key, key) != 0) {
    curr = curr->next;
  }
  if (curr) {
    ((info *)curr->data)->value = value;
  } else {
    new = malloc(sizeof(info));
    DIE(new == NULL, "Hashtable info malloc");

    new->key = malloc(key_size_bytes);
    DIE(new->key == NULL, "Hashtable info key malloc");
    DIE(!memcpy(new->key, key, key_size_bytes), "Hashtable info key memcpy");

    new->value = value;
    add_nth_node(ht->buckets[index], ht->buckets[index]->size, new);
    ht->size++;
  }
}

void *get_ht_entry(Hashtable *ht, void *key) {
  unsigned int index;
  Node *curr;
  if (!ht || !key) {
    fprintf(stderr, "Hashtable get: wrong parameters.\n");
    return NULL;
  }

  index = ht->hash_function(key) % ht->hmax;
  curr = ht->buckets[index]->head;
  while (curr && ht->compare_function(((info *)curr->data)->key, key) != 0) {
    curr = curr->next;
  }
  if (!curr) {
    return NULL;
  }
  return ((info *)curr->data)->value;
}

/*
 * Functie care intoarce:
 * 1, daca pentru cheia key a fost asociata anterior o valoare in hashtable
 * folosind functia put 0, altfel.
 */
int has_ht_key(Hashtable *ht, void *key) {
  return get_ht_entry(ht, key) != NULL;
}

/*
 * Procedura care elimina din hashtable intrarea asociata cheii key.
 * Atentie! Trebuie avuta grija la eliberarea intregii memorii folosite pentru o
 * intrare din hashtable (adica memoria pentru copia lui key --vezi observatia
 * de la procedura put--, pentru structura info si pentru structura Node din
 * lista inlantuita).
 */
void remove_ht_entry(Hashtable *ht, void *key) {
  unsigned int index, i;
  Node *curr;
  if (!ht || !key) {
    fprintf(stderr, "Hashtable remove: wrong parameters.\n");
    return;
  }

  index = ht->hash_function(key) % ht->hmax;
  curr = ht->buckets[index]->head;
  i = 0;
  while (curr && ht->compare_function(((info *)curr->data)->key, key) != 0) {
    curr = curr->next;
    i++;
  }
  if (curr) {
    free(((info *)curr->data)->key);
    free(curr->data);
    purge_nth_node(ht->buckets[index], i);
    ht->size--;
  }
}

/*
 * Procedura care elibereaza memoria folosita de toate intrarile din hashtable,
 * dupa care elibereaza si memoria folosita pentru a stoca structura hashtable.
 */
void free_ht(Hashtable *ht) {
  int i;
  LinkedList *list;
  Node *curr, *past;
  if (!ht) {
    return;
  }
  for (i = 0; i < ht->hmax; i++) {
    list = ht->buckets[i];
    curr = list->head;
    while (curr) {
      free(((info *)curr->data)->key);
      free(curr->data);
      past = curr;
      curr = curr->next;
      free(past);
    }
    free(list);
  }
  free(ht->buckets);
  free(ht);
}

void free_ht_and_values(Hashtable *ht, void (*free_value)(void *)) {
  int i;
  LinkedList *list;
  Node *curr, *past;
  if (!ht || !free_value) {
    return;
  }
  for (i = 0; i < ht->hmax; i++) {
    list = ht->buckets[i];
    curr = list->head;
    while (curr) {
      free(((info *)curr->data)->key);
      free_value(((info *)curr->data)->value);
      free(curr->data);
      past = curr;
      curr = curr->next;
      free(past);
    }
    free(list);
  }
  free(ht->buckets);
  free(ht);
}

int get_ht_size(Hashtable *ht) {
  if (!ht) {
    return -1;
  }

  return ht->size;
}

int get_ht_hmax(Hashtable *ht) {
  if (!ht) {
    return -1;
  }

  return ht->hmax;
}

