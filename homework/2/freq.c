/* Copyright 2020 Barbu Matei (matei.barbu1905@stud.acs.upb.ro) */
#include <stdio.h>
#include <stdlib.h>

#define MAX 2000000 + 1
int main() {
  unsigned char *freq;
  int nr, nr_max, nr_min;

  freq = calloc(MAX, sizeof(unsigned char));
  if (freq == NULL) {
    perror("A aparut o eroare la alocarea memoriei.\n");
    return 2;
  }

  nr_max = -1;
  nr_min = MAX;
  while (scanf("%d", &nr) == 1) {
    freq[nr]++;
    if (nr > nr_max) {
      nr_max = nr;
    }
    if (nr < nr_min) {
      nr_min = nr;
    }
  }
  /* parcurgem valorile din intervalul de numere [minim, maxim] */
  for (nr = nr_min; nr <= nr_max; nr++) {
    if (freq[nr] != 0) {
      /* afisam frecventa doar numerelor care au fost citite */
      printf("%d %d\n", nr, freq[nr]);
    }
  }
  free(freq);
  return 0;
}
