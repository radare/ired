/* Copyleft 2009 -- pancake /at/ nopcode /dot/ org */

#define _FILE_OFFSET_BITS 64
#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#if __WIN32__
#include <windows.h>
static HANDLE _fd = NULL;
#define setenv(x,y,z) SetEnvironmentVariable(x, y)
static inline int io_open(char *file) {
	_fd = CreateFile(file, GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, 0, NULL);
	if (_fd == INVALID_HANDLE_VALUE)
	_fd = CreateFile(file, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_ALWAYS, 0, NULL);
	return (_fd==INVALID_HANDLE_VALUE)?-1:0;
}
static int io_read(void *x, int y) {
	DWORD ret;
	return ReadFile(_fd,x,y,&ret,NULL)?ret:-1;
}
#define io_write(x,y) WriteFile(_fd, x, y, NULL, NULL)
#define io_seek(x,y) SetFilePointer(_fd,x,0,!y?FILE_BEGIN:y==1?FILE_CURRENT:FILE_END)
#define io_close() CloseHandle(_fd)
#define io_system(x) system(x)
#define io_truncate(x) { io_seek(x,SEEK_SET); SetEndOfFile(_fd); }

#else
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
#endif
