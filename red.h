#define VERSION "0.1"

#include "io.c"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#if __WIN32__
#include <windows.h>
#define setenv(x,y,z) SetEnvironmentVariable(x, y)
#endif

#include "util.c"
#include "cmd.c"
