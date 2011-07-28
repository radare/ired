/* Copyleft 2009-2010 -- pancake /at/ nopcode /dot/ org */

#include <ctype.h>

static inline void hexdump(const ut8 *buf, unsigned int len, int w) {
	unsigned int i, j;
	for(i=0;i<len;i+=w) {
		printf("0x%08"LLF"x: ", curseek+i);
		for(j=i;j<i+w;j++) {
			if(j>=len) {
				printf(j%2?"   ":"  ");
				continue;
			}
			printf(j%2?"%02x ":"%02x", buf[j]);
		}
		printf(" ");
		for(j=i;j<i+w;j++)
			printf("%c", isprint(buf[j])?buf[j]:'.');
		printf("\n");
	}
}

static void print_fmt(const ut8 *buf, char *fmt, unsigned int len) {
	unsigned int i, up, inc = 0, lup = 0, rep = 0;
	char *ofmt = fmt;
	do { /* TODO: needs cleanup */
		for(;(*fmt||rep);fmt++) {
			up = rep?rep:*fmt;
			switch(up) {
			case 'i': if(len>3) printf("%d\n", ((buf[0]<<24) | (buf[1]<<16) | (buf[2]<<8) | buf[3])); inc=4; break;
			case 'I': if(len>3) printf("%d\n", ((buf[3]<<24) | (buf[2]<<16) | (buf[1]<<8) | buf[0])); inc=4; break;
			case 's': if(len>1) printf("%d\n", (buf[0]<<8 | buf[1])); inc=2; break;
			case 'S': if(len>1) printf("%d\n", (buf[1]<<8 | buf[0])); inc=2; break;
			case 'o': if(len>0) printf("0%o\n", buf[0]); inc=1; break;
			case 'b': if(len>0) printf("0x%02x\n", buf[0]); inc=1; break;
			case 'w': if(len>1) printf("0x%02x%02x\n", buf[1], buf[0]); inc=2; break;
			case 'W': if(len>1) printf("0x%02x%02x\n", buf[0], buf[1]); inc=2; break;
			case 'd': if(len>3) printf("0x%02x%02x%02x%02x\n",buf[3], buf[2], buf[1], buf[0]); inc=4; break;
			case 'D': if(len>3) printf("0x%02x%02x%02x%02x\n", buf[0], buf[1], buf[2], buf[3]); inc=4; break;
			case 'q': if(len>7) printf("0x%02x%02x%02x%02x%02x%02x%02x%02x\n",
				  buf[7], buf[6], buf[5], buf[4], buf[3], buf[2], buf[1], buf[0]); inc=8; break;
			case 'Q': if(len>7) printf("0x%02x%02x%02x%02x%02x%02x%02x%02x\n",
				  buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]); inc=8; break;
			case '.': inc=1; break;
			case ':': inc=4; break;
			case 'z': for(i=0; inc<len && isascii(buf[0]);i++) { printf(i?"%c\n":"%c", buf[0]); buf++; } inc++; break;
			case 'Z': for(i=0; inc<len && isascii(buf[0]);i++) { printf(i?"%c\n":"%c", buf[0]); buf+=2; } inc+=2; break;
			case '*': rep = lup; break;
			default: fprintf(stderr, "Unknown format '%c' (%d)\n", up, up);
				return;
			}
			if(!rep) lup = up;
			buf += inc;
			if(inc>len) break;
			len -= inc;
		}
		fmt = ofmt;
	} while(!rep && inc && inc<len);
}

static ut64 str2ut64(const char *str) {
	ut64 ret = 0LL;
	SKIPSPACES(str);
	if(str[0]=='b'&&str[1]=='\0')
		ret = bsize;
	else if(str[0]=='0') {
		if(str[1]=='x') sscanf(str, "0x%"LLF"x", &ret);
		else sscanf(str, "0x%"LLF"o", &ret);
	} else sscanf(str, "%"LLF"u", &ret);
	str += strlen(str)-1;
	if(*str=='K') ret *= 1024;
	else if(*str=='M') ret *= 1024*1024;
	else if(*str=='G') ret *= 1024*1024*1024;
	return ret;
}

static int hex2byte(ut8 *val, ut8 c) {
	if('0' <= c && c <= '9') *val = (ut8)(*val) * 16 + ( c - '0');
	else if(c >= 'A' && c <= 'F') *val = (ut8)(*val) * 16 + ( c - 'A' + 10);
	else if(c >= 'a' && c <= 'f') *val = (ut8)(*val) * 16 + ( c - 'a' + 10);
	else return 1;
	return 0;
}

static unsigned int hexstr2raw(ut8 *arg) {
	ut8 *ptr, d, c = 0;
	unsigned int j = 0, len = 0;
	for(ptr=(ut8 *)arg; *ptr; ptr++) {
		if(*ptr==' ') continue;
		d = c;
		if(hex2byte(&c, *ptr))
			return -1;
		c |= d;
		if(!j++) c <<= 4;
		else if(j==2) {
			arg[len++] = c;
			c = j = 0;
		}
	}
	return len;
}

static ut8 *getcurblk(const char *arg, int *len) {
	ut8 *buf = NULL;
	if(*arg) {
		*len = (int)str2ut64((const char *)arg);
		if(*len<1) *len = bsize;
	}
	if(*len>0 && (buf = malloc(*len)) != NULL) {
		if(io_seek((int)curseek, SEEK_SET)<0) {
			free(buf);
			buf = NULL;
		} else *len = io_read(buf, *len);
	}
	return buf;
}
