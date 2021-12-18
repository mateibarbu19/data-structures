/* Copyright 2020 Barbu Matei (matei.barbu1905@stud.acs.upb.ro) */
#include <stdio.h>
#include <stdlib.h>

#define NR_BITS_IN_INT (8 * sizeof(unsigned int))

#define MAXB 11
#define MAXM (1 << MAXB)
#define MASK_LAST_MAXB_BITS (MAXM - 1)
#define MASK_FIRST_MAXB_BITS (MASK_LAST_MAXB_BITS << (NR_BITS_IN_INT - MAXB))

#define ALPHA (0.7213 / (1.0 + (1.079 / MAXM)))

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

int nr_of_leading_0s(unsigned int nr) {
  int count = 0, bit;
  /* pozitia primului bit semnificativ */
  int pos = NR_BITS_IN_INT - 1;

  bit = (nr & (1 << pos)) >> pos; /* bit-ul de pe pozitia pos */
  while (bit == 0 && pos >=0) {
    pos--;
    count++;
    bit = (nr & (1 << pos)) >> pos;
  }
  return count;
}

int main(int argc, char *argv[]) {
  FILE *fin;
  int nr, *buckets, i, x;
  unsigned int j, key, last_bits;
  double Z, sum, E;

  if (argc != 2) {
    printf("Nu a fost transmis doar un parametru.\n");
    return 0;
  }
  fin = fopen(argv[1], "rt");
  if (fin == NULL) {
    return 1;
  }

  buckets = calloc(MAXM, sizeof(int));
  if (buckets == NULL) {
    perror("A aparut o eroare la alocarea memoriei.\n");
    return 2;
  }
  while (fscanf(fin, "%d", &nr) == 1) {
    key = hash_function_int(&nr);
    j = (key & MASK_FIRST_MAXB_BITS) >> (NR_BITS_IN_INT - MAXB);
    last_bits = key & (~MASK_FIRST_MAXB_BITS);
    x = nr_of_leading_0s(last_bits) - MAXB;

    if (x > buckets[j]) {
      buckets[j] = x;
    }
  }

  sum = 0;
  for (i = 0; i < MAXM; i++) {
    sum += 1.0 / (1 << buckets[i]);
  }
  Z = 1.0 / sum;
  E = ALPHA * MAXM * MAXM * Z;
  printf("%d\n", (int) E * 2);

  free(buckets);
  fclose(fin);
  return 0;
}
