CC?=gcc
CFLAGS+=-Wall -g
PREFIX?=/usr

all: clean red.o
	${CC} red.o -o red

loc:
	@wc -l *.c *.h | grep total

clean:
	rm -f red red.o

install:
	cp red ${PREFIX}/bin
	cp vired ${PREFIX}/bin
