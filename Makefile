CC?=gcc
CCw32?=i486-mingw32-gcc
CFLAGS+=-O2
PREFIX?=/usr

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
	cp ired ${PREFIX}/bin
	cp vired ${PREFIX}/bin
	cp ired.1 ${PREFIX}/share/man/man1
	cp vired.1 ${PREFIX}/share/man/man1
