#include <windows.h>

#include "kapl.h"

#define WIN32CON_TEST	1

static unsigned char alt_key[256];
static unsigned char alt_shift_key[256];

static void nl();
static void bksp();
static void scroll(HANDLE,INT);

static HANDLE hOut,hIn;
static CONSOLE_SCREEN_BUFFER_INFO	csbi;
static short old_color;
static int   old_mode,new_mode;

static char  *conbuf;
static int	  conmax;
static int	  conpos;

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

static int key_event(KEY_EVENT_RECORD e)
{
	int i,k;
	short vk;
	int c;
	int s;

	if(e.bKeyDown){
		vk=e.wVirtualKeyCode;
		k=e.uChar.AsciiChar;
		if(vk==VK_RETURN){
			nl();
			conbuf[conpos]='\0';
			return 1;
		} else if(vk==VK_BACK){
			bksp();
			if(conpos)conpos--;
		} else if(conpos==conmax-1){
			MessageBeep(-1);
		} else if(k>31){
			s=e.dwControlKeyState;
			if((s&LEFT_ALT_PRESSED)||
			   (s&RIGHT_ALT_PRESSED)){
				if((s&SHIFT_PRESSED)||
				   (s&CAPSLOCK_ON))
					 k=alt_shift_key[k]?alt_shift_key[k]:k;
				else k=alt_key[k]?alt_key[k]:k;
			}
			put_char(k);
			conbuf[conpos++]=k;
		}
	}
	return 0;
}

static void bksp()
{
	GetConsoleScreenBufferInfo(hOut,&csbi);
	if(csbi.dwCursorPosition.X==0) MessageBeep(-1);
	else {
		csbi.dwCursorPosition.X--;
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
	s.Bottom = csbi.dwSize.Y-1;
	s.Left=0;
	s.Right=csbi.dwSize.X-1;

	d.X=0;
	d.Y=0;
	f.Attributes=FOREGROUND_GREEN;
	f.Char.AsciiChar=' ';
	ScrollConsoleScreenBuffer(h,&s,NULL,d,&f);
}

void prep_terminal()
{
	build_keymap();
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfo(hOut,&csbi);
	old_color=csbi.wAttributes;
	SetConsoleTextAttribute(hOut,FOREGROUND_GREEN);

	GetConsoleMode(hIn,&old_mode);
	new_mode=ENABLE_WINDOW_INPUT|ENABLE_PROCESSED_INPUT;
	SetConsoleMode(hIn,new_mode);
}

void deprep_terminal()
{
	SetConsoleMode(hIn,old_mode);
	SetConsoleTextAttribute(hOut,old_color);
}

char* readline(char*buf,int n)
{
	int	cnt,i;
	int done=0;
	INPUT_RECORD	inbuf[128];

	conbuf=buf; conpos=0; conmax=n;
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
	return buf;
}

#if WIN32CON_TEST
#define BUFLEN	20
int main()
{
	char buf[BUFLEN+1];

	prep_terminal();
	while(1){
		printf("%s\n",readline(buf,BUFLEN));
	}
	deprep_terminal();
}
#endif
