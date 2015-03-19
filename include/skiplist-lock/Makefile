.PHONY:	all clean map

CC = g++
BINS=main
map: main-map

LDFLAGS=-lpthread
all:	main

skiplist-lock-map.o:
	$(CC) $(CFLAGS) -c -o skiplist-lock-map.o skiplist-lock-map.c

optimistic-map.o: skiplist-lock-map.h 
	$(CC) $(CFLAGS) -c -o optimistic-map.o optimistic-map.c 

sl-map.o: skiplist-lock-map.h optimistic-map.h
	$(CC) $(CFLAGS) -c -o sl-map.o sl-map.c

test-map.o: skiplist-lock-map.h optimistic-map.h sl-map.h
	$(CC) $(CFLAGS) -c -o test-map.o test-map.c

main-map: skiplist-lock-map.o optimistic-map.o sl-map.o test-map.o 
	$(CC) $(CFLAGS) skiplist-lock-map.o optimistic-map.o sl-map.o test-map.o -o $(BINS)-map $(LDFLAGS)

skiplist-lock.o:
	$(CC) $(CFLAGS) -c -o skiplist-lock.o skiplist-lock.c

optimistic.o: skiplist-lock.h 
	$(CC) $(CFLAGS) -c -o optimistic.o optimistic.c 

intset.o: skiplist-lock.h optimistic.h
	$(CC) $(CFLAGS) -c -o intset.o intset.c

test.o: skiplist-lock.h optimistic.h intset.h
	$(CC) $(CFLAGS) -c -o test.o test.c

main: skiplist-lock.o optimistic.o intset.o test.o 
	$(CC) $(CFLAGS) skiplist-lock.o optimistic.o intset.o test.o -o $(BINS) $(LDFLAGS)

clean:
	-rm -f $(BINS) *.o
