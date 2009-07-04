CC?=gcc
CFLAGS+=-Wall
PREFIX?=/usr

all: clean red.o
	${CC} red.o -o red

loc:
	wc -l *.c *.h

clean:
	rm -f red red.o

install:
	cp red ${PREFIX}/bin
