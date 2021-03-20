#define VERSION "0.7"

#include <unistd.h>
#include <sys/types.h>
#include "io.c"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define SKIPSPACES(x) for(;*(x)==' '||*(x)=='\t';x++) {};

#if(plan9)
static int setenv(char *var, char *val, int force) {
	char str[256];
	snprintf(str, sizeof(str), "echo %s > /env/%s", var, val);
	return system(str);
}
#define LLF "L"
#else
#define LLF "ll"
#endif
