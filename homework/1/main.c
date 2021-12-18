// Copyright 2020 Barbu Matei (matei.barbu1905@stud.acs.upb.ro)
#include "DoublyLinkedList.h"
#include "Player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int interpretor(FILE *in, FILE *out);

int main(int argc, char *argv[]) {
  FILE *in, *out;
  int ok;

  if (argc != 3) {
    printf("Programul nu a primit parametrii corecti.\n");
    return -1;
  }
  in = fopen(argv[1], "rt");
  out = fopen(argv[2], "wt");

  ok = interpretor(in, out);

  fclose(in);
  fclose(out);
  return ok;
}

int interpretor(FILE *in, FILE *out) {
  int q, i;
  char comenzi[LINE_LENGTH];
  struct DoublyLinkedList *list;

  list = start_player_list();

  fscanf(in, "%d", &q);
  fgetc(in); /* citim '\n' de dupa q */
  for (i = 0; i < q; i++) {
    fgets(comenzi, LINE_LENGTH, in);
    comenzi[strlen(comenzi) - 1] = '\0';
    /* puts(comenzi); */

    if (strstr(comenzi, "ADD_FIRST ") == comenzi) {
      player_add(list, FIRST, comenzi + strlen("ADD_FIRST "), out);
      /* comenzi + strlen("ADD_FIRST ") = pozitia pe care se afla numele
       * fisierului melodiei */
    } else if (strstr(comenzi, "ADD_LAST ") == comenzi) {
      player_add(list, LAST, comenzi + strlen("ADD_LAST "), out);
    } else if (strstr(comenzi, "ADD_AFTER ") == comenzi) {
      player_add(list, CURR, comenzi + strlen("ADD_AFTER "), out);
    } else if (strcmp(comenzi, "DEL_FIRST") == 0) {
      player_del(list, FIRST, NULL, out);
    } else if (strcmp(comenzi, "DEL_LAST") == 0) {
      player_del(list, LAST, NULL, out);
    } else if (strcmp(comenzi, "DEL_CURR") == 0) {
      player_del(list, CURR, NULL, out);
    } else if (strstr(comenzi, "DEL_SONG ") == comenzi) {
      player_del(list, SPECIAL, comenzi + strlen("DEL_SONG "), out);
    } else if (strcmp(comenzi, "MOVE_NEXT") == 0) {
      player_move(list, +1, out);
    } else if (strcmp(comenzi, "MOVE_PREV") == 0) {
      player_move(list, -1, out);
    } else if (strcmp(comenzi, "SHOW_FIRST") == 0) {
      player_show(list, FIRST, out);
    } else if (strcmp(comenzi, "SHOW_LAST") == 0) {
      player_show(list, LAST, out);
    } else if (strcmp(comenzi, "SHOW_CURR") == 0) {
      player_show(list, CURR, out);
    } else if (strcmp(comenzi, "SHOW_PLAYLIST") == 0) {
      player_show(list, SPECIAL, out);
    }
  }

  free_list(list, 1);
  return 0;
}
