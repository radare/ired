/* Copyleft 2009 -- pancake /at/ nopcode /dot/ org */

inline char *skipspaces(char *arg) {
	if (*arg==' '||*arg=='\t')
		arg++;
	return arg;
}

ull str2ull(char *str) {
	ull ret = 0LL;
	str = skipspaces(str);
	if (str[0]=='0'&&str[1]=='x')
		sscanf(str, "0x%llx", &ret);
	else sscanf(str, "%lld", &ret);
	return ret;
}

/* TODO : remove and use sscanf? */
int hex2byte(uc8 *val, uc8 c) {
	if ('0' <= c && c <= '9')      *val = (uc8)(*val) * 16 + ( c - '0');
	else if (c >= 'A' && c <= 'F') *val = (uc8)(*val) * 16 + ( c - 'A' + 10);
	else if (c >= 'a' && c <= 'f') *val = (uc8)(*val) * 16 + ( c - 'a' + 10);
	else return 1;
	return 0;
}

/* TODO : cleanup */
int hexstr2raw(char *arg) {
	uc8 *ptr, c, d;
	unsigned int j, len;
	len = c = d = j = 0;
	for (ptr=(uc8 *)arg;*ptr;ptr++) {
		d = c;
		if (hex2byte(&c, *ptr))
			return -1;
		c |= d;
		if (j++ == 0) c <<= 4;
		if (j==2) {
			arg[len++] = c;
			c = j = 0;
			if (ptr[0]==' ')
				continue;
		}
	}
	return len;
}

void *getcurblk(char *arg) {
	void *buf;
	int len = bsize;
	if (*arg) {
		len = (int)str2ull(arg);
		if (len <1)
			len = bsize;
	}
	buf = malloc(len);
	if (buf == NULL) {
		fprintf(stderr, "Cannot malloc %d bytes.\n", len);
		return NULL;
	}
	io_seek(seek, SEEK_SET);
	len = io_read(buf, len);
	return buf;
}

/* TODO: endian swap helper */
