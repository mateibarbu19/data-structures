#include <stdio.h>
#include <stdlib.h>

#include "Stack.h"
#include "Queue.h"


int are_paranthesis_balanced(char *s) {
    struct Stack *st;
    int i, ok;

    st = malloc(sizeof(struct Stack));
    if (st == NULL) {
        perror("Not enough memory to create the stack!");
        return -1;
    }

    init_stack(st);
    i = 0;
    ok = 1;
    while (s[i] != '\0' && ok) {
        if (s[i] == ')') {
            if (*(char*)peek_stack(st) == '(') {
                pop_stack(st);
            }
            else {
                ok = 1;
            }
        }
        if (s[i] == ']') {
            if (*(char*)peek_stack(st) == '[') {
                pop_stack(st);
            }
            else {
                ok = 1;
            }
        }
        if (s[i] == '}') {
            if (*(char*)peek_stack(st) == '{') {
                pop_stack(st);
            }
            else {
                ok = 1;
            }
        }
        else {
            push_stack(st, &s[i]);
        }
        i++;
    }
    purge_stack(st);
    free(st);
    return ok;
}

void mesaj_paranteze(int ok) {
    if (ok == 1) {
        printf("Sirul este parantezat corect.\n");
    }
    else if (ok == 0) {
        printf("Sirul nu este parantezat corect.\n");
    }
}

int main() {
    int numbers[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int crt_val;
    struct Queue *q;
    struct Stack *st;
    char *s[] = { "({()[({[()]})]})((()))()", "({()[({[()]}]]})((()))()" };

    q = malloc(sizeof(struct Queue));
    if (q == NULL) {
        perror("Not enough memory to create the queue!");
        return -1;
    }

    st = malloc(sizeof(struct Stack));
    if (st == NULL) {
        perror("Not enough memory to create the stack!");
        return -1;
    }

    printf("-------------------------------- Test stack --------------------------------\n");
    init_stack(st);
    push_stack(st, &numbers[10]);

    /* Test push & size */
    printf("#1:\nOutput: size: %d\nOutput asteptat: size: 1\n", get_size_stack(st));

    /* Test peek */
    crt_val = *(int *)peek_stack(st);
    printf("#2:\nOutput: %d\nOutput asteptat: 10\n", crt_val);

    /* Test pop */
    crt_val = *(int *)peek_stack(st);
    pop_stack(st);
    printf("#3:\nOutput: value: %d size: %d\nOutput asteptat: value: 10 size: 0\n", crt_val, get_size_stack(st));

    push_stack(st, &numbers[9]);
    push_stack(st, &numbers[8]);
    push_stack(st, &numbers[7]);

    /* Test multiple pushes */
    printf("#4:\nOutput: size: %d top elem: %d\nOutput asteptat: size: 3 top elem: 7\n", get_size_stack(st), *(int *)peek_stack(st));

    /* Test multiple pops */
    printf("#5:\nOutput: ");
    while (!is_empty_stack(st)) {
        printf("%d ", *(int *)peek_stack(st));
        pop_stack(st);
    }
    printf("size: %d", get_size_stack(st));
    printf("\nOutput asteptat: 7 8 9 size: 0\n");

    purge_stack(st);
    free(st);

    printf("\n\n-------------------------------- Test queue --------------------------------\n");
    init_q(q);
    enqueue(q, &numbers[5]);

    /* Test enqueue & size */
    printf("#1:\nOutput: size: %d\nOutput asteptat: size: 1\n", get_size_q(q));

    /* Test front */
    crt_val = *(int *)front(q);
    printf("#2:\nOutput: %d\nOutput asteptat: 5\n", crt_val);

    /* Test dequeue */
    crt_val = *(int *)front(q);
    dequeue(q);
    printf("#3:\nOutput: value: %d size: %d\nOutput asteptat: value: 5 size: 0\n", crt_val, get_size_q(q));

    enqueue(q, &numbers[2]);
    enqueue(q, &numbers[3]);
    enqueue(q, &numbers[4]);

    /* Test multiple pushes */
    printf("#4:\nOutput: size: %d top elem: %d\nOutput asteptat: size: 3 front elem: 2\n", get_size_q(q), *(int *)front(q));

    /* Test multiple pops */
    printf("#5:\nOutput: ");
    while (!is_empty_q(q)) {
        printf("%d ", *(int *)front(q));
        dequeue(q);
    }
    printf("size: %d", get_size_q(q));
    printf("\nOutput asteptat: 2 3 4 size: 0\n");

    purge_q(q);
    free(q);

    printf("\n\n-------------------------------- Test pr. 3 --------------------------------\n");

    crt_val = are_paranthesis_balanced(s[0]);
    printf("#1:\nOutput: ");
    mesaj_paranteze(crt_val);
    printf("Output asteptat: ");
    mesaj_paranteze(1);

    crt_val = are_paranthesis_balanced(s[1]);
    printf("#2:\nOutput: ");
    mesaj_paranteze(crt_val);
    printf("Output asteptat: ");
    mesaj_paranteze(0);

    return 0;
}
