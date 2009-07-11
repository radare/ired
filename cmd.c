/* Copyleft 2009 -- pancake /at/ nopcode /dot/ org */

static void cmd_hexdump(char *arg) {
	unsigned int len = bsize;
	unsigned char *buf = getcurblk(arg, &len);
	if (buf) {
		hexdump(buf, len, 16);
		free(buf);
	}
}

static void cmd_print(char *arg) {
	unsigned char *buf;
	unsigned int len = bsize;
	if (*arg) {
		buf = getcurblk(arg, &len);
		if (!buf) return;
		print_fmt(buf, arg, len);
		free(buf);
	} else printf(
		"ob/wW/dD/qQ  byte (oct,hex), word, dword, qword (little, big endian)\n"
		"i/I/f/F      int32 (lil, big), float (lil, big)\n"
		"s/S          short int16 (lil, big)\n"
		"z/Z          zero-terminatted string (ascii, wide-ascii)\n"
		"./:/*        skip 1 or 4 chars, repeat last format instead of cycle\n");
}

static void cmd_bytedump(char *arg) {
	unsigned int i, len = bsize;
	unsigned char *buf = getcurblk(arg, &len);
	if (!buf) return;
	for(i=0;i<len;i++)
		printf("%02x", buf[i]);
	printf("\n");
	free(buf);
}

void cmd_search(char *arg) {
	unsigned char *buf = NULL;
	unsigned int i, len, hit=0;
	arg = skipspaces(arg);
	if (*arg=='"') {
		arg++;
		len = strlen(arg)-1;
		arg[len]='\0';
	} else len = hexstr2raw(arg);
	if ((buf = getcurblk("", &len)))
	do {
		for(i=0;i<bsize;i++) {
			if (arg[hit++]!=buf[i]) hit = 0;
			else if (hit == len)
				printf("0x%llx\n", seek+i-len+1);
		}
		seek+=bsize;
	} while(io_read(buf, bsize)>0);
	free(buf);
}

static void cmd_bsize(char *arg) {
	if (!*arg) printf("%d\n", bsize);
	else if (*arg=='+') bsize += (int)str2ut64(arg+1);
	else if (*arg=='-') bsize -= (int)str2ut64(arg+1);
	else bsize = str2ut64(arg);
	if (bsize<1) bsize = 1;
	obsize = bsize;
}

static void cmd_seek(char *arg) {
	if (!*arg) printf("%lld\n", seek);
	else if (*arg=='+') oseek = seek += str2ut64(arg+1);
	else if (*arg=='-') oseek = seek -= str2ut64(arg+1);
	else oseek = seek = str2ut64(arg);
}

static void cmd_dump(char *file) {
	unsigned int len = bsize;
	void *buf = getcurblk("", &len);
	FILE *fd = fopen(file, "wb");
	fwrite(buf, len, 1, fd);
	fclose(fd);
	free(buf);
}

static void cmd_load(char *file) {
	void *buf;
	FILE *fd = fopen(file, "rb");
	if (!fd) return;
	buf = malloc(bsize);
	if (!buf) return;
	io_write(buf, fread(buf, 1, bsize, fd));
	fclose(fd);
	free(buf);
}

static void cmd_write(char *arg) {
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

static void cmd_help(char *arg) {
	if (*arg) {
		ut64 ret = str2ut64(arg);
		printf("0x%llx %lld 0%llo\n", ret, ret, ret);
	} else printf(
	"s[+-addr]     seek to relative or absolute address\n"
	"b[+-size]     change block size\n"
	"w[hex|\"str\"]  write hexpair or string\n"
	"/[hex|\"str\"]  search hexpair or string\n"
	"x[size]       hexdump\n"
	"X[size]       hexpair dump\n"
	"p[fmt]        print formatted current block ('p' for help)\n"
	"r[-[num]]     truncate or -remove N bytes\n"
	".[file]       interpret file\n"
	"<[file]       load file in current seek\n"
	">[file]       dump current block to file\n"
	"!cmd          run shell command\n"
	"?expr         calculate numeric expression\n"
	"q             quit\n");
}

static void cmd_resize(char *arg) {
	unsigned int len;
	if (!*arg) printf("%lld\n", io_seek(0, SEEK_END));
	else if (*arg=='-') {
		ut8 *buf = malloc(bsize);
		ut64 i, n = str2ut64(arg+1);
		if (buf) {
			for(i=0;;i+=len) {
				io_seek(seek+n+i, SEEK_SET);
				len = io_read(buf, bsize);
				if (len<1) break;
				io_seek(seek+i, SEEK_SET);
				io_write(buf, len);
			}
			free(buf);
		}
		io_truncate(io_seek(0, SEEK_END)-n);
	} else io_truncate(str2ut64(arg));
}

static void cmd_system(char *arg) {
	unsigned int len = bsize;
	char *buf = NULL, str[64];
	if (strstr(arg, "BLOCK")) {
		FILE *fd = fopen(".curblk", "w");
		if (fd) {
			buf = getcurblk("", &len);
			if (buf) {
				setenv("BLOCK", ".curblk", 1);
				fwrite(buf, len, 1, fd);
				fclose(fd);
				free(buf);
			}
		}
	}
	if (strstr(arg, "OFFSET")) {
		sprintf(str, "%lld", seek);
		setenv("OFFSET", str, 1); // XXX
	}
	if (strstr(arg, "XOFFSET")) {
		sprintf(str, "0x%llx", seek);
		setenv("XOFFSET", str, 1); // XXX
	}
	io_system(arg);
	unlink(".curblk");
	free(buf);
}
