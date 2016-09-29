all:	DVmega-version

CC = gcc
CFLAGS = -O2 -Wall

DVmega-version:
		$(CC) $(CFLAGS) -o DVmega-version DVmega-version.c

clean:
		rm -f *.o *.bak *~ DVmega-version
