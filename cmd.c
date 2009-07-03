/* Copyleft 2009 -- pancake /at/ nopcode /dot/ org */

void cmd_hexdump(char *arg)
{
	int i, j;
	int inc = 16;
	int len = bsize;
	unsigned char *buf = malloc(bsize);

	if (*arg) {
		len = (int)str2ull(arg);
		if (len <1)
			len = bsize;
	} 
	lseek(fd, seek, SEEK_SET);
	read(fd, buf, bsize);

	for(i=0;i<len;i+=inc) {
		printf("0x%08llx  ", seek+i);
		for(j=i;j<i+inc;j++) {
			if (j>=len) {
				if (j%2) printf("   ");
				else printf("  ");
				continue;
			}
			printf("%02x", buf[j]);
			if (j%2) printf(" ");
		}

		for(j=i;j<i+inc;j++) {
			if (j >= len) printf(" ");
			else printf("%c", isprint(buf[j])?buf[j]:'.');
		}
		printf("\n");
	}
}

void cmd_bsize(char *arg)
{
	bsize = (int)str2ull(arg);
	if (bsize<1)
		bsize = 1;
}

void cmd_seek(char *arg)
{
	oseek = seek = str2ull(arg);
}

void cmd_write(char *arg)
{
	arg = skipspaces(arg);
	if (*arg=='"') {
		printf("write string ('%s')\n", arg);
	} printf("write hexpair ('%s')\n", arg);
}

void cmd_help(char *arg)
{
	if (*arg) {
		ull ret = str2ull(arg);
		printf("0x%llx %lld 0%llo\n", ret, ret, ret);
	} else fprintf(stderr, "Check source for help.\n");
}
