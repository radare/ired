</$objtype/mkfile

CC=pcc
ALL=ired

9ired:
	$CC -Dplan9 -D_POSIX_SOURCE -D_BSD_EXTENSION $CFLAGS -o 9ired ired.c

clean:
	rm -f ired.$O 9ired
