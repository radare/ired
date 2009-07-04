/* Copyleft 2009 -- pancake /at/ nopcode /dot/ org */

void cmd_hexdump(char *arg) {
	int i, j, inc = 16;
	unsigned int len = bsize;
	unsigned char *buf;

	if (*arg) {
		len = (int)str2ull(arg);
		if (len <1)
			len = bsize;
	} 
	lseek(fd, seek, SEEK_SET);
	buf = malloc(len);
	if (buf == NULL) {
		fprintf(stderr, "Cannot malloc %d bytes.\n", len);
		return;
	}
	len = read(fd, buf, len);

	for(i=0;i<len;i+=inc) {
		printf("0x%08llx ", seek+i);
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
	free(buf);
}

void cmd_bsize(char *arg) {
	bsize = (int)str2ull(arg);
	if (bsize<1)
		bsize = 1;
}

void cmd_seek(char *arg) {
	oseek = seek = str2ull(arg);
}

void cmd_write(char *arg) {
	int len;
	arg = skipspaces(arg);
	if (*arg=='"') {
		arg++;
		len = strlen(arg)-1;
		arg[len]='\0';
	} else {
		len = hexstr2raw(arg);
		printf("write hexpair ('%d')\n", len);
	}
	lseek(fd, seek, SEEK_SET);
	write(fd, arg, len);
}

void cmd_help(char *arg) {
	if (*arg) {
		ull ret = str2ull(arg);
		printf("0x%llx %lld 0%llo\n", ret, ret, ret);
	} else fprintf(stderr, "Check source for help.\n");
}
