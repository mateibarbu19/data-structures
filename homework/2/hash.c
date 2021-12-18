/* Copyright 2020 Barbu Matei (matei.barbu1905@stud.acs.upb.ro) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LMAX 100 + 1

/* Pentru mai multe detalii consultati: man 3 strdup */
char *strdup(const char *s) {
  size_t len;
  char *new;
  if (s == NULL) {
    return NULL;
  }
  len = strlen(s) + 1;
  new = malloc(len);
  if (new == NULL) {
    return NULL;
  }
  memcpy(new, s, len);
  return new;
}

unsigned int hash_function_string(void *a) {
  /*
   * Credits: http://www.cse.yorku.ca/~oz/hash.html
   */
  unsigned char *puchar_a = (unsigned char *)a;
  unsigned int hash = 5381;
  int c;

  while ((c = *puchar_a++))
    hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

  return hash;
}

struct bucket {
  unsigned int key;
  /* Valorile din tabel sunt: */
  char count;
  char *s;
};


unsigned int nr_of_songs_in_file(FILE *fin) {
  char s[LMAX];
  int count = 0;
  rewind(fin);
  while (fgets(s, LMAX, fin) != NULL) {
    count++;
  }
  return count;
}

struct Table {
  unsigned int nr_of_buckets;
  struct bucket *buckets;
};


void init_Table(struct Table *t, unsigned int nr_of_buckets) {
  unsigned int i;
  if (t == NULL || nr_of_buckets == 0) {
    return;
  }
  t->buckets = malloc(nr_of_buckets * sizeof(struct bucket));
  if (t->buckets == NULL) {
    perror("A aparut o eroare la alocarea memoriei.\n");
  }
  t->nr_of_buckets = nr_of_buckets;
  for (i = 0; i < nr_of_buckets; i++) {
    t->buckets[i].count = 0;
    t->buckets[i].s = NULL;
    t->buckets[i].key = (unsigned int) -1;
  }
}

/* Functia determina adauga un sir de caractere in dictionar, iar daca sirul a mai aparut
 * ii mareste numarul de apariti.
 */
void add_to_Table(struct Table *t, char *s) {
  unsigned int key, i;
  if (t == NULL || s == NULL) {
    return;
  }

  key = hash_function_string(s) % t->nr_of_buckets;

  i = key;
  /* Cautam prima pozitie libera sa inseram sirul ijn dictionar
   * sau vedem daca a mai aparut
   */
  while (t->buckets[i].s != NULL && strcmp(t->buckets[i].s, s) != 0) {
    i = (i + 1) % t->nr_of_buckets;
  }

  /* In cazul primei apariti copiam si continutul sirului in dictionar */
  if (t->buckets[i].s == NULL) {
    t->buckets[i].s = strdup(s);
    t->buckets[i].key = i; /* respectiv cheia la care se gaseste */
  }
  if (t->buckets[i].s == NULL) {
    perror("A aparut o eroare la alocarea memoriei.\n");
  }
  (t->buckets[i].count)++; /* Mereu incrementam numarul de aparitii */
}

/* Functia dealoca toata memoria folosita de dict., dar nu si dict. in sine */
void purge_Table(struct Table *t) {
  unsigned int i;
  if (t == NULL) {
    return;
  }
  for (i = 0; i < t->nr_of_buckets; i++) {
    free(t->buckets[i].s);
  }
  free(t->buckets);
}

int main() {
  struct Table *freq;
  char s[LMAX];
  unsigned int i, nr_of_songs;


  nr_of_songs = nr_of_songs_in_file(stdin);

  freq = malloc(sizeof(struct Table));
  if (freq == NULL) {
    perror("A aparut o eroare la alocarea memoriei.\n");
    return 2;
  }
  init_Table(freq, nr_of_songs);

  rewind(stdin);
  /* Observatie: inputul este obligatoriu redirectat dintr-un fisier. */
  /* Altfel, rewind(stdin) nu functioneaza. */
  while (fgets(s, LMAX, stdin) != NULL) {
    s[strlen(s) - 1] = '\0'; /* stergem '\n'-ul din coada */

    add_to_Table(freq, s);
  }

  for (i = 0; i < freq->nr_of_buckets; i++) {
    if (freq->buckets[i].count != 0) {
      printf("%s %d\n", freq->buckets[i].s, freq->buckets[i].count);
    }
  }
  purge_Table(freq);
  free(freq);

  return 0;
}
