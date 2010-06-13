#include <unistd.h>

int main() {
	char next=0, ch, och=0;
	while (read (0, &ch, 1)==1) {
		if (next) {
			if (ch==next)
				next = 0;
			continue;
		}
		if (och == ' ' && ch==' ')
			next = '\n';
		if (och == '0' && ch=='x')
			next = ':';
		if (!next && ch!=' ')
			write (1, &ch, 1);
		och = ch;
	}
	return 0;
}
