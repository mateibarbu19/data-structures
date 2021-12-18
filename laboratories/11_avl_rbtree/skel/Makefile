CC := gcc
CFLAGS := -ansi -ggdb -Wall -Wextra

TASKS := task1 task2
SRCDIR := src

ALL_SRCS := $(wildcard $(SRCDIR)/*.c)
OBJFILES := $(patsubst $(SRCDIR)/%.c,%.o,$(ALL_SRCS))

.PHONY: build clean

build: $(TASKS)

run: task1
	./task1

run_2: task2
	./task2

task1: task1.o avl.o
	$(CC) $(CFLAGS) -o $@ $^

task2: task2.o rb_tree.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJFILES) $(TASKS)
