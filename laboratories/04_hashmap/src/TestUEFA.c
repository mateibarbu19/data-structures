#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Hashtable.h"

struct InfoFotbalist {
    char *nume;
    char *nationalitate;
    char *pozitie;
    char *un_trofeu;
    int rating;
    int a;
    int b;
};

void print_Fotbalist(struct InfoFotbalist *f_info) {
    if (f_info == NULL) {
        printf("Fotbalist inexistent!\n");
        return;
    }
    printf("* Nume: %s | ", f_info->nume);
    printf("* Nationalitate: %s | ", f_info->nationalitate);
    printf("* Pozitie: %s | ", f_info->pozitie);
    printf("* Un trofeu: %s | ", f_info->un_trofeu);
    printf("* Rating: %d | ", f_info->rating);
    printf("\n");
}

int main() {
    struct Hashtable *ht_fotbalisti, *ht;
    struct InfoFotbalist informatii_fotbalisti[5];
    int i, j;
    struct LinkedList *list;
    struct Node *curr, *past;

    /* Initializare structuri informatii fotbalisti. */

    informatii_fotbalisti[0].nume = "Messi";
    informatii_fotbalisti[0].nationalitate = "Argentinian";
    informatii_fotbalisti[0].pozitie = "CF";
    informatii_fotbalisti[0].un_trofeu = "Ballon d'Or";
    informatii_fotbalisti[0].rating = 300;
    informatii_fotbalisti[0].a = 697;
    informatii_fotbalisti[0].a = 289;

    informatii_fotbalisti[1].nume = "Ronaldo";
    informatii_fotbalisti[1].nationalitate = "Portughez";
    informatii_fotbalisti[1].pozitie = "CF";
    informatii_fotbalisti[1].un_trofeu = "Ballon d'Or";
    informatii_fotbalisti[1].rating = 300;
    informatii_fotbalisti[1].a = 725;
    informatii_fotbalisti[1].a = 220;

    informatii_fotbalisti[2].nume = "Neymar";
    informatii_fotbalisti[2].nationalitate = "Brazilian";
    informatii_fotbalisti[2].pozitie = "SS";
    informatii_fotbalisti[2].un_trofeu = "Ballon d'Or";
    informatii_fotbalisti[2].rating = 250;
    informatii_fotbalisti[2].a = 187;
    informatii_fotbalisti[2].a = 99;

    informatii_fotbalisti[3].nume = "Pele";
    informatii_fotbalisti[3].nationalitate = "Brazilian";
    informatii_fotbalisti[3].pozitie = "CF";
    informatii_fotbalisti[3].un_trofeu = "Ballon d'Or";
    informatii_fotbalisti[3].rating = 350;
    informatii_fotbalisti[3].a = 643;
    informatii_fotbalisti[3].a = 301;

    informatii_fotbalisti[4].nume = "Adrian Popa";
    informatii_fotbalisti[4].nationalitate = "Roman";
    informatii_fotbalisti[4].pozitie = "LW";
    informatii_fotbalisti[4].un_trofeu = "Cupa campionilor";
    informatii_fotbalisti[4].rating = 199;
    informatii_fotbalisti[4].a = 51;
    informatii_fotbalisti[4].a = 22;

    /* Initializare hashtable. */
    ht_fotbalisti = malloc(sizeof(struct Hashtable));
    init_ht(ht_fotbalisti, 10, hash_function_string, compare_function_strings);

    put(ht_fotbalisti, "Messi", strlen("Messi") + 1, &informatii_fotbalisti[0]);
    put(ht_fotbalisti, "Ronaldo", strlen("Ronaldo") + 1, &informatii_fotbalisti[1]);
    put(ht_fotbalisti, "Neymar", strlen("Neymar") + 1, &informatii_fotbalisti[2]);
    put(ht_fotbalisti, "Pele", strlen("Pele") + 1, &informatii_fotbalisti[3]);
    put(ht_fotbalisti, "Adrian Popa", strlen("Adrian Popa") + 1, &informatii_fotbalisti[4]);

    print_Fotbalist(get(ht_fotbalisti, "Messi"));
    print_Fotbalist(get(ht_fotbalisti, "Ronaldo"));
    print_Fotbalist(get(ht_fotbalisti, "Neymar"));
    print_Fotbalist(get(ht_fotbalisti, "Pele"));
    print_Fotbalist(get(ht_fotbalisti, "Adrian Popa"));

    ht = ht_fotbalisti;
    for (i = 0; i < ht->hmax; i++) {
      list = ht->buckets + i;
      curr = list->head;
      j = 0;
      while (curr != NULL) {
        if (((struct InfoFotbalist*)((struct info*)curr->data)->value)->rating < 200) {
          free(((struct info*)curr->data)->key);
          free(curr->data);
          remove_nth_node(ht->buckets + i, j);
          past = curr;
          curr = curr->next;
          free(past);
          ht->size--;

        }
        else {
          j++;
          curr = curr->next;
        }
      }
    }

    printf("\n");
    print_Fotbalist(get(ht_fotbalisti, "Messi"));
    print_Fotbalist(get(ht_fotbalisti, "Ronaldo"));
    print_Fotbalist(get(ht_fotbalisti, "Neymar"));
    print_Fotbalist(get(ht_fotbalisti, "Pele"));
    printf("%d\n", has_key(ht_fotbalisti, "Adrian Popa"));

    free_ht(ht_fotbalisti);

    return 0;
}

