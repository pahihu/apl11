#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "kapl.h"

#define WIN32CON_TEST	1

static unsigned char alt_key[256];
static unsigned char alt_shift_key[256];

static void nl();
static void bksp();
static void scroll(HANDLE,INT);

static HANDLE hOut,hIn;
static CONSOLE_SCREEN_BUFFER_INFO	csbi;
static int   old_mode,new_mode;

static char  *conbuf;	/* current console buffer */
static int	  conmax;	/* max. size of buffer	  */
static int	  conlen;	/* console buffer length  */
static int	  curpos;	/* cursor position in the buffer */

static short  homex,homey;	/* current line home */
static short  maxx,maxy;	/* screen size		 */

static int	insovr=0;		/* insovr 1/0 mode	  */

#define HISTORY 256
static char *h[HISTORY];	/* history buffer */
static int  hlen=0;			/* where to place next line */
static int  hpos;			/* position in the history list */

static FILE *hf=0;			/* history file */

static void build_keymap()
{
	int i;

	for(i=0;i<256;i++)
		alt_key[i]=alt_shift_key[i]=0;
	for(i=0;i<KAPLSIZE;i+=4){
		      alt_key[kbd_kapl[i  ]]=kbd_kapl[i+2];
		alt_shift_key[kbd_kapl[i+1]]=kbd_kapl[i+3];
	}
}

static void put_char(int k)
{
	int i;

	WriteFile(hOut,&k,1,&i,NULL);
}

static void put_str(char *s,int n)
{
	int i;
	WriteFile(hOut,s,n,&i,NULL);
}

static void bell()
{
	MessageBeep(-1);
}

static void gotoxy(int x,int y)
{
	COORD c;

	c.X=x;c.Y=y;
	SetConsoleCursorPosition(hOut,c);
}

static void getxy(short *x,short *y)
{
	GetConsoleScreenBufferInfo(hOut,&csbi);
	*x=csbi.dwCursorPosition.X;
	*y=csbi.dwCursorPosition.Y;
}

static void gotoleft(int n)
{
	short x,y;
	int i;

	getxy(&x,&y);
	for(i=0;i<n;i++)
		if(x-1>=0)x--;
		else{x=maxx-1;y--;}
	gotoxy(x,y);
}

static void gotoright(int n)
{
	short x,y;
	int i;

	getxy(&x,&y);
	for(i=0;i<n;i++)
		if(x+1<maxx)x++;
		else{x=0;y++;}
	gotoxy(x,y);
}

static void histfill()
{
	if(hlen){
		if(hpos<0)hpos=hlen-1;
		if(hpos==hlen)hpos=0;

		memset(conbuf,' ',conlen);
		gotoxy(homex,homey); put_str(conbuf,conlen);
		strcpy(conbuf,h[hpos]); conlen=curpos=strlen(h[hpos]);
		gotoxy(homex,homey); put_str(conbuf,conlen);
	}else bell();
}

static void histadd(char *buf)
{
	if(buf){
		int l=strlen(buf)-1;
		if(buf[l]=='\n')buf[l]='\0';
		h[hlen]=strdup(buf);
		hlen=(hlen+1)%HISTORY;
		if(hf){
			fputs(buf,hf);fputc('\n',hf);
			fflush(hf);
		}
	}
}

static void change_cursor()
{
	CONSOLE_CURSOR_INFO	cci;

	insovr=1-insovr;
	cci.dwSize=insovr?90:15;
	cci.bVisible=TRUE;
	SetConsoleCursorInfo(hOut,&cci);
}

static void gotoend()
{
	gotoxy(homex,homey);gotoright(curpos=conlen);
}

static int key_event(KEY_EVENT_RECORD e)
{
	int i,k;
	short vk;
	int c;
	int s;
	short x,y;

	if(e.bKeyDown){
		vk=e.wVirtualKeyCode;
		k=e.uChar.AsciiChar;
		switch(vk){
		case VK_HOME:
			gotoxy(homex,homey);curpos=0;
			break;
		case VK_END:
			gotoend();
			break;
		case VK_INSERT:
			change_cursor();
			break;
		case VK_DELETE:
			if(curpos!=conlen){
				getxy(&x,&y);
				memmove(conbuf+curpos,conbuf+curpos+1,conlen-curpos-1);
				conbuf[conlen-1]=' ';
				put_str(conbuf+curpos+1,conlen-curpos-1);
				gotoxy(x,y);
				--conlen;
			}else bell();
			break;
		case VK_LEFT:
			if(curpos){
				--curpos;
				gotoleft(1);
			}else bell();
			break;
		case VK_RIGHT:
			if(curpos!=conlen){
				++curpos;
				gotoright(1);
			}else bell();
			break;
		case VK_UP:
			hpos--;histfill();
			break;
		case VK_DOWN:
			hpos++;histfill();
			break;
		case VK_RETURN:
			gotoend();nl();
			conbuf[conlen]='\0';
			return 1;
			break;
		case VK_BACK:
			if(curpos){
				gotoleft(1); getxy(&x,&y);
				memmove(conbuf+curpos-1,conbuf+curpos,conlen-curpos);
				conbuf[conlen-1]=' ';
				if(conlen==curpos)put_str(conbuf+curpos-1,1);
				else put_str(conbuf+curpos-1,conlen-curpos+1);
				gotoxy(x,y);
				--conlen;
				--curpos;
			}else bell();
			break;
		default:
			if(conlen==conmax-1)bell();
			else if(k>31){
				s=e.dwControlKeyState;
				if((s&LEFT_ALT_PRESSED)||
			   	(s&RIGHT_ALT_PRESSED)){
					if((s&SHIFT_PRESSED)||
				   	(s&CAPSLOCK_ON))
					 	k=alt_shift_key[k]?alt_shift_key[k]:k;
					else k=alt_key[k]?alt_key[k]:k;
				}
				if(insovr){
					getxy(&x,&y);
					memmove(conbuf+curpos+1,conbuf+curpos,conlen-curpos);
					conbuf[curpos]=k;
					put_str(conbuf+curpos,conlen-curpos+1);
					gotoxy(x,y); gotoright(1);
					conlen++;
				}else{
					getxy(&x,&y);
					conbuf[curpos]=k;
					put_char(k);
					if(curpos==conlen)conlen++;
				}
				curpos++;
			}
			break;
		}
	}
	return 0;
}

static void bksp()
{
	GetConsoleScreenBufferInfo(hOut,&csbi);
	if(csbi.dwCursorPosition.Y==0) bell();
	else {
		if(csbi.dwCursorPosition.X==0){
			csbi.dwCursorPosition.Y--;
			csbi.dwCursorPosition.X=csbi.dwSize.X-1;
		} else csbi.dwCursorPosition.X--;
		SetConsoleCursorPosition(hOut,csbi.dwCursorPosition);
	}
}

static void nl()
{
	GetConsoleScreenBufferInfo(hOut,&csbi);
	csbi.dwCursorPosition.X=0;
	if((csbi.dwSize.Y-1)==csbi.dwCursorPosition.Y)
		scroll(hOut,1);
	else csbi.dwCursorPosition.Y+=1;
	SetConsoleCursorPosition(hOut,csbi.dwCursorPosition);
}

static void scroll(HANDLE h,int n)
{
	SMALL_RECT	s;
	CHAR_INFO	f;
	COORD		d;

	s.Top = 1;
	s.Bottom = maxy-1;
	s.Left=0;
	s.Right=maxx-1;

	d.X=0;
	d.Y=0;
	f.Attributes=FOREGROUND_GREEN;
	f.Char.AsciiChar=' ';
	ScrollConsoleScreenBuffer(h,&s,NULL,d,&f);
}

void prep_terminal()
{
	static int _kbd=1;

	if(_kbd)build_keymap();

	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfo(hOut,&csbi);
	if(_kbd){
		maxx=csbi.dwSize.X;
		maxy=csbi.dwSize.Y;
	}

	GetConsoleMode(hIn,&old_mode);
	new_mode=old_mode & ~(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT);
	SetConsoleMode(hIn,new_mode);
	_kbd=0;
}

void deprep_terminal()
{
	SetConsoleMode(hIn,old_mode);
}

char* readline(char*buf,int n)
{
	int	cnt,i;
	int done=0;
	INPUT_RECORD	inbuf[128];

	if(hf==0){
		FILE *cf=fopen("session.log","r");
		if(cf){
			while(!feof(cf))histadd(fgets(buf,n,cf));
			fclose(cf);
			hf=fopen("session.log","a+");
		}else hf=fopen("session.log","w+");
	}
	prep_terminal();
	memset(buf,0,n);

	hpos=hlen;
	conbuf=buf; curpos=conlen=0; conmax=n;
	getxy(&homex,&homey);
	while(!done) {
		if(!ReadConsoleInput(hIn,inbuf,128,&cnt))break;
		for(i=0;i<cnt;i++)
			switch(inbuf[i].EventType){
			case KEY_EVENT:
				done=key_event(inbuf[i].Event.KeyEvent);
				break;
			case WINDOW_BUFFER_SIZE_EVENT:
			case FOCUS_EVENT:
			case MENU_EVENT:
				break;
			}
	}
	insovr=1;change_cursor();
	deprep_terminal();
	histadd(buf);
	return buf;
}

#if WIN32CON_TEST
#include <stdio.h>
#define BUFLEN	128
int main()
{
	char buf[BUFLEN+1];

	while(1){
		printf("\t");
		printf("%s\n",readline(buf,BUFLEN));
	}
}
#endif
