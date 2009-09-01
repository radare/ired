CC?=gcc
CCw32?=i486-mingw32-gcc
CFLAGS+=-O2
PREFIX?=/usr

all:
	${CC} ${CFLAGS} ired.c -o ired

w32:
	${CCw32} ${CFLAGS} ired.c -o ired.exe

loc:
	@wc -l *.c *.h | grep total

clean:
	rm -f ired ired.o

install:
	cp ired ${PREFIX}/bin
	cp vired ${PREFIX}/bin
