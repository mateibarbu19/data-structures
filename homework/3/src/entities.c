#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "Hashtable.h"
#include "LinkedList.h"
#include "entities.h"
#include "tools.h"

Paper *init_paper(void) {
  Paper *info;
  info = malloc(sizeof(Paper));
  DIE(info == NULL, "Paper malloc");

  info->title = NULL;
  info->venue = NULL;
  info->year = -1;
  info->num_authors = -1;
  info->id = (int64_t)-1;
  info->references = NULL;
  info->num_refs = -1;
  info->authors = NULL;

  return info;
}

void destroy_paper(void *p) {
  Paper *info = (Paper *)p;
  free(info->title);
  free(info->venue);
  free(info->references);
  free(info->authors);
  free_list(info->sources);
  free_list(info->cited_by);
  free(info);
}

void attribute_to_paper(Paper *info, const char *title, const char *venue,
                        const int year, const int64_t id, const int num_authors,
                        const int64_t *references, const int num_refs) {
  if (!info || !title || !venue || !references) {
    fprintf(stderr, "Cannot attribute to paper.\n");
    return;
  }
  if (info->title != NULL) {
    fprintf(stderr, "Warning: Attributing new paper to an old one.\n");
    return;
  }
  info->title = my_strdup(title);
  DIE(info->title == NULL, "Paper title malloc");

  info->venue = my_strdup(venue);
  DIE(info->venue == NULL, "Paper title malloc");

  info->year = year;
  info->id = id;

  info->num_authors = num_authors;
  info->authors = malloc(num_authors * sizeof(Author *));

  info->references = malloc(num_refs * sizeof(int64_t));
  DIE(info->references == NULL, "Paper references malloc");
  DIE(!memcpy(info->references, references, num_refs * sizeof(int64_t)),
      "Paper references memcpy");
  info->num_refs = num_refs;
  info->sources = init_list();
  info->cited_by = init_list();
}

Author *init_author(void) {
  Author *human;
  human = malloc(sizeof(Author));
  DIE(human == NULL, "Author malloc");

  human->name = NULL;
  human->id = (int64_t)-1;
  human->institution = NULL;
  human->papers = NULL;
  return human;
}

void destroy_author(void *p) {
  Author *human = (Author *)p;
  free(human->name);
  free(human->institution);
  free_list(human->papers);
  free_list(human->colleagues);
  free_ht(human->knows_who);
  free(human);
}

void attribute_to_author(Author *human, const char *name, const int64_t id,
                         const char *institution) {
  if (!human || !name || !institution) {
    fprintf(stderr, "Cannot attribute to paper.\n");
    return;
  }
  if (human->name != NULL) {
    fprintf(stderr, "Warning: Attributing new author to an old one.\n");
    return;
  }
  human->name = my_strdup(name);
  DIE(human->name == NULL, "Author name malloc");

  human->id = id;

  human->institution = my_strdup(institution);
  DIE(human->name == NULL, "Author institution malloc");

  human->papers = init_list();
  human->colleagues = init_list();
  human->knows_who =
      init_ht(MAX_COLLEAGUES, hash_function_int64, compare_function_ints);
}
