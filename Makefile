all:	DVmega-version

CC = gcc
CFLAGS = -O2 -Wall

DVmega-version:
		$(CC) $(CFLAGS) -o MMDVM-version MMDVM-version.c

clean:
		rm -f *.o *.bak *~ MMDVM-version
