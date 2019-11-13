#include <apluswin32.h>

int getchx()
{
	int c;
	while(!kbhit());
	c=getch();
	if((c==0)||(c==0xE0))c=(c<<8)|getch();
	return c;
}

int main(int argc, char* argv[])
{
	int c=' ';
	char *p,b[128];
	aplusw32_init();
	//while((0xFF&c)!='q'){c=getchex();printf("%05X (%c)\n",c,0xFF&c);};
	while(p=cgetsex(b,sizeof(b)))printf("%s",p);
	//while(p=_cgets(b))printf("%s",p);
	return 0;
}
