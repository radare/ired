/* Copyleft 2009 -- pancake /at/ nopcode /dot/ org */

#define _FILE_OFFSET_BITS 64
#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

static int _fd = -1;

static inline int io_open(char *file) {
	_fd = open(file, O_RDWR|O_CREAT, 0644);
	if (_fd == -1) _fd = open(file, O_RDONLY);
	return _fd;
}
#define io_read(x,y) read(_fd, x, y)
#define io_write(x,y) write(_fd, x, y)
#define io_seek(x,y) lseek(_fd, x, y)
#define io_close() close(_fd)
#define io_system(x) system(x)
#define io_truncate(x) ftruncate(_fd,(unsigned long)x)
