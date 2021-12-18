#ifndef ENTITIES_H_
#define ENTITIES_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "Hashtable.h"
#include "LinkedList.h"

#define CURRENT_YEAR 2020
#define MAX_YEAR 2020
#define MIN_YEAR 1950
#define MAX_PAPERS 10000
#define MAX_AUTHORS 20000
#define MAX_COLLEAGUES 20
#define MAX_VENUES 100
#define MAX_PAPERS_MISSING MAX_PAPERS

struct author_info {
  char *name;
  int64_t id;
  char *institution;
  LinkedList *papers;     /* List with pointers to the papers */
  LinkedList *colleagues; /* List with pointers other authors */
  Hashtable *knows_who;   /* List with pointers other authors */
};
typedef struct author_info Author;

struct paper_info {
  char *title;
  char *venue;
  int year;
  int num_authors;
  int64_t id;
  int num_refs;
  int64_t *references;
  Author **authors; /* array of pointers to Author entities */
  LinkedList *sources;
  LinkedList *cited_by;
};
typedef struct paper_info Paper;

/**
 * Initialises all the fields contained in the Paper structure, but does
 * no attributtion. Also allocates memory for the structure itself.
 * It is important so when using a Paper * structure, the user doesn't have
 * to check memeory allocation and other pointers.
 *
 * @return a pointer to the newly created Paper structure
 */
Paper *init_paper(void);

/**
 * Deallocates the memory that was previously allocated for the structure, then
 * deallocates the structure itself.
 *
 * @param data  Paper structure whose data is to be deallocated
 */
void destroy_paper(void *p);

/**
 * Attributes values to a new paper. It cannot attribute values to old papers.
 *
 * @param info          the paper structure implemented by you
 * @param title         the title of the paper
 * @param venue         the venue of the paper
 * @param year          the year the paper was published
 * @param id            a unique integer id of the paper
 * @param references    an array of integers representing the ids of the
 *                      papers the current one references
 * @param num_refs      the length of the references array
 */
void attribute_to_paper(Paper *info, const char *title, const char *venue,
                        const int year, const int64_t id, const int num_authors,
                        const int64_t *references, const int num_refs);

/**
 * Initialises all the fields contained in the Author structure, but does
 * no attributtion. Also allocates memory for the structure itself.
 * It is important so when using a Paper * structure, the user doesn't have
 * to check memeory allocation and other pointers.
 *
 * @return a pointer to the newly created Author structure
 */
Author *init_author(void);

/**
 * Deallocates the memory that was previously allocated for the structure, then
 * deallocates the structure itself.
 *
 * @param data Author structure whose data is to be deallocated
 */
void destroy_author(void *p);

/**
 * Attributes values to a new author. It cannot attribute values to old authors.
 *
 * @param human         the Author structure implemented by you
 * @param name          the name of the author
 * @param id            the id of the author
 * @param institution   the name of the author's insititution
 */
void attribute_to_author(Author *human, const char *name, const int64_t id,
                         const char *institution);

#endif /* ENTITIES_H_ */
