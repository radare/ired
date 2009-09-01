CC?=gcc
CCw32?=i486-mingw32-gcc
CFLAGS+=-O2
PREFIX?=/usr
DESTDIR?=

all:
	${CC} ${CFLAGS} ired.c -o ired

dist:
	if [ -z "${VERSION}" ]; then echo "make dist VERSION=0.2" ; exit 1 ; fi
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
	cp ired ${DESTDIR}${PREFIX}/bin
	cp vired ${DESTDIR}${PREFIX}/bin
	cp ired.1 ${DESTDIR}${PREFIX}/share/man/man1
	cp vired.1 ${DESTDIR}${PREFIX}/share/man/man1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/ired
	rm -f ${DESTDIR}${PREFIX}/bin/vired
	rm -f ${DESTDIR}${PREFIX}/share/man/man1/ired.1
	rm -f ${DESTDIR}${PREFIX}/share/man/man1/vired.1
