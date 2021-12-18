#ifndef TOOLS_H_
#define TOOLS_H_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * If the assertion is true, the program is closed and the file and line number
 * at which the program closes are printed to `stderr`, together with a given
 * message.
 */
#define DIE(assertion, call_description)                                       \
  do {                                                                         \
    if (assertion) {                                                           \
      fprintf(stderr, "(%s:%u): ", __FILE__, __LINE__);                        \
      perror(call_description);                                                \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

/** For more details consult the Linux manual, with the command man 3 strdup **/
char *my_strdup(const char *s);

uint64_t hash_function_int64(void *a);

int compare_function_ints(void *a, void *b);

int compare_function_int64s(void *a, void *b);

uint64_t hash_function_string(void *a);

int compare_function_strings(void *a, void *b);

int *number(int x);

char *add_two_strings(const char *a, const char *b);

#endif /* TOOLS_H_ */
