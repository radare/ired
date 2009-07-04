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
		sscanf(str, "0x%08llx", &ret);
	else sscanf(str, "%lld", &ret);
	return ret;
}

/* TODO : cleanup */
int hex2byte(unsigned char *val, unsigned char c) {
	if ('0' <= c && c <= '9')      *val = (unsigned char)(*val) * 16 + ( c - '0');
	else if (c >= 'A' && c <= 'F') *val = (unsigned char)(*val) * 16 + ( c - 'A' + 10);
	else if (c >= 'a' && c <= 'f') *val = (unsigned char)(*val) * 16 + ( c - 'a' + 10);
	else return 1;
	return 0;
}

/* TODO : cleanup */
int hexstr2raw(char *arg) {
	unsigned char *ptr, c, d;
	unsigned int j, len;
	len = c = d = j = 0;
	for (ptr=(unsigned char *)arg;*ptr;ptr++) {
		d = c;
		if (hex2byte(&c, *ptr))
			return -1;
		c |= d;
		if (j++ == 0) c <<= 4;
		if (j==2) {
			if (j>0) {
				arg[len++] = c;
				c = j = 0;
			}
			if (ptr[0]==' ')
				continue;
		}
	}
	return len;
}

/* TODO: endian swap helper */
