/* Copyleft 2009 -- pancake /at/ nopcode /dot/ org */

void cmd_hexdump(char *arg) {
	unsigned int len = bsize;
	unsigned char *buf = getcurblk(arg, &len);
	if (buf) {
		hexdump(buf, len);
		free(buf);
	}
}

/* TODO: cleanup and simplify!! */
void cmd_print(char *arg) {
	unsigned char *buf, *obuf;
	char *oarg = arg;
	unsigned int i, inc=0, lup=0, up, len = bsize, rep = 0;
i=0;
	if (!*arg) printf( /* TODO: simplify help message */
		"b/w/d/q   byte, word, dword, qword (little endian)\n"
		"B/W/D/Q   byte, word, dword, qword (big endian)\n"
		"i/I/f/F   int32 (lil, big), float (lil, big)\n"
		"s/S       short int16 (lil, big)\n"
		"z/Z       zero-terminatted string (ascii, wide-ascii)\n"
		"o         octal byte\n"
		"./:       skip 1 or 4 chars\n"
		"*         repeat last format until death\n");
	else {
		obuf = buf = getcurblk(arg, &len);
		if (buf != NULL)
		do {
			/* TODO: move into util.c as print_fmt */
			for(;(*arg||rep);arg++) {
				up = rep?rep:*arg;
				switch(up) {
				case 'i': if (len>3) printf("%d\n", ((buf[0]<<24) | (buf[1]<<16) | (buf[2]<<8) | buf[3])); inc=4; break;
				case 'I': if (len>3) printf("%d\n", ((buf[3]<<24) | (buf[2]<<16) | (buf[1]<<8) | buf[0])); inc=4; break;
				case 's': if (len>1) printf("%d\n", (buf[0]<<8 | buf[1])); inc=2; break;
				case 'S': if (len>1) printf("%d\n", (buf[1]<<8 | buf[0])); inc=2; break;
				case 'o': if (len>0) printf("%oo\n", buf[0]); inc=1; break;
				case 'B': case 'b': if (len>0) printf("0x%02x\n", buf[0]); inc=1; break;
				case 'w': if (len>1) printf("0x%02x%02x\n", buf[1], buf[0]); inc=2; break;
				case 'W': if (len>1) printf("0x%02x%02x\n", buf[0], buf[1]); inc=2; break;
				case 'd': if (len>3) printf("0x%02x%02x%02x%02x\n",buf[3], buf[2], buf[1], buf[0]); inc=4; break;
				case 'D': if (len>3) printf("0x%02x%02x%02x%02x\n", buf[0], buf[1], buf[2], buf[3]); inc=4; break;
				case 'q': if (len>7) printf("0x%02x%02x%02x%02x%02x%02x%02x%02x\n",
					  buf[7], buf[6], buf[5], buf[4], buf[3], buf[2], buf[1], buf[0]); inc=8; break;
				case 'Q': if (len>7) printf("0x%02x%02x%02x%02x%02x%02x%02x%02x\n",
					  buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]); inc=8; break;
				case '.': inc=1; break;
				case ':': inc=4; break;
				case 'z': 
					  for(i=0; inc<len && buf[0] ;i++) { 
						  printf("%c", buf[0]); 
						  buf++; inc++; } 
						  if (i) printf("\n"); break;
				case '*': rep = lup; break;
				default: fprintf(stderr, "UNKNOWN FORMAT CHAR (%d)(%c)\n", up, up);
				}
				if (!rep) lup = up;
				buf += inc;
				if (inc>len) break;
				len -= inc;
			}
			if (inc>len) break; /* XXX: ugly */
			arg = oarg;
		} while(!rep);
		free(obuf);
	}
}

void cmd_bytedump(char *arg) {
	unsigned int i, len = bsize;
	unsigned char *buf = getcurblk(arg, &len);
	if (!buf) return;
	for(i=0;i<len;i++)
		printf("%02x", buf[i]);
	printf("\n");
	free(buf);
}

void cmd_search(char *arg) {
	unsigned char *buf;
	unsigned int i, len, hit=0;
	arg = skipspaces(arg);
	if (*arg=='"') {
		arg++;
		len = strlen(arg)-1;
		arg[len]='\0';
	} else len = hexstr2raw(arg);
	buf = getcurblk("", &len);
	do {
		for(i=0;i<bsize;i++) {
			if (arg[hit++]!=buf[i])
				hit = 0;
			else if (hit == len)
				printf("0x%llx\n", seek+i-len+1);
		}
		seek+=bsize;
	} while(io_read(buf, bsize)>0);
	free(buf);
}

void cmd_bsize(char *arg) {
	if (!*arg)
		printf("%d\n", bsize);
	else if (*arg=='+')
		bsize += (int)str2ut64(arg+1);
	else if (*arg=='-')
		bsize -= (int)str2ut64(arg+1);
	else bsize = str2ut64(arg);
	if (bsize<1)
		bsize = 1;
	obsize = bsize;
}

void cmd_seek(char *arg) {
	if (!*arg)
		printf("%lld\n", seek);
	else if (*arg=='+')
		oseek = seek += str2ut64(arg+1);
	else if (*arg=='-')
		oseek = seek -= str2ut64(arg+1);
	else oseek = seek = str2ut64(arg);
}

void cmd_dump(char *file) {
	unsigned int len = bsize;
	void *buf = getcurblk("", &len);
	FILE *fd = fopen(file, "wb");
	fwrite(buf, len, 1, fd);
	fclose(fd);
	free(buf);
}

void cmd_load(char *file) {
	FILE *fd = fopen(file, "rb");
	int len = bsize;
	void *buf;
	if (!fd)
		return;
	buf = malloc(bsize);
	if (!buf)
		return;
	len = fread(buf, 1, len, fd);
	io_write(buf, len);
	fclose(fd);
	free(buf);
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
		ut64 ret = str2ut64(arg);
		printf("0x%llx %lld 0%llo\n", ret, ret, ret);
	} else printf(
	"s[+-addr]     seek to relative or absolute address\n"
	"b[+-size]     change block size\n"
	"w[hex|\"str\"]  write hexpair or string\n"
	"/[hex|\"str\"]  search hexpair or string\n"
	"x[size]       hexdump\n"
	"X[size]       hexpair dump\n"
	"p[fmt]        print current block with format ('p' for help)\n"
	".[file]       interpret file\n"
	"<[file]       load file in current seek\n"
	">[file]       dump current block to file\n"
	"!cmd          run shell command\n"
	"q             quit\n");
}

void cmd_system(char *arg) {
	FILE *f;
	int len;
	char *buf = malloc(bsize+128);
	if (strstr(arg, "BLOCK")) {
		setenv("BLOCK", ".curblk", 1);
		f = fopen(".curblk", "w");
		io_seek(seek, SEEK_SET);
		len = io_read(buf, bsize);
		fwrite(buf, len, 1, f);
		fclose(f);
	}
	if (strstr(arg, "FILE"))
		setenv("FILE", "/bin/ls", 1); // XXX
	if (strstr(arg, "OFFSET")) {
		sprintf(buf, "%lld", seek);
		setenv("OFFSET", buf, 1); // XXX
	}
	if (strstr(arg, "XOFFSET")) {
		sprintf(buf, "0x%llx", seek);
		setenv("XOFFSET", buf, 1); // XXX
	}
	io_system(arg);
	unlink(".curblk");
	free(buf);
}
