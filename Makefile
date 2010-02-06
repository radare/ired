CC?=gcc
CCw32?=i486-mingw32-gcc
CFLAGS+=-O2 -Wall
PREFIX?=/usr
DESTDIR?=

all: ired bdiff

bdiff: bdiff.o
	${CC} ${CFLAGS} bdiff.o -o bdiff

ired: ired.o
	${CC} ${CFLAGS} ired.o -o ired

dist:
	if [ -z "${VERSION}" ]; then echo "make dist VERSION=0.3" ; exit 1 ; fi
	mkdir -p ired-${VERSION}
	cd ired-${VERSION} ; hg clone .. . ; rm -rf .hg
	tar czvf ired-${VERSION}.tar.gz ired-${VERSION}
	rm -rf ired-${VERSION}

w32:
	${CCw32} ${CFLAGS} ired.c -o ired.exe

loc:
	@wc -l *.c *.h | grep total

clean:
	rm -f ired ired.o

install:
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp ired ${DESTDIR}${PREFIX}/bin
	cp bdiff ${DESTDIR}${PREFIX}/bin
	cp vired ${DESTDIR}${PREFIX}/bin
	mkdir -p ${DESTDIR}${PREFIX}/share/man/man1
	cp ired.1 ${DESTDIR}${PREFIX}/share/man/man1
	cp vired.1 ${DESTDIR}${PREFIX}/share/man/man1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/ired
	rm -f ${DESTDIR}${PREFIX}/bin/vired
	rm -f ${DESTDIR}${PREFIX}/share/man/man1/ired.1
	rm -f ${DESTDIR}${PREFIX}/share/man/man1/vired.1
