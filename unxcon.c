#include <stdio.h>

void csi(int c,int b)
{
	if (b) printf("\x1b[%d;1m",c);
	else printf("\x1b[%dm",c);
	fflush(stdout);
}

void ttyerror()
{
	csi(31,1);
}

void ttyquery()
{
	csi(30,0);
}

void ttyanswer()
{
	csi(34,1);
}
