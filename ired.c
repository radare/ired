/* Copyleft 2009-2012 -- pancake /at/ nopcode /dot/ org */

#define ut64 unsigned long long 
#define ut8 unsigned char

static int verbose = 1;
static char *script = 0;
static ut64 oldseek, curseek = 0LL;
static int obsize, bsize = 256;
static int red_cmd(char *cmd); // XXX : recursive depenency
#define BUFSZ 128*1024

#include "ired.h"
#include "util.c"
#include "cmd.c"

static void red_slurpin() {
	ut8 buf[BUFSZ];
	for(;;) {
		int len = read(0, buf, sizeof(buf));
		if(len<1) break;
		hexdump(buf, len, 16);
		curseek += len;
	}
}

static void red_interpret(char *file) {
	char buf[BUFSZ];
	FILE *fd = fopen(file, "r");
	if(fd != NULL) {
		for(;;) {
			if(fgets(buf, sizeof(buf), fd) == NULL)
				break;
			red_cmd(buf);
		}
		fclose(fd);
	} else fprintf(stderr, "Cannot open script file '%s'\n", file? file: "");
}

static int red_cmd(char *cmd) {
	char *arg = cmd+1;
	SKIPSPACES(arg);
	switch(*cmd) {
	case 'q': return 0;
	case ';': case '#': break; // comment
	case '>': cmd_dump(arg); break;
	case '<': cmd_load(arg); break;
	case '.': red_interpret(arg); break;
	case 's': cmd_seek(arg); break;
	case 'b': cmd_bsize(arg); break;
	case '/': cmd_search(arg); break;
	case 'p': cmd_print(arg); break;
	case 'r': cmd_resize(arg); break;
	case 'x': cmd_hexdump(arg); break;
	case 'X': cmd_bytedump(arg); break;
	case 'w': cmd_write(arg); break;
	case '!': cmd_system(arg); break;
	case '?': cmd_help(arg); break;
	default: fprintf(stderr, "? %s\n", cmd);
	}
	return 1;
}

static int red_prompt() {
	char *at, *at2, line[BUFSZ];
	if(verbose) {
		printf("[0x%08"LLF"x]> ", curseek);
		fflush(stdout);
	}
	if(fgets(line, sizeof(line), stdin) == NULL)
		return 0;
	line[strlen(line)-1] = '\0';
	if(*line != '!') {
		at = strchr(line, '@');
		oldseek = curseek;
		obsize = bsize;
		if(at) {
			*at = 0;
			at2 = strchr(++at, ':');
			if(at2) {
				*at2 = 0; at2++;
				if(*at2) bsize = (int)str2ut64(at2);
			}
			if(*at) curseek = str2ut64(at);
		}
	}
	at = line;
	SKIPSPACES(at);
	return red_cmd(at);
}

static int red_open(char *file) {
	int ret = io_open(file);
	if(ret != -1) {
		oldseek = 0;
		setenv("FILE", file, 1);
		if(script)
			red_interpret(script);
		while(red_prompt()) {
			curseek = oldseek;
			bsize = obsize;
		}
		io_close();
	} else fprintf(stderr, "Cannot open '%s'\n", file? file: "");
	return ret==-1 ?1:0;;
}

static int red_help() {
	puts("ired [-hnv] [-i script] [file] [..]");
	return 0;
}

int main(int argc, char **argv) {
	int i, ret = 1;
	argc++;
	if(argc>1 && argv[1])
	for(i=1; i<argc; i++) {
		if(argv[i] && argv[i][0]=='-')
			switch(argv[i][1]) {
			case 'i': script = argv[++i]; break;
			case 'n': verbose = 0; break;
			case 'v': puts("ired "VERSION); ret = 0; break;
			case 'h': ret = red_help(); break;
			case 0x0: red_slurpin(); ret = 0; break;
		} else ret = red_open(argv[i]);
	} else ret = red_help();
        return ret;
}
