#include <stdio.h>
#include <stdlib.h>

#include "Deque.h"


int is_string_palindrome(char *s) {
    int i, ok;
    struct Deque *d;

    d = malloc(sizeof(struct Deque));
    if (d == NULL) {
        perror("Not enough memory to create the stack!");
        return -1;
    }

    init_deque(d);
    i = 0;
    while (s[i] != '\0') {
        push_back_deque(d, &s[i]);
        i++;
    }
    while (!is_empty_deque(d) && *(char*)front_deque(d) == *(char*)back_deque(d)) {
        pop_front_deque(d);
        if (!is_empty_deque(d)) {
            pop_back_deque(d);
        }
    }
    ok = 0;
    if (is_empty_deque(d)) {
        ok = 1;
    }
    purge_deque(d);
    free(d);
    return ok;
}

void mesaj_palindrom(int ok) {
    if (ok == 1) {
        printf("Inputul este palindrom.\n");
    }
    else if (ok == 0) {
        printf("Inputul nu este palindrom.\n");
    }
}

int main() {
    char pal[11];
    int ok;

    printf("-------------------------------- Test pr. 4 --------------------------------\n");

    sprintf(pal, "%d", 123454321);
    ok = is_string_palindrome(pal);
    printf("#1:\nOutput: ");
    mesaj_palindrom(ok);
    printf("Output asteptat: ");
    mesaj_palindrom(1);

    sprintf(pal, "%d", 123455321);
    ok = is_string_palindrome(pal);
    printf("#2:\nOutput: ");
    mesaj_palindrom(ok);
    printf("Output asteptat: ");
    mesaj_palindrom(0);

    return 0;
}
