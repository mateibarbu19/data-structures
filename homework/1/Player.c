// Copyright 2020 Barbu Matei (matei.barbu1905@stud.acs.upb.ro)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./Player.h"
#include "./DoublyLinkedList.h"

/* Functia aloca si initializeaza o lista dublu inlantuita pentru player */
struct DoublyLinkedList *start_player_list() {
  struct DoublyLinkedList *list = malloc(sizeof(struct DoublyLinkedList));
  if (list == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei\n");
  }
  init_list(list);
  return list;
}

/* Functia adauga metadatele unui fisier in lista player-ului, dupa cele trei
 * moduri FIRST, LAST si CURR.
 */
void player_add(struct DoublyLinkedList *list, int mode, char *s, FILE *out) {
  FILE *song;
  int song_position;
  struct Node *elem;
  struct Info *metadata;
  char *title;

  if (list == NULL || s == NULL) {
    return;
  }

  song = open_dir_file(DATABASE_PATH, s, "rb");
  if (song == NULL) {
    return;
  }
  title = title_name(song);

  /* Tratam mai jos cazurile de exceptie */
  if (mode == CURR) {
    if (list->size == 0) {
      free(title);
      fclose(song);
      return;
    }
    elem = get_nth_node(list, list->cursor);
    if (strcmp(((struct Info *)elem->data)->Title, title) == 0) {
      free(title);
      fclose(song);
      return;
    }
  }

  song_position = find_song_in_player(list, title);
  if (song_position != -1) {
    /* Cantecul se afla deja in player */
    player_del(list, SPECIAL, s, out);
  }
  free(title);

  metadata = malloc(sizeof(struct Info));
  if (metadata == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.\n");
    return;
  }
  fill_metadata(metadata, song);
  fclose(song);

  if (mode == FIRST) {
    add_nth_node(list, 0, metadata);
    list->cursor++; /* s-a mutat cursorul la dreapta cu o pozitie */
  } else if (mode == LAST) {
    add_nth_node(list, list->size, metadata);
  } else if (mode == CURR) {
    /* adaugam pe pozitia urmatoare */
    add_nth_node(list, list->cursor + 1, metadata);
  }
  if (list->size == 1) {
    list->cursor = 0;
    /* daca lista era goala, punem cursorul pe prima pozitie */
  }
}

/* Functia sterge si dealoca datele unui fisier din lista player-ului, dupa cele
 * trei moduri FIRST, LAST, CURR, si SPECIAL.
 */
void player_del(struct DoublyLinkedList *list, int mode, char *s, FILE *out) {
  struct Node *old_node;
  int old_cursor, song_position;
  char *title;
  FILE *song;

  if (list == NULL) {
    return;
  }

  /* Tratam cazurile de exceptie */
  if ((mode == FIRST || mode == LAST) && list->size == 0) {
    if (out != NULL) {
      fprintf(out, "Error: delete from empty playlist\n");
    }
    return;
  }
  if (mode == CURR && list->size == 0) {
    if (out != NULL) {
      fprintf(out, "Error: no track playing\n");
    }
    return;
  }

  if (mode == FIRST) {
    if (list->cursor == 0) {
      list->cursor = 1; /* mutam cursorul la al doilea nod */
    }
    old_node = remove_nth_node(list, 0);
    list->cursor--; /* mutam cursorul la stanga */
  } else if (mode == LAST) {
    if (list->cursor == list->size - 1) {
      list->cursor--; /* mutam cursorul la stanga */
    }
    old_node = remove_nth_node(list, list->size - 1);
  } else if (mode == CURR) {
    old_cursor = list->cursor;
    if (list->cursor == list->size - 1) {
      list->cursor--; /* mutam cursorul la stanga */
    }
    old_node = remove_nth_node(list, old_cursor);
  } else if (mode == SPECIAL) {
    song = open_dir_file(DATABASE_PATH, s, "rb");
    if (song == NULL) {
      return;
    }
    title = title_name(song);
    fclose(song);
    song_position = find_song_in_player(list, title);
    if (song_position == -1) {
      if (out != NULL) {
        fprintf(out, "Error: no song found to delete\n");
      }
      free(title);
      return;
    }
    if (list->cursor == song_position) {
      if (list->cursor == list->size - 1) {
        list->cursor--; /* mutam cursorul la stanga */
      }
    } else if (song_position < list->cursor) {
      list->cursor--; /* mutam cursorul la stanga */
    }
    old_node = remove_nth_node(list, song_position);
    free(title);
  }
  free(old_node->data);
  free(old_node);
}

/* Functia muta cursorul cu move pozitii, atat timp cat nu depaseste
  dimensiunile listei.
 */
void player_move(struct DoublyLinkedList *list, int move, FILE *out) {
  if (list == NULL) {
    return;
  }
  if (list->size == 0 && out != NULL) {
    fprintf(out, "Error: no track playing\n");
  }
  if (0 <= list->cursor + move && list->cursor + move < list->size) {
    list->cursor += move;
  }
}

/* Functia afiseaza datele din lista intr-un fisier, dupa cele patru
 * moduri FIRST, LAST, CURR, si SPECIAL, intr-un format specific.
 */
void player_show(struct DoublyLinkedList *list, int mode, FILE *out) {
  struct Node *elem;

  if (list == NULL || out == NULL) {
    return;
  }

  if (mode != SPECIAL) {
    if (mode == FIRST) {
      elem = list->head;
    } else if (mode == LAST) {
      elem = list->tail;
    } else if (mode == CURR) {
      elem = get_nth_node(list, list->cursor);
    }
    if (elem == NULL) {
      fprintf(out, "Error: show empty playlist\n");
    } else {
      print_data(elem->data, out);
    }
  } else {
    fputc('[', out);
    for (elem = list->head; elem != NULL; elem = elem->next) {
      fputs(((struct Info *)elem->data)->Title, out);
      if (elem != list->tail) {
        fprintf(out, "; ");
      }
    }
    fprintf(out, "]\n");
  }
}

/* Functia cauta in lista pozitia in care se afla datele unei melodii
 * cu titlu s. Daca nu a fost gasit nimic in lista sa intoarce -1.
 */
int find_song_in_player(struct DoublyLinkedList *list, char *s) {
  struct Node *elem;
  int i = 0;

  if (list == NULL || s == NULL) {
    return -1;
  }

  elem = list->head;
  while (i < list->size && strcmp(((struct Info *)elem->data)->Title, s) != 0) {
    i++;
    elem = elem->next;
  }
  if (i == list->size) {
    return -1;
  }
  return i;
}

/* Functia extrage din metadatele melodiei transmise ca parametru titlul */
char *title_name(FILE *song) {
  char *title;
  if (song == NULL) {
    return NULL;
  }
  title = malloc(31);
  if (title == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.\n");
    return NULL;
  }
  fseek(song, -94, SEEK_END);
  fread(title, sizeof(char), 30, song);
  title[30] = '\0';
  return title;
}

/* Functia completeaza metadatele din fisierul song, in campul field */
void fill_metadata(struct Info *field, FILE *song) {
  if (song == NULL || field == NULL) {
    return;
  }
  fseek(song, -94, SEEK_END);
  fread(field->Title, sizeof(char), 30, song);
  fread(field->Artist, sizeof(char), 30, song);
  fread(field->Album, sizeof(char), 30, song);
  fread(field->Year, sizeof(char), 4, song);
  field->Title[30] = '\0';
  field->Artist[30] = '\0';
  field->Album[30] = '\0';
  field->Year[4] = '\0';
}

/* Functia afiseaza datele din camp, in fisierul de iesire sub un anumit format
 */
void print_data(struct Info *field, FILE *fout) {
  if (field == NULL || fout == NULL) {
    return;
  }
  fprintf(fout, "Title: ");
  fputs(field->Title, fout);
  fputc('\n', fout);
  fprintf(fout, "Artist: ");
  fputs(field->Artist, fout);
  fputc('\n', fout);
  fprintf(fout, "Album: ");
  fputs(field->Album, fout);
  fputc('\n', fout);
  fprintf(fout, "Year: ");
  fputs(field->Year, fout);
  fputc('\n', fout);
}

/* Functia deschide un fisier din folderul path.
 * Path trebuie sa contina separatorul de cale la final.
 */
FILE *open_dir_file(char *path, char *name, char *mode) {
  FILE *thisfile;
  char *new_path;
  if (path == NULL || name == NULL || mode == NULL) {
    return NULL;
  }
  new_path = malloc(strlen(path) + strlen(name) + 1);
  if (new_path == NULL) {
    fprintf(stderr, "A aparut o eroare la alocarea memoriei.\n");
    return NULL;
  }
  snprintf(new_path, strlen(path) + 1, "%s", path);
  snprintf(new_path + strlen(path), strlen(name) + 1, "%s", name);
  thisfile = fopen(new_path, mode);
  free(new_path);
  return thisfile;
}
