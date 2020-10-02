/* cmd.c - MIT - Copyright 2009-2019 -- pancake /at/ nopcode /dot/ org */

#ifndef HAVE_SYSTEM
#define HAVE_SYSTEM 1
#endif

static int cmd_hexdump(char *arg) {
	int len = bsize;
	ut8 *buf = getcurblk(arg, &len);
	if(buf && len>0) {
		hexdump(buf, len, 16);
		free(buf);
		return 1;
	}
	return 2;
}

static int cmd_print(char *arg) {
	ut8 *buf;
	int len = bsize;
	if(*arg) {
		buf = getcurblk(arg, &len);
		if(!buf) return 2;
		print_fmt(buf, arg, len);
		free(buf);
	} else printf(
		"ob/wW/dD/qQ  byte (oct,hex), word, dword, qword (lil, big endian)\n"
		"i/I/f/F      int32 (lil, big), float (lil, big)\n"
		"s/S          short int16 (lil, big)\n"
		"z/Z          zero-terminatted string (ascii, wide-ascii)\n"
		"./:/*        skip 1 or 4 chars, repeat last format instead of cycle\n");
	return 1;
}

static int cmd_bytedump(const char *arg) {
	int i, len = bsize;
	ut8 *buf = getcurblk(arg, &len);
	if(!buf) return 2;
	for(i=0;i<len;i++)
		printf("%02x", buf[i]);
	printf("\n");
	free(buf);
	return 1;
}

static int cmd_search(const char *arg) {
	ut8 *buf, *barg;
	size_t len;
	int i, hit = 0;
	SKIPSPACES(arg);
	if(*arg=='"') {
		arg++;
		barg = (ut8*)strdup(arg);
		len = strlen((const char *)barg)-1;
		// TODO: ensure last char is '"'
		barg[len]='\0';
	} else {
		barg = (ut8*)strdup(arg);
		len = hexstr2raw(barg);
	}
	if((buf = getcurblk("", &bsize)))
	do {
		for(i=0;i<bsize;i++) {
			if(barg[hit++]!=buf[i]) hit = 0;
			else if(hit == len)
				printf("0x%"LLF"x\n", curseek+i-len+1);
		}
		curseek += bsize;
	} while(io_read(buf, bsize)>0);
	free(buf);
	free(barg);
	return 1;
}

static int cmd_bsize(char *arg) {
	if(!*arg) printf("%d\n", bsize);
	else if(*arg=='+') bsize += (int)str2ut64(arg+1);
	else if(*arg=='-') bsize -= (int)str2ut64(arg+1);
	else bsize = str2ut64(arg);
	if(bsize<1) bsize = 1;
	obsize = bsize;
	return 1;
}

static int cmd_seek(char *arg) {
	if(!*arg) printf("%"LLF"d\n", curseek);
	else if(*arg=='+') curseek += str2ut64(arg+1);
	else if(*arg=='-') curseek -= str2ut64(arg+1);
	else curseek = str2ut64(arg);
	io_seek((oldseek=curseek), 0);
	return 1;
}

static int cmd_dump(char *file) {
	int len = bsize;
	ut8 *buf = getcurblk("", &len);
	if(len<1 || buf) {
		FILE *fd = fopen(file, "wb");
		if (!fd) {
			free (buf);
			return -1;
		}
		if(fwrite(buf, len, 1, fd)<len) {
			perror("fwrite");
			return -1;
		}
		fclose(fd);
		free(buf);
	} else perror("getcurblk");
	return 1;
}

static int cmd_load(char *file) {
	FILE *fd = fopen(file, "rb");
	if(fd) {
		void *buf = malloc(bsize);
		if(bsize>0 && buf) {
			int len = fread(buf, 1, bsize, fd);
			if(len<bsize)
				perror("fread");
			io_seek (curseek, SEEK_SET);
			if(io_write(buf, len)<len) {
				perror("io_write");
				return -1;
			}
		} else perror("malloc");
		free(buf);
		fclose(fd);
	} else perror("fopen");
	return 1;
}

static int cmd_write(const char *arg) {
	ut8 *barg;
	int len;
	SKIPSPACES(arg);
	if(*arg=='"') {
		barg = (ut8*)strdup (arg+1);
		len = strlen(++arg)-1;
		// TODO: ensure last char is "
		barg[len] = '\0';
	} else {
		barg = (ut8*)strdup (arg);
		len = hexstr2raw(barg);
	}
	if (len<2)  {
		printf ("Invalid hexpair\n");
		return 2;
	}
	io_seek(curseek, SEEK_SET);
	if(io_write(barg, len)<len) {
		perror("io_write");
		return -1;
	}
	//free ((void*)arg);
	return 1;
}

static int cmd_help(char *arg) {
	if(*arg) {
		ut64 ret = r_num_calc (NULL, arg, NULL);
		printf("0x%"LLF"x %"LLF"d 0%"LLF"o\n", ret, ret, ret);
	} else printf(
		"s[+-addr]     seek to relative or absolute address\n"
		"b[+-size]     change block size\n"
		"w[hex|\"str\"]  write hexpair or string\n"
		"/[hex|\"str\"]  search hexpair or string\n"
		"x[size]       hexdump\n"
		"X[size]       hexpair dump\n"
		"p[fmt]        print formatted current block ('p' for help)\n"
		"d[size]       disasemble\n"
		"r[+-[num]]    truncate or -remove N bytes\n"
		".[file]       interpret file\n"
		"<[file]       load file in current seek\n"
		">[file]       dump current block to file\n"
		"!cmd          run shell command\n"
		"?expr         calculate numeric expression\n"
		"q             quit\n");
	return 1;
}

static int cmd_resize(char *arg) {
	ut8 *buf;
	ut64 tail, n;
	int i, len, ret = 0;
	switch(*arg) {
	case '\0':
		printf("%"LLF"d\n", (ut64)io_seek(0, SEEK_END));
		break;
	case '+': // XXX: needs cleanup
		n = str2ut64(arg+1);
		len = (ut64)io_seek(0, SEEK_END);
		tail = len-curseek;
		if((buf=malloc(tail))) { // XXX: Use block
			io_seek(curseek, SEEK_SET);
			io_read(buf, tail);
			io_seek(curseek+n, SEEK_SET);
			io_write(buf, tail);
			free (buf);
		} else perror("malloc");
		break;
	case '-':
		buf = malloc(bsize);
		if(buf) {
			n = str2ut64(arg+1);
			for(i=0;!ret;i+=len) {
				io_seek(curseek+n+i, SEEK_SET);
				if((len = io_read(buf, bsize))>0) {
					io_seek(curseek+i, SEEK_SET);
					if(io_write(buf, len)<len)
						perror("io_write"),ret=-1;
				} else break; //perror("io_read"), ret=-1;
			}
			free(buf);
			if((ret = io_seek(0, SEEK_END))>n)
				//ret = io_truncate(ret-n);
				ret = 0;
		} else perror("malloc");
		break;
	default:
		//ret = io_truncate(str2ut64(arg));
		ret = 0;
	}
	if(ret<0) perror("truncate");
	return 1;
}

static int cmd_system(char *arg) {
	int len = bsize;
	char str[1024];
	ut8 *buf;
	if(strstr(arg, "BLOCK")) {
		FILE *fd = fopen(".curblk", "wb");
		if(fd) {
			if((buf = getcurblk("", &len))) {
				setenv("BLOCK", ".curblk", 1);
				fwrite(buf, len, 1, fd);
				free(buf);
			}
			fclose(fd);
		}
	}
	if(strstr(arg, "BSIZE")) {
		sprintf(str, "%d", len);
		setenv("BSIZE", str, 1);
	}
	if(strstr(arg, "OFFSET")) {
		sprintf(str, "%"LLF"d", curseek);
		setenv("OFFSET", str, 1);
	}
	if(strstr(arg, "XOFFSET")) {
		sprintf(str, "0x%"LLF"x", curseek);
		setenv("XOFFSET", str, 1);
	}
#if HAVE_SYSTEM
	if(io_system(arg)<0)
		perror("system");
#endif
	unlink(".curblk");
	return 1;
}
