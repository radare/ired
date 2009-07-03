CC?=gcc

all: clean red.o
	${CC} red.o -o red

clean:
	rm -f red red.o
