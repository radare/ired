/* Copyleft 2009 -- pancake /at/ nopcode /dot/ org */

#define _FILE_OFFSET_BITS 64
#define _GNU_SOURCE

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

static int fd = -1;

#define io_read(x,y) read(fd, x, y)
#define io_write(x,y) write(fd, x, y)
#define io_open(x,y) \
	fd = open(file, O_RDWR|O_CREAT, 0644); \
	if (fd == -1) fd = open(file, O_RDONLY); \
	return fd
#define io_seek(x,y) return lseek(fd,x,y)
#define io_close() return close(fd)
#define io_system(x) return system(x)
