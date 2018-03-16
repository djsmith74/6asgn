CC = gcc

CFLAGS = -Wall -pedantic -ansi -D_POSIX_C_SOURCE -g -std=c99

LD = gcc

LDFLAGS =

all: mush

mush: main.o io.o pline.o print.o exec.o mush.o
	$(LD) $(CFLAGS) -o mush main.o io.o pline.o print.o exec.o mush.o

io.o: io.c
	$(CC) $(CFLAGS) -c io.c 

mush.o: mush.c
	$(CC) $(CFLAGS) -c mush.c

main.o: main.c 
	$(LD) $(CFLAGS) -c main.c

pline.o: pline.c
	$(CC) $(CLFAGS) -c pline.c

print.o: print.c
	$(CC) $(CFLAGS) -c print.c 

exec.o: exec.c
	$(CC) $(CFLAGS) -c exec.c

clean: 
	rm -f *.o mush
