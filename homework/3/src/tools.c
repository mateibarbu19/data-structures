#include <stdint.h>
#include <string.h>

#include "tools.h"

#define FNV_offset_basis ((uint64_t)14695981039346656037ull)
#define FNV_prime ((uint64_t)1099511628211ull)

char *my_strdup(const char *s) {
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

uint64_t hash_function_int64(void *a) {
  /*
   * Credits:
   * https://stackoverflow.com/questions/6943493/hash-table-with-64-bit-values-as-key
   */
  uint64_t hash = FNV_offset_basis;
  unsigned char *p = a;
  unsigned int i;

  for (i = 0; i < sizeof(int64_t); i++) {
    hash *= FNV_prime;
    hash ^= p[i];
  }

  return hash;
}

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

int compare_function_int64s(void *a, void *b) {
  int64_t int_a = *((int64_t *)a);
  int64_t int_b = *((int64_t *)b);

  if (int_a == int_b) {
    return 0;
  } else if (int_a < int_b) {
    return -1;
  } else {
    return 1;
  }
}

uint64_t hash_function_string(void *a) {
  /*
   * Credits: http://www.cse.yorku.ca/~oz/hash.html
   */
  unsigned char *puchar_a = (unsigned char *)a;
  uint64_t hash = 5381;
  int c;

  while ((c = *puchar_a++))
    hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

  return hash;
}

int compare_function_strings(void *a, void *b) {
  char *str_a = (char *)a;
  char *str_b = (char *)b;

  return strcmp(str_a, str_b);
}

int *number(int x) {
  int *nr = malloc(sizeof(int));
  memcpy(nr, &x, sizeof(int));
  return nr;
}

char *add_two_strings(const char *a, const char *b) {
  size_t len_a, len_b;
  char *result;
  if (!a || !b) {
    return NULL;
  }
  len_a = strlen(a);
  len_b = strlen(b);
  result = malloc(len_a + len_b + 1);
  DIE(!result, "String malloc");
  memcpy(result, a, len_a);
  memcpy(result + len_a, b, len_b);
  result[len_a + len_b] = '\0';
  return result;
}
