#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  struct LinkedList *list, *odd, *even;
  struct Node *currNode;
  int numbers[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  list = malloc(sizeof(struct LinkedList));
  init_list(list);

  add_nth_node(list, 0, &numbers[10]);
  printf("%d\n", *((int *)(list->head->data)));
  currNode = remove_nth_node(list, 0);
  printf("#1:\n");
  printf("Output: %d\n", *((int *)(currNode->data)));
  printf("Output asteptat: 10\n\n");
  free(currNode);

  add_nth_node(list, 0, &numbers[8]);
  currNode = remove_nth_node(list, 0);
  printf("#2:\n");
  printf("Output: %d\n", *((int *)(currNode->data)));
  printf("Output asteptat: 8\n\n");
  free(currNode);

  add_nth_node(list, 0, &numbers[0]);
  add_nth_node(list, 10, &numbers[1]);
  add_nth_node(list, 20, &numbers[2]);
  add_nth_node(list, 30, &numbers[3]);
  add_nth_node(list, 40, &numbers[4]);
  printf("#3:\n");
  printf("Output: ");
  print_int_linkedlist(list);
  printf("Output asteptat: 0 1 2 3 4\n\n");

  add_nth_node(list, 0, &numbers[0]);
  add_nth_node(list, 0, &numbers[1]);
  add_nth_node(list, 0, &numbers[2]);
  add_nth_node(list, 0, &numbers[3]);
  add_nth_node(list, 0, &numbers[4]);
  printf("#4:\n");
  printf("Output: ");
  print_int_linkedlist(list);
  printf("Output asteptat: 4 3 2 1 0 0 1 2 3 4\n\n");

  currNode = remove_nth_node(list, 0);
  free(currNode);
  printf("#5:\n");
  printf("Output: ");
  print_int_linkedlist(list);
  printf("Output asteptat: 3 2 1 0 0 1 2 3 4\n\n");

  currNode = remove_nth_node(list, 8);
  free(currNode);
  printf("#6:\n");
  printf("Output: ");
  print_int_linkedlist(list);
  printf("Output asteptat: 3 2 1 0 0 1 2 3\n\n");

  currNode = remove_nth_node(list, 0);
  free(currNode);
  currNode = remove_nth_node(list, 0);
  free(currNode);
  currNode = remove_nth_node(list, 0);
  printf("#7:\n");
  printf("Output: ");
  printf("%d\n", *((int *)currNode->data));
  free(currNode);
  printf("Output asteptat: 1\n\n");

  currNode = remove_nth_node(list, 40);
  free(currNode);
  currNode = remove_nth_node(list, 30);
  free(currNode);
  currNode = remove_nth_node(list, 20);
  free(currNode);
  currNode = remove_nth_node(list, 10);
  free(currNode);
  currNode = remove_nth_node(list, 0);
  free(currNode);
  add_nth_node(list, 0, &numbers[3]);
  add_nth_node(list, 1, &numbers[3]);
  add_nth_node(list, 2, &numbers[7]);
  add_nth_node(list, 0, &numbers[1]);
  printf("#8:\n");
  printf("Output: ");
  print_int_linkedlist(list);
  printf("Output asteptat: 1 3 3 7\n\n");

  currNode = remove_nth_node(list, 2);
  free(currNode);
  currNode = remove_nth_node(list, 1);
  free(currNode);
  printf("#9:\n");
  printf("Output: ");
  print_int_linkedlist(list);
  printf("Output asteptat: 1 7\n\n");

  free_list(&list);

  list = malloc(sizeof(struct LinkedList));
  init_list(list);

  add_nth_node(list, 0, "mere");
  add_nth_node(list, 0, "are");
  add_nth_node(list, 0, "Ana");
  printf("#10:\n");
  printf("Output: ");
  print_string_linkedlist(list);
  printf("Output asteptat: Ana are mere\n\n");

  free_list(&list);


  /** DE AICI IN JOS TESTEZ PROBLEMA 2 **/

  list = malloc(sizeof(struct LinkedList));
  odd = malloc(sizeof(struct LinkedList));
  even = malloc(sizeof(struct LinkedList));
  init_list(list);

  add_nth_node(list, 0, &numbers[0]);
  add_nth_node(list, 1, &numbers[1]);
  add_nth_node(list, 2, &numbers[2]);
  add_nth_node(list, 3, &numbers[3]);
  add_nth_node(list, 4, &numbers[4]);
  add_nth_node(list, 5, &numbers[5]);
  add_nth_node(list, 6, &numbers[6]);
  add_nth_node(list, 7, &numbers[7]);
  add_nth_node(list, 8, &numbers[8]);
  add_nth_node(list, 9, &numbers[9]);
  add_nth_node(list, 10, &numbers[10]);
  split_list(list, odd, even);

  printf("#11:\n");
  printf("Output:\n");
  printf("  Lista: ");
  print_int_linkedlist(list);
  printf("  Subsirul cu indici impari: ");
  print_int_linkedlist(odd);
  printf("  Subsirul cu indici pari: ");
  print_int_linkedlist(even);

  printf("Output asteptat:\n");
  printf("  Lista: 0 1 2 3 4 5 6 7 8 9 10\n");
  printf("  Subsirul cu indici impari: 0 2 4 6 8 10\n");
  printf("  Subsirul cu indici pari: 1 3 5 7 9\n");

  free_list(&list);
  free_list(&odd);
  free_list(&even);

  return 0;
}
