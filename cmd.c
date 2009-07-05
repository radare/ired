/* Copyleft 2009 -- pancake /at/ nopcode /dot/ org */

#define HEXWIDTH 16
void cmd_hexdump(char *arg) {
	unsigned int i, j, len = bsize;
	unsigned char *buf;

	if (*arg) {
		len = (int)str2ull(arg);
		if (len <1)
			len = bsize;
	} 
	buf = malloc(len);
	if (buf == NULL) {
		fprintf(stderr, "Cannot malloc %d bytes.\n", len);
		return;
	}
	io_seek(seek, SEEK_SET);
	len = io_read(buf, len);

	for(i=0;i<len;i+=HEXWIDTH) {
		printf("0x%08llx ", seek+i);
		for(j=i;j<i+HEXWIDTH;j++) {
			if (j>=len) {
				if (j%2) printf("   ");
				else printf("  ");
				continue;
			}
			printf("%02x", buf[j]);
			if (j%2) printf(" ");
		}
		for(j=i;j<i+HEXWIDTH;j++) {
			if (j >= len) printf(" ");
			else printf("%c", isprint(buf[j])?buf[j]:'.');
		}
		printf("\n");
	}
	free(buf);
}

void cmd_bytedump(char *arg) {
	unsigned int i, len = bsize;
	unsigned char *buf;
	if (*arg) {
		len = (int)str2ull(arg);
		if (len <1)
			len = bsize;
	}
	buf = malloc(len);
	if (buf == NULL) {
		fprintf(stderr, "Cannot malloc %d bytes.\n", len);
		return;
	}
	io_seek(seek, SEEK_SET);
	len = io_read(buf, len);
	for(i=0;i<len;i++)
		printf("%02x", buf[i]);
	printf("\n");
}

void cmd_bsize(char *arg) {
	if (!*arg)
		printf("%d\n", bsize);
	else if (*arg=='+')
		bsize+=str2ull(arg+1);
	else if (*arg=='-')
		bsize-=str2ull(arg+1);
	else bsize = str2ull(arg);
	if (bsize<1)
		bsize = 1;
}

void cmd_seek(char *arg) {
	if (!*arg)
		printf("%lld\n", seek);
	else if (*arg=='+')
		oseek = seek+=str2ull(arg+1);
	else if (*arg=='-')
		oseek = seek-=str2ull(arg+1);
	else oseek = seek = str2ull(arg);
}

void cmd_write(char *arg) {
	unsigned int len;
	arg = skipspaces(arg);
	if (*arg=='"') {
		arg++;
		len = strlen(arg)-1;
		arg[len]='\0';
	} else len = hexstr2raw(arg);
	io_seek(seek, SEEK_SET);
	io_write(arg, len);
}

void cmd_help(char *arg) {
	if (*arg) {
		ull ret = str2ull(arg);
		printf("0x%llx %lld 0%llo\n", ret, ret, ret);
	} else printf(
	"s[+-addr]    seek to relative or absolute address\n"
	"b[+-size]    change block size\n"
	"w hex|\"str\"  change block size\n"
	"x[size]    hexdump\n"
	"!cmd       run shell command\n"
	"q          quit\n");
}

void cmd_system(char *arg) {
	io_system(arg);
}
