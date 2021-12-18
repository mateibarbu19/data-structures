CC := gcc
CFLAGS := -ansi -ggdb -Wall -Wextra

TASKS := treap
SRCDIR := src

ALL_SRCS := $(wildcard $(SRCDIR)/*.c)
SRCFILES := $(filter-out $(SRCDIR)/treap%.c, $(ALL_SRCS))
OBJFILES := $(patsubst $(SRCDIR)/%.c,%.o,$(SRCFILES))

.PHONY: build clean

build: treap

run: treap
	./treap

treap: $(OBJFILES) $(SRCDIR)/treap.c
	$(CC) $(CFLAGS) -o $@ $^

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJFILES) $(TASKS)
