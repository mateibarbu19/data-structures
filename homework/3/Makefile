CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Wpedantic -m32
DEBUG=-ggdb
PUBL=publications

.PHONY: build clean

build: $(PUBL).o

$(PUBL).o: tools.o LinkedList_unlinked.o ArrayList_unlinked.o Hashtable_unlinked.o \
           Queue_unlinked.o entities.o Heap_unlinked.o $(PUBL)_unlinked.o
	ld -m elf_i386 -r $^ -o $@

$(PUBL)_unlinked.o: $(PUBL).c
	$(CC) $(CFLAGS) $(DEBUG) -lm $^ -c -o $@

entities.o: src/entities.c
	$(CC) $(CFLAGS) $(DEBUG) $^ -c -o $@


Heap_unlinked.o: src/Heap.c
	$(CC) $(CFLAGS) $(DEBUG) $^ -c -o $@

Queue_unlinked.o: src/Queue.c
	$(CC) $(CFLAGS) $(DEBUG) $^ -c -o $@

Hashtable_unlinked.o: src/Hashtable.c
	$(CC) $(CFLAGS) $(DEBUG) $^ -c -o $@

LinkedList_unlinked.o: src/LinkedList.c
	$(CC) $(CFLAGS) $(DEBUG) $^ -c -o $@

ArrayList_unlinked.o: src/ArrayList.c
	$(CC) $(CFLAGS) $(DEBUG) $^ -c -o $@

tools.o: src/tools.c
	$(CC) $(CFLAGS) $(DEBUG) $^ -c -o $@

clean:
	rm -f *.o *.h.gch
	rm -f src/*.h.gch
