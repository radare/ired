#define VERSION "0.3"

#include "io.c"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#if(plan9)
static int setenv(char *var, char *val, int force) {
	char str[128];
	snprintf(str, sizeof(str), "echo %s > /env/%s", var, val);
	if (system(str)<0)
		perror("system");
	return force;
}
#define LLF "L"
#else
#define LLF "ll"
#endif

#include "util.c"
#include "cmd.c"
