CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99 -I.
DEBUG=-ggdb

build: tema1

tema1: main.o Player_unlinked.o DoublyLinkedList.o
	$(CC) $(CFLAGS) $(DEBUG) main.o Player_unlinked.o DoublyLinkedList.o -o tema1

Player_unlinked.o: Player.c Player.h
	$(CC) $(CFLAGS) $(DEBUG) Player.c -c -o Player_unlinked.o

DoublyLinkedList.o: DoublyLinkedList.h DoublyLinkedList.c
	$(CC) $(CFLAGS) $(DEBUG) DoublyLinkedList.c -c -o DoublyLinkedList.o

main.o: main.c
	$(CC) $(CFLAGS) $(DEBUG) main.c -c -o main.o

run:
	./tema1

clean:
	rm *.o
	rm tema1

