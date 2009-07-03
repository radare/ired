CC?=gcc
PREFIX?=/usr

all: clean red.o
	${CC} red.o -o red

clean:
	rm -f red red.o

install:
	cp red ${PREFIX}/bin
