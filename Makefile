CC?=gcc
VERSION=0.6git
CCw32?=i486-mingw32-gcc
CFLAGS+=-O2 -Wall
#CFLAGS+=-DVERSION=\"${VERSION}\"
PREFIX?=/usr/local
DESTDIR?=

all: ired bdiff

bdiff: bdiff.o
	${CC} ${CFLAGS} bdiff.o -o bdiff

ired.o: calc.c cmd.c io.c util.c hexparse.c

ired.js:
	emcc -Os -o ired.js ired.c

ired.wasm:
	emcc -Os -o ired.html ired.c

ired.bc:
	clang -emit-llvm -o ired.bc -c ired.c

ired: ired.o
	${CC} ${CFLAGS} ired.o -o ired

dist:
	@if [ -z "${VERSION}" ]; then echo "Try: make dist VERSION=0.5" ; exit 1 ; fi
	git clone . ired-${VERSION}
	rm -rf ired-${VERSION}/.git
	tar czvf ired-${VERSION}.tar.gz ired-${VERSION}
	rm -rf ired-${VERSION}

ios:
	$(MAKE) CC="xcrun --sdk iphoneos gcc -arch arm64 -DHAVE_SYSTEM=0"

ios16:
	$(MAKE) CC="xcrun --sdk iphoneos gcc -arch armv7 -mno-thumb -DHAVE_SYSTEM=0"

ios32:
	$(MAKE) CC="xcrun --sdk iphoneos gcc -arch armv7 -DHAVE_SYSTEM=0"

w32:
	${CCw32} ${CFLAGS} ired.c -o ired.exe

v850:
	./v850.sh

loc:
	@wc -l *.c *.h | grep total

clean:
	rm -f ired ired.o bdiff bdiff.o

install:
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f ired ${DESTDIR}${PREFIX}/bin
	cp -f bdiff ${DESTDIR}${PREFIX}/bin
	cp -f vired ${DESTDIR}${PREFIX}/bin
	mkdir -p ${DESTDIR}${PREFIX}/share/man/man1
	cp -f ired.1 ${DESTDIR}${PREFIX}/share/man/man1
	cp -f vired.1 ${DESTDIR}${PREFIX}/share/man/man1

deinstall uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/ired
	rm -f ${DESTDIR}${PREFIX}/bin/vired
	rm -f ${DESTDIR}${PREFIX}/share/man/man1/ired.1
	rm -f ${DESTDIR}${PREFIX}/share/man/man1/vired.1
