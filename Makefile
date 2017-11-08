all:	MMDVMVersion

CC = gcc
CFLAGS = -O2 -Wall

MMDVMVersion:
		$(CC) $(CFLAGS) -o MMDVMVersion MMDVMVersion.c

clean:
		rm -f *.o *.bak *~ MMDVMVersion
