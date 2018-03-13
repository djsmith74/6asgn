CC = gcc

CFLAGS = -Wall -pedantic -ansi -g -std=c99

LD = gcc

LDFLAGS =

all: parseline

parseline: io.o pline.o print.o
	$(LD) $(CFLAGS) -o parseline io.o pline.o print.o

io.o: io.c io.h
	$(CC) $(CFLAGS) -c io.c 

pline.o: pline.c pline.h
	$(CC) $(CLFAGS) -c pline.c

print.o: print.c print.h
	$(CC) $(CFLAGS) -c print.c 

clean: 
	rm -f *.o parseline
