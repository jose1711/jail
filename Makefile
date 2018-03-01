CC=gcc
CFLAGS=-g -Wall
LDFLAGS=-L /usr/X11R6/lib -lX11 -lXtst -lXext

Jail: Jail.o

clean:
	rm -f Jail.o Jail
