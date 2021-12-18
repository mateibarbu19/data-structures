#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "LinkedList.h"
#include <stdint.h>

struct info_t {
  void *key;
  void *value;
};
typedef struct info_t info;

struct Hashtable_t {
  LinkedList **buckets; /* Array de pointeri catre liste simplu-inlantuite. */
  int size; /* Nr. total de noduri existente curent in toate bucket-urile. */
  int hmax; /* Nr. de bucket-uri. */
  /* (Pointer la) Functie pentru a calcula valoarea hash asociata cheilor. */
  uint64_t (*hash_function)(void *);
  /* (Pointer la) Functie pentru a compara doua chei. */
  int (*compare_function)(void *, void *);
};
typedef struct Hashtable_t Hashtable;

Hashtable *init_ht(int hmax, uint64_t (*hash_function)(void *),
                   int (*compare_function)(void *, void *));

void put_ht_pair(Hashtable *ht, void *key, int key_size_bytes, void *value);

void *get_ht_entry(Hashtable *ht, void *key);

int has_ht_key(Hashtable *ht, void *key);

void remove_ht_entry(Hashtable *ht, void *key);

int get_ht_size(Hashtable *ht);

int get_ht_hmax(Hashtable *ht);

void free_ht(Hashtable *ht);

void free_ht_and_values(Hashtable *ht, void (*free_value)(void *));

#endif /* HASHTABLE_H_ */
