.PHONY: build clean

CC=gcc
CFLAGS=-Wall -Wextra -ansi -pedantic -I.
DEBUG=-ggdb

build: hll freq hash

freq: freq.c
	$(CC) $(CFLAGS) $(DEBUG) -o $@ $<

hll: hll.c
	$(CC) $(CFLAGS) $(DEBUG) -o $@ $<

hash: hash.c
	$(CC) $(CFLAGS) $(DEBUG) -o $@ $<

clean:
	rm freq
	rm hash
	rm hll
