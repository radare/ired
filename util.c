/* Copyleft 2009 -- pancake /at/ nopcode /dot/ org */

inline char *skipspaces(char *arg) {
	if (*arg==' '||*arg=='\t')
		arg++;
	return arg;
}

ut64 str2ut64(char *str) {
	ut64 ret = 0LL;
	str = skipspaces(str);
	if (str[0]=='0'&&str[1]=='x')
		sscanf(str, "0x%llx", &ret);
	else sscanf(str, "%lld", &ret);
	return ret;
}

/* TODO : remove and use sscanf? */
int hex2byte(ut8 *val, ut8 c) {
	if ('0' <= c && c <= '9')      *val = (ut8)(*val) * 16 + ( c - '0');
	else if (c >= 'A' && c <= 'F') *val = (ut8)(*val) * 16 + ( c - 'A' + 10);
	else if (c >= 'a' && c <= 'f') *val = (ut8)(*val) * 16 + ( c - 'a' + 10);
	else return 1;
	return 0;
}

/* TODO : cleanup */
int hexstr2raw(char *arg) {
	unsigned int j, len;
	ut8 *ptr, c, d;
	len = c = d = j = 0;
	for (ptr=(ut8 *)arg;*ptr;ptr++) {
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

void *getcurblk(char *arg, unsigned int *len) {
	void *buf;
	if (*arg) {
		*len = (int)str2ut64(arg);
		if (*len <1)
			*len = bsize;
	}
	buf = malloc(*len);
	if (buf == NULL || io_seek(seek, SEEK_SET)<0) {
		free(buf);
		buf = NULL;
	} else *len = io_read(buf, *len);
	return buf;
}

/* TODO: endian swap helper */
