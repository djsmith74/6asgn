CC = gcc

CFLAGS = -Wall -pedantic -ansi -D_POSIX_C_SOURCE -g -std=c99

LD = gcc

LDFLAGS =

all: mush

mush: io.o pline.o print.o mush.o main.o
	$(LD) $(CFLAGS) -o mush io.o pline.o print.o mush.o main.o

io.o: io.c io.h
	$(CC) $(CFLAGS) -c io.c 

mush.o: mush.c mush.h
	$(CC) $(CFLAGS) -c mush.c

main.o: main.c 
	$(LD) $(CFLAGS) -c main.c

pline.o: pline.c pline.h
	$(CC) $(CLFAGS) -c pline.c

print.o: print.c print.h
	$(CC) $(CFLAGS) -c print.c 

clean: 
	rm -f *.o mush
