#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "publications.h"
#include "src/ArrayList.h"
#include "src/Hashtable.h"
#include "src/Heap.h"
#include "src/LinkedList.h"
#include "src/Queue.h"
#include "src/entities.h"
#include "src/tools.h"

#define FIELD_BUC 1000
#define FIELD_and_INS 50000

struct publications_data {
  Hashtable *all_papers;
  Hashtable *all_authors;
  Hashtable *waitting;
  Hashtable *venue_papers;
  Hashtable *field_papers;
  Hashtable *fldandinstit;
  /* frequence array */
  unsigned int *papers_in_year;
};

PublData *init_publ_data(void) {
  PublData *data = malloc(sizeof(PublData));
  DIE(data == NULL, "PublData init");

  data->all_papers =
      init_ht(MAX_PAPERS, hash_function_int64, compare_function_int64s);

  data->all_authors =
      init_ht(MAX_AUTHORS, hash_function_int64, compare_function_int64s);

  data->waitting =
      init_ht(MAX_PAPERS_MISSING, hash_function_int64, compare_function_int64s);

  data->venue_papers =
      init_ht(MAX_VENUES, hash_function_string, compare_function_strings);

  data->field_papers =
      init_ht(FIELD_BUC, hash_function_string, compare_function_strings);

  data->fldandinstit =
      init_ht(FIELD_and_INS, hash_function_string, compare_function_strings);

  data->papers_in_year = calloc(MAX_YEAR - MIN_YEAR + 1, sizeof(unsigned int));
  DIE(!data->papers_in_year, "PublData papers in year calloc");
  return data;
}

void destroy_publ_data(PublData *data) {
  if (!data) {
    return;
  }
  free_ht_and_values(data->all_papers, destroy_paper);
  free_ht_and_values(data->all_authors, destroy_author);
  free_ht_and_values(data->waitting, free_list);
  free_ht_and_values(data->venue_papers, free_list);
  free_ht_and_values(data->field_papers, free_array_list);
  free_ht_and_values(data->fldandinstit, free_list);
  free(data->papers_in_year);
  free(data);
}

/* This function sets pointers from a paper to it's authors, and back */
static void tie_paper_to_authors(PublData *data, Paper *info,
                                 const char **author_names,
                                 const int64_t *author_ids,
                                 const char **institutions,
                                 const int num_authors) {
  int i;
  Author *human, *someone;
  if (!data || !info || !author_names || !author_ids || !institutions) {
    return;
  }

  for (i = 0; i < num_authors; i++) {
    human = init_author();
    attribute_to_author(human, author_names[i], author_ids[i], institutions[i]);
    someone = get_ht_entry(data->all_authors, &human->id);
    if (someone == NULL) {
      put_ht_pair(data->all_authors, &human->id, sizeof(int64_t), human);
      someone = human;
      info->authors[i] = human;
      add_nth_node(human->papers, human->papers->size, info);
    } else {
      destroy_author(human); /* sorry about that */
      info->authors[i] = someone;
      add_nth_node(someone->papers, someone->papers->size, info);
    }
  }
}

/**
 * This is done inefficently, and should be improved
 * We use a Hashtable, with the keys being the field concatenated with
 * the institution, so that we retain at that key only the unique authors
 * that have ties to this criteria.
 */
static void set_fields_and_instit(PublData *data, Paper *info,
                                  const char **institutions,
                                  const int num_authors, const char **fields,
                                  const int num_fields) {
  int i, j, found;
  char *key;
  LinkedList *list;
  Author *human;
  Node *curr;
  if (!data || !data->fldandinstit || !info || !institutions || !fields) {
    return;
  }
  /* For the institutiions of the authors */
  for (i = 0; i < num_authors; i++) {
    /* For the fields of the paper */
    for (j = 0; j < num_fields; j++) {
      /* make a unique key of the instituion and filed */
      key = add_two_strings(institutions[i], fields[j]);
      /* get the list of authors with this key */
      list = get_ht_entry(data->fldandinstit, key);
      if (list == NULL) {
        /* if no authors are found, add the first one */
        list = init_list();
        add_nth_node(list, 0, info->authors[i]);
        put_ht_pair(data->fldandinstit, key, strlen(key) + 1, list);
      } else {
        /* if we have a list of authors with this institution and filed */
        /* check to see if the current author is in this list */
        found = 0;
        curr = list->head;
        while (curr && !found) {
          human = curr->data;
          if (human->id == info->authors[i]->id) {
            found = 1;
          }
          curr = curr->next;
        }
        if (!found) {
          add_nth_node(list, list->size, info->authors[i]);
        }
      }
      free(key);
    }
  }
}

/**
  * We use this function to build a "graph" in which every author, hash a
  * pointer to all his colleagues.
  */
static void set_colleagues(Paper *info) {
  int i, j;
  Author *human, *other_h, *someone;

  for (i = 0; i < info->num_authors; i++) {
    human = info->authors[i];
    for (j = 0; j < info->num_authors; j++) {
      if (j != i) {
        other_h = info->authors[j];
        someone = get_ht_entry(human->knows_who, &other_h->id);
        if (someone == NULL) {
          add_nth_node(human->colleagues, human->colleagues->size, other_h);
          put_ht_pair(human->knows_who, &other_h, sizeof(int64_t), other_h);
        }
      }
    }
  }
}

/**
  * We use this function to tie all papers to their sources, and back.
  * If we do not find a source in the Hashtable of papers, we add the
  * current paper to the sources watting list in a watting Hashtable.
  */
static void check_citations(PublData *data, Paper *info) {
  int i;
  Paper *source, *later;
  LinkedList *influenced;
  Node *curr;
  if (!data || !info) {
    return;
  }

  for (i = 0; i < info->num_refs; i++) {
    source = get_ht_entry(data->all_papers, &info->references[i]);
    if (source != NULL) {
      add_nth_node(info->sources, info->sources->size, source);
      add_nth_node(source->cited_by, source->cited_by->size, info);
    } else {
      influenced = get_ht_entry(data->waitting, &info->references[i]);
      if (influenced == NULL) {
        influenced = init_list();
        add_nth_node(influenced, 0, info);
        put_ht_pair(data->waitting, &info->references[i], sizeof(int64_t),
                    influenced);
      } else {
        add_nth_node(influenced, influenced->size, info);
      }
    }
  }

  influenced = get_ht_entry(data->waitting, &info->id);
  if (influenced) {
    for (curr = influenced->head; curr; curr = curr->next) {
      later = curr->data;
      add_nth_node(later->sources, later->sources->size, info);
      add_nth_node(info->cited_by, info->cited_by->size, later);
    }
    remove_ht_entry(data->waitting, &info->id);
    free_list(influenced);
  }
}

/* This function ties all papers with there venues which are
 * are stored in a Hashtable */
static void add_to_venue_list(PublData *data, Paper *info) {
  LinkedList *collection;
  if (!data || !info) {
    return;
  }

  collection = get_ht_entry(data->venue_papers, info->venue);
  if (collection) {
    add_nth_node(collection, collection->size, info);
  } else {
    collection = init_list();
    add_nth_node(collection, 0, info);
    put_ht_pair(data->venue_papers, info->venue, strlen(info->venue) + 1,
                collection);
  }
}

/* For each paper we add it to the corresponding fields
 * in a Field Hashtable */
static void add_to_field_array_list(PublData *data, Paper *info,
                                    const int num_fields, const char **fields) {
  ArrayList *collection;
  char *s;
  int i;
  if (!data || !info || !fields || num_fields < 1) {
    return;
  }
  for (i = 0; i < num_fields; i++) {
    s = my_strdup(fields[i]);
    collection = get_ht_entry(data->field_papers, s);
    if (!collection) {
      collection = init_array_list();
      put_ht_pair(data->field_papers, s, strlen(s) + 1, collection);
    }
    add_array_nth_node(collection, collection->size, info);
    free(s);
  }
}

void add_paper(PublData *data, const char *title, const char *venue,
               const int year, const char **author_names,
               const int64_t *author_ids, const char **institutions,
               const int num_authors, const char **fields, const int num_fields,
               const int64_t id, const int64_t *references,
               const int num_refs) {
  Paper *info;
  if (!data || !title || !venue || !author_names || !author_ids ||
      !institutions || !fields || !references) {
    return;
  }
  info = init_paper();
  attribute_to_paper(info, title, venue, year, id, num_authors, references,
                     num_refs);
  put_ht_pair(data->all_papers, &info->id, sizeof(int64_t), info);
  tie_paper_to_authors(data, info, author_names, author_ids, institutions,
                       num_authors);
  set_colleagues(info);
  check_citations(data, info);
  add_to_venue_list(data, info);
  add_to_field_array_list(data, info, num_fields, fields);
  set_fields_and_instit(data, info, institutions, num_authors, fields,
                        num_fields);
  data->papers_in_year[year - MIN_YEAR]++;
}

char *get_oldest_influence(PublData *data, const int64_t id_paper) {
  Paper *info, *source, *other_s, *oldest;
  Node *curr;
  Queue *q;
  Hashtable *seen_paper;
  int64_t key = id_paper;

  if (!data || !data->all_papers) {
    return NULL;
  }
  info = get_ht_entry(data->all_papers, &key);

  oldest = NULL;
  if (info) {
    q = init_queue();
    seen_paper =
        init_ht(MAX_PAPERS >> 5, hash_function_int64, compare_function_int64s);

    put_ht_pair(seen_paper, &info->id, sizeof(int64_t), number(1));
    for (curr = info->sources->head; curr != NULL; curr = curr->next) {
      other_s = curr->data;
      oldest = other_s;
      /* we just need to take a first paper as the oldest */
      /* and later compare it to other ones */
      if (!has_ht_key(seen_paper, &other_s->id)) {
        enqueue(q, other_s);
        put_ht_pair(seen_paper, &other_s->id, sizeof(int64_t), number(1));
      }
    }

    while (!is_empty_queue(q)) {
      source = front(q);
      dequeue(q);

      if (source->year < oldest->year) {
        oldest = source;
      } else if (source->year == oldest->year) {
        if (source->cited_by->size > oldest->cited_by->size) {
          oldest = source;
        } else if (source->cited_by->size == oldest->cited_by->size) {
          if (source->id < oldest->id) {
            oldest = source;
          }
        }
      }

      for (curr = source->sources->head; curr != NULL; curr = curr->next) {
        other_s = curr->data;
        if (!has_ht_key(seen_paper, &other_s->id)) {
          enqueue(q, other_s);
          put_ht_pair(seen_paper, &other_s->id, sizeof(int64_t), number(1));
        }
      }
    }

    free_ht_and_values(seen_paper, free);
    free_queue(q);
  }
  if (oldest) {
    return oldest->title;
  } else {
    return "None";
  }
}

float get_venue_impact_factor(PublData *data, const char *venue) {
  LinkedList *collection;
  Paper *info;
  Node *curr;
  int total_nr_of_citations = 0;
  char *publication = my_strdup(venue);
  if (!data || !data->venue_papers || !venue) {
    return 0.f;
  }

  collection = get_ht_entry(data->venue_papers, publication);
  free(publication);
  if (!collection) {
    return 0.f;
  } else {
    for (curr = collection->head; curr; curr = curr->next) {
      info = curr->data;
      total_nr_of_citations += info->cited_by->size;
    }
    return (float)total_nr_of_citations / collection->size;
  }
}

int get_number_of_influenced_papers(PublData *data, const int64_t id_paper,
                                    const int distance) {
  Paper *info, *source, *other_s;
  Node *curr;
  Queue *q;
  LinkedList *dist;
  Hashtable *seen_paper;
  int64_t key = id_paper;
  int ok, *d, count;

  if (!data || !data->all_papers) {
    return -1;
  }
  source = get_ht_entry(data->all_papers, &key);
  count = 0;

  if (source) {
    q = init_queue();
    dist = init_list();
    seen_paper =
        init_ht(MAX_PAPERS >> 5, hash_function_int64, compare_function_int64s);

    enqueue(q, source);
    add_nth_node(dist, 0, number(0));
    put_ht_pair(seen_paper, &source->id, sizeof(int64_t), number(1));

    ok = 1;
    while (!is_empty_queue(q) && ok) {
      other_s = front(q);
      dequeue(q);
      d = dist->head->data;
      purge_nth_node(dist, 0);
      count++;

      for (curr = other_s->cited_by->head; curr != NULL; curr = curr->next) {
        info = curr->data;
        if (!has_ht_key(seen_paper, &info->id)) {
          enqueue(q, info);
          add_nth_node(dist, dist->size, number(*d + 1));
          put_ht_pair(seen_paper, &info->id, sizeof(int64_t), number(1));
        }
      }

      ok = (dist->head && *(int *)dist->head->data <= distance);
      free(d);
    }

    free_ht_and_values(seen_paper, free);
    purge_list(dist, free);
    free_queue(q);
  }
  return count - 1;
}

int get_erdos_distance(PublData *data, const int64_t id1, const int64_t id2) {
  Author *first, *human, *coll;
  Node *curr;
  Queue *q;
  LinkedList *dist;
  Hashtable *seen_author;
  int *d, ret;
  int64_t key = id1;

  if (!data || !data->all_authors) {
    return -1;
  }
  first = get_ht_entry(data->all_authors, &key);

  if (first) {
    q = init_queue();
    seen_author =
        init_ht(MAX_AUTHORS / 20, hash_function_int64, compare_function_int64s);
    dist = init_list();

    enqueue(q, first);
    add_nth_node(dist, 0, number(0));
    put_ht_pair(seen_author, &key, sizeof(int64_t), number(1));

    while (!is_empty_queue(q) && !(((Author *)front(q))->id == id2)) {
      human = front(q);
      d = dist->head->data;
      dequeue(q);
      purge_nth_node(dist, 0);

      for (curr = human->colleagues->head; curr != NULL; curr = curr->next) {
        coll = curr->data;
        if (!has_ht_key(seen_author, &coll->id)) {
          enqueue(q, coll);
          add_nth_node(dist, dist->size, number(*d + 1));
          put_ht_pair(seen_author, &coll->id, sizeof(int64_t), number(1));
        }
      }

      free(d);
    }
    if (!is_empty_queue(q) && ((Author *)front(q))->id == id2) {
      ret = *(int *)dist->head->data;
    } else {
      ret = -1;
    }
    purge_list(dist, free);
    free_ht_and_values(seen_author, free);
    free_queue(q);
  } else {
    ret = -1;
  }
  return ret;
}

static int cmp_papers_for_field(void *a, void *b) {
  Paper *Pa = a;
  Paper *Pb = b;
  if (!Pa || !Pb) {
    return 0;
  }
  if (Pa->cited_by->size > Pb->cited_by->size) {
    return 1;
  } else if (Pa->cited_by->size < Pb->cited_by->size) {
    return -1;
  } else {
    if (Pa->year > Pb->year) {
      return 1;
    } else if (Pa->year < Pb->year) {
      return -1;
    } else {
      if (Pa->id < Pb->id) {
        return 1;
      } else {
        return -1;
      }
    }
  }
}

static int d_cmp_papers_for_field(void *a, void *b) {
  return -cmp_papers_for_field(a, b);
}

char **get_most_cited_papers_by_field(PublData *data, const char *field,
                                      int *num_papers) {
  ArrayList *list;
  Paper *info;
  char *s, **names;
  int i;

  if (!data || !field || !num_papers) {
    return NULL;
  }
  s = my_strdup(field);
  list = get_ht_entry(data->field_papers, s);
  free(s);
  if (!list) {
    return NULL;
  }
  if (list->size < *num_papers) {
    *num_papers = list->size;
  }

  /* Indexing is done at 0 */
  quicksort_kth_nodes(list, 0, list->size - 1, *num_papers - 1,
                      d_cmp_papers_for_field);
  names = malloc(*num_papers * sizeof(char *));
  for (i = 0; i < *num_papers; i++) {
    info = list->data[i];
    names[i] = my_strdup(info->title);
  }
  return names;
}

int get_number_of_papers_between_dates(PublData *data, const int early_date,
                                       const int late_date) {
  int i;
  int num = 0;
  if (!data || !data->papers_in_year) {
    return -1;
  }
  for (i = early_date; i <= late_date; i++) {
    num += data->papers_in_year[i - MIN_YEAR];
  }
  return num;
}

int get_number_of_authors_with_field(PublData *data, const char *institution,
                                     const char *field) {
  char *key;
  LinkedList *list;
  if (!data || !data->fldandinstit || !institution || !field) {
    return 0;
  }
  key = add_two_strings(institution, field);
  list = get_ht_entry(data->fldandinstit, key);
  free(key);
  if (!list) {
    return 0;
  }
  return list->size;
}

int *get_histogram_of_citations(PublData *data, const int64_t id_author,
                                int *num_years) {
  Author *human;
  Paper *info;
  Node *curr;
  int *citations_in_year, oldest, year;
  int64_t key = id_author;
  if (!data || !data->all_authors || !num_years) {
    *num_years = -1;
    return NULL;
  }
  human = get_ht_entry(data->all_authors, &key);
  if (!human) {
    *num_years = 0;
    return NULL;
  }

  citations_in_year = calloc(CURRENT_YEAR - MIN_YEAR + 1, sizeof(int));
  oldest = CURRENT_YEAR + 1;
  for (curr = human->papers->head; curr; curr = curr->next) {
    info = curr->data;
    year = abs(info->year - CURRENT_YEAR);
    citations_in_year[year] += info->cited_by->size;
    if (info->year < oldest) {
      oldest = info->year;
    }
  }

  *num_years = CURRENT_YEAR - oldest + 1;
  return citations_in_year;
}

static int cmp_reading_order(void *a, void *b) {
  Paper *Pa = a, *Pb = b;
  if (!Pa || !Pb) {
    return 0;
  }
  if (Pa->year != Pb->year) {
    return Pa->year - Pb->year;
  } else {
    if (Pa->id > Pb->id) {
      return 1;
    } else {
      return -1;
    }
  }
}

char **get_reading_order(PublData *data, const int64_t id_paper,
                         const int distance, int *num_papers) {
  Paper *info, *source, *other_s;
  Node *curr;
  Queue *q;
  LinkedList *dist;
  Hashtable *outdeg;
  Heap *my_heap;
  int64_t key = id_paper;
  int *d, *ext_deg;
  char **names;
  if (!data || !data->all_papers) {
    *num_papers = 0;
    return NULL;
  }

  source = get_ht_entry(data->all_papers, &key);
  if (!source) {
    *num_papers = 0;
    return NULL;
  }

  q = init_queue();
  dist = init_list();
  outdeg = init_ht(10, hash_function_int64, compare_function_int64s);
  my_heap = init_heap(cmp_reading_order);
  names = malloc(100 * sizeof(char *));

  enqueue(q, source);
  add_nth_node(dist, 0, number(0));
  put_ht_pair(outdeg, &source->id, sizeof(int64_t), number(0));

  while (!is_empty_queue(q)) {
    other_s = front(q);
    dequeue(q);
    d = dist->head->data;
    purge_nth_node(dist, 0);
    ext_deg = get_ht_entry(outdeg, &other_s->id);
    // fprintf(stderr, "%d: %lld\n", *d, other_s->id);

    for (curr = other_s->sources->head; curr; curr = curr->next) {
      info = curr->data;
      if (*d + 1 <= distance) {
        (*ext_deg)++;
        if (!has_ht_key(outdeg, &info->id)) {
          enqueue(q, info);
          add_nth_node(dist, dist->size, number(*d + 1));
          put_ht_pair(outdeg, &info->id, sizeof(int64_t), number(0));
        }
      } else {
        if (has_ht_key(outdeg, &info->id)) {
          (*ext_deg)++;
        }
      }
    }
    if (*ext_deg == 0) {
      heap_insert(my_heap, other_s);
      *ext_deg = MAX_PAPERS;
    }

    free(d);
  }

  *num_papers = 0;
  while (!heap_empty(my_heap)) {
    other_s = heap_top(my_heap);
    heap_pop(my_heap);
    names[*num_papers] = my_strdup(other_s->title);
    (*num_papers)++;
    // fprintf(stderr, "%lld %d %s\n", other_s->id, other_s->year,
    // other_s->title);

    for (curr = other_s->cited_by->head; curr; curr = curr->next) {
      info = curr->data;
      ext_deg = get_ht_entry(outdeg, &info->id);
      if (ext_deg) {
        (*ext_deg)--;
        // fprintf(stderr, "  %lld %d\n", info->id, *ext_deg);
        if (*ext_deg <= 0) {
          *ext_deg = MAX_PAPERS;
          heap_insert(my_heap, info);
        }
      }
    }
  }
  // fprintf(stderr, "\n");
  free_ht_and_values(outdeg, free);
  purge_list(dist, free);
  free_queue(q);
  heap_free(my_heap);
  (*num_papers)--;
  free(names[*num_papers]);
  return names;
}

static float score_by_erdos(PublData *data, Author *human, int erdos) {
  float exponent, sum = 0, impact;
  Paper *info;
  Node *curr;
  if (!data || !human || erdos <= 0) {
    return (float)-1;
  }
  exponent = exp(-erdos);
  for (curr = human->papers->head; curr; curr = curr->next) {
    info = curr->data;
    impact = get_venue_impact_factor(data, info->venue);
    sum += info->cited_by->size * impact;
  }
  return exponent * sum;
}

char *find_best_coordinator(PublData *data, const int64_t id_author) {
  Author *first, *human, *coll, *best;
  Node *curr;
  Queue *q;
  LinkedList *dist;
  Hashtable *seen_author;
  int *d, ok, min_erdos;
  char *ret;
  int64_t key = id_author;
  float score, max_score;
  if (!data || !data->all_authors) {
    return NULL;
  }

  first = get_ht_entry(data->all_authors, &key);
  if (first) {
    q = init_queue();
    seen_author = init_ht(MAX_AUTHORS / 200, hash_function_int64,
                          compare_function_int64s);
    dist = init_list();

    enqueue(q, first);
    add_nth_node(dist, 0, number(0));
    put_ht_pair(seen_author, &key, sizeof(int64_t), number(1));

    ok = 1;
    max_score = 0;
    min_erdos = 10;
    best = NULL;
    while (!is_empty_queue(q) && ok) {
      human = front(q);
      d = dist->head->data;
      dequeue(q);
      purge_nth_node(dist, 0);
      if (*d > 5) {
        free(d);
        break;
      }

      score = score_by_erdos(data, human, *d);
      if (score > max_score) {
        best = human;
        min_erdos = *d;
        max_score = score;
      } else if (score == max_score) {
        if (*d < min_erdos) {
          best = human;
          max_score = score;
          min_erdos = *d;
        } else if (*d == min_erdos) {
          if (human->id < best->id) {
            best = human;
            max_score = score;
            min_erdos = *d;
          }
        }
      }

      for (curr = human->colleagues->head; curr != NULL; curr = curr->next) {
        coll = curr->data;
        if (!has_ht_key(seen_author, &coll->id)) {
          enqueue(q, coll);
          add_nth_node(dist, dist->size, number(*d + 1));
          put_ht_pair(seen_author, &coll->id, sizeof(int64_t), number(1));
        }
      }

      free(d);
    }
    if (!best) {
      ret = "None";
    } else {
      ret = best->name;
    }
    purge_list(dist, free);
    free_ht_and_values(seen_author, free);
    free_queue(q);
  } else {
    ret = "None";
  }
  return ret;
}
