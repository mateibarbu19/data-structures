// Copyright 2020 Barbu Matei (matei.barbu1905@stud.acs.upb.ro)
#ifndef PLAYER_H_
#define PLAYER_H_

#include <stdio.h>

#define FIRST 0
#define LAST 2
#define CURR 1
#define SPECIAL 3
#define DATABASE_PATH "songs/"
#define LINE_LENGTH 102

struct Info {
  char Title[31];
  char Artist[31];
  char Album[31];
  char Year[5];
};

struct DoublyLinkedList *start_player_list();

void player_add(struct DoublyLinkedList *list, int mode, char *s, FILE *out);

void player_del(struct DoublyLinkedList *list, int mode, char *s, FILE *out);

void player_move(struct DoublyLinkedList *list, int move, FILE *out);

void player_show(struct DoublyLinkedList *list, int mode, FILE *out);

int find_song_in_player(struct DoublyLinkedList *list, char *s);

char *title_name(FILE *song);

void fill_metadata(struct Info *field, FILE *song);

void print_data(struct Info *field, FILE *fout);

FILE *open_dir_file(char *path, char *name, char *mode);

#endif  // PLAYER_H_
