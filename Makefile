CC?=gcc
CCw32?=i486-mingw32-gcc
CFLAGS+=-Wall -O2
PREFIX?=/usr

all:
	${CC} ${CFLAGS} red.c -o red

w32:
	${CCw32} ${CFLAGS} red.c -o red.exe

loc:
	@wc -l *.c *.h | grep total

clean:
	rm -f red red.o

install:
	cp red ${PREFIX}/bin
	cp vired ${PREFIX}/bin
