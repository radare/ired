/* Copyleft 2009 -- pancake /at/ nopcode /dot/ org */

#define _FILE_OFFSET_BITS 64
#define _GNU_SOURCE

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

static int fd = -1;

static inline
int io_read(void *buf, int len) {
	return read(fd, buf, len);
}

static inline
int io_write(void *buf, int len) {
	return write(fd, buf, len);
}

static inline
int io_open(char *file) {
	fd = open(file, O_RDWR|O_CREAT, 0644);
	if (fd == -1)
		fd = open(file, O_RDONLY);
	return fd;
}

static inline
int io_seek(ull addr, int type) {
	return lseek(fd, addr, type);
}

static inline
int io_close() {
	return close(fd);
}

static inline
int io_system(char *cmd) {
	return system(cmd);
}
