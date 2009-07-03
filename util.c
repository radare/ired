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

/* TODO: endian swap helper */
