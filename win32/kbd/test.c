#include <stdio.h>

extern void win_terminal();
extern int win_getch();

int main(void)
{
	char c;
	int done;

	win_terminal();
	done = 0;
	while (!done) {
		c = win_getch();
		if (0 == c) c = win_getch();
		switch (c) {
		case 'Q': done = 1; continue;
		case '\r': printf("\n"); continue;
		}
		printf("%c", c);
	}
	return 0;
}