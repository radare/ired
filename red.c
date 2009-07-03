/* Copyleft 2009 -- pancake /at/ nopcode /dot/ org */

#define ull unsigned long long 

static int fd = 0;
static char *file = 0;
ull oseek, seek = 0LL;
int bsize = 512;

#include "red.h"

int red_cmd(char *cmd) {
	switch(*cmd) {
	case 'q':
		return 0;
	case 's':
		cmd_seek(cmd+1);
		break;
	case 'x':
		cmd_hexdump(cmd+1);
		break;
	case 'w':
		cmd_write(cmd+1);
		break;
	case '?':
		cmd_help(cmd+1);
		break;
	default:
		fprintf(stderr, "? %s", cmd);
	}
	return 1;
}

int red_prompt() {
	char *at, line[4096];

	printf("[0x%08llx]> ", seek);
	fflush(stdout);
	fgets(line, 4095, stdin);
	if (feof(stdin))
		return 0;
	line[strlen(line)-1]='\0';
	at = strchr(line, '@');
	if (at) {
		*at='\0';
		oseek = seek;
		seek = str2ull(at+1);
	} else oseek = seek;
	return red_cmd(skipspaces(line));
}

int red_open(char *file) {
	fd = open(file, O_RDWR);
	if (fd == -1)
		fd = open(file, O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "Cannot open '%s'.\n", file);
		return 1;
	}
	while(red_prompt())
		seek = oseek;
	close(fd);
}

int main(int argc, char **argv) {
        char ch;
        while(argc-->0)
		red_open(argv[argc]); /* open myself :) */
        return 0;
}
