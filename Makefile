CC = gcc
CFLAGS = -g -O0 -D_REENTRANT -Wall
LIBS = -lpthread
RANLIB = ranlib

PROGS = flock flock_ex flock_nonblock

all: $(PROGS)

flock: flock.c
	$(CC) $(CFLAGS) $@.c -o $@ $(LIBS)

flock_ex: flock_ex.c
	$(CC) $(CFLAGS) $@.c -o $@ $(LIBS)

flock_nonblock: flock_nonblock.c
	$(CC) $(CFLAGS) $@.c -o $@ $(LIBS)

clean:
	rm -f $(PROGS) $(CLEANFILES)

check-syntax:
	gcc -o null -Wall -Wextra -pedantic -fsyntax-only $(CHK_SOURCES)
