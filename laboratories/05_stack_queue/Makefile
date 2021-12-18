CC=gcc
CFLAGS=-Wall -Wextra -pedantic -ansi
DEBUG=-g -ggdb -O0 -march=native

build: Test Test_pr4

Test: src/Test.o src/Stack_unlinked.o src/Queue_unlinked.o src/LinkedList.o
	$(CC) $(CFLAGS) $(DEBUG) src/Test.o src/Queue_unlinked.o src/Stack_unlinked.o src/LinkedList.o -o Test

Test_pr4: src/Test_pr4.o src/Deque_unlinked.o src/DoublyLinkedList.o
	$(CC) $(CFLAGS) $(DEBUG) src/Test_pr4.o src/Deque_unlinked.o src/DoublyLinkedList.o -o Test_pr4

src/Queue_unlinked.o: src/Queue.h src/Queue.c
	$(CC) $(CFLAGS) $(DEBUG) src/Queue.c -c -o src/Queue_unlinked.o

src/Stack_unlinked.o: src/Stack.h src/Stack.c
	$(CC) $(CFLAGS) $(DEBUG) src/Stack.c -c -o src/Stack_unlinked.o

src/Deque_unlinked.o: src/Deque.h src/Deque.c
	$(CC) $(CFLAGS) $(DEBUG) src/Deque.c -c -o src/Deque_unlinked.o

src/LinkedList.o: src/LinkedList.h src/LinkedList.c
	$(CC) $(CFLAGS) $(DEBUG) src/LinkedList.c -c -o src/LinkedList.o

src/DoublyLinkedList.o: src/DoublyLinkedList.h src/DoublyLinkedList.c
	$(CC) $(CFLAGS) $(DEBUG) src/DoublyLinkedList.c -c -o src/DoublyLinkedList.o

src/Test.o: src/Test.c
	$(CC) $(CFLAGS) $(DEBUG) src/Test.c -c -o src/Test.o

src/Test_pr4.o: src/Test_pr4.c
	$(CC) $(CFLAGS) $(DEBUG) src/Test_pr4.c -c -o src/Test_pr4.o

run:
	./Test

clean:
	rm src/*.o
	rm Test
	rm Test_pr4
