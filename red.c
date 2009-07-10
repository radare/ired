/* Copyleft 2009 -- pancake /at/ nopcode /dot/ org */

#define ut64 unsigned long long 
#define ut8 unsigned char

static int verbose = 1;
static char *script = 0;
static ut64 oseek, seek = 0LL;
static int obsize, bsize = 256;
static char *red_interpret(char *file); // XXX

#include "red.h"

static int red_cmd(char *cmd) {
	switch(*cmd) {
	case 'q': return 0;
	case '>': cmd_dump(cmd+1); break;
	case '<': cmd_load(cmd+1); break;
	case '.': red_interpret(skipspaces(cmd+1)); break;
	case 's': cmd_seek(cmd+1); break;
	case 'b': cmd_bsize(cmd+1); break;
	case '/': cmd_search(cmd+1); break;
	case 'x': cmd_hexdump(cmd+1); break;
	case 'X': cmd_bytedump(cmd+1); break;
	case 'w': cmd_write(cmd+1); break;
	case '!': cmd_system(cmd+1); break;
	case '?': cmd_help(cmd+1); break;
	default: fprintf(stderr, "? %s\n", cmd);
	}
	return 1;
}

static int red_prompt() {
	char *at, *at2, line[4096];
	if (verbose) {
		printf("[0x%08llx]> ", seek);
		fflush(stdout);
	}
	fgets(line, 4095, stdin);
	if (feof(stdin))
		return 0;
	line[strlen(line)-1]='\0';
	at = strchr(line, '@');
	oseek = seek;
	obsize = bsize;
	if (at) {
		*at = 0; at++;
		at2 = strchr(at, ':');
		if (at2) {
			*at2 = 0; at2++;
			if (*at2) bsize = (int)str2ut64(at2);
		}
		if (*at) seek = str2ut64(at);
	}
	return red_cmd(skipspaces(line));
}

int red_slurpin() {
	int len;
	unsigned char buf[1024];
	for(;;) {
		len = read(0, buf, 1024);
		if (len<1)
			break;
		hexdump(buf, len);
		seek+=len;
	}
	return 0;
}

static char *red_interpret(char *file) {
	char buf[1024];
	FILE *fh = fopen(file, "r");
	if (fh == NULL) {
		fprintf(stderr, "Cannot open script file '%s'\n", file);
		return file;
	}
	for(;;) {
		fgets(buf, 1023, fh);
		if (feof(fh))
			break;
		red_cmd(buf);
	}
	fclose(fh);
	return NULL;
}

static void red_open(char *file) {
	if (io_open(file) != -1) {
		if (script)
			script = red_interpret(script);
		while(red_prompt()) {
			seek = oseek;
			bsize = obsize;
		}
		io_close();
	} else fprintf(stderr, "Cannot open '%s'.\n", file);
}

static int red_help() {
	puts("red [-nhv] [-i script] [file] [..]");
	return 0;
}

int main(int argc, char **argv) {
	int i;
	if (argc==1)
		return red_help();
	for(i=1;i<argc;i++) {
		if (argv[i][0]!='-')
			red_open(argv[i]);
		else
		switch(argv[i][1]) {
		case 'i': script = argv[++i]; break;
		case 'n': verbose = 0; break;
		case 'v': puts("red "VERSION" 2009"); return 0;
		case 'h': return red_help();
		case 0: return red_slurpin();
		}
	}
        return 0;
}
