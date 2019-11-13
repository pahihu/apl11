/*********************************************************************
 *
 *     Win32 console keyboard driver
 *
 *		(c" 2003-2008, Ergodesign Bt.
 *
 */
#include <windows.h>

#include "apl79_fon.h"
#include "apluskbd.c"

static HANDLE	hIn;
static int old_mode, new_mode;
static int cs;
static unsigned char kbd[256];

static void
win_initkbd(char **map,int n)
{
	int i;

	for(i=0;i<256;i++)
		kbd[i]=i;

	if(map){
		for(i=0;i<n;i+=2)
			kbd[map[i][0]]=map[i+1][0];
	}
}

int
win_terminal()
{
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hIn,&old_mode);
	new_mode = (old_mode & ~(ENABLE_LINE_INPUT|ENABLE_PROCESSED_INPUT));
	win_initkbd(aplus_kbd,sizeof(aplus_kbd)/sizeof(char*));
	return 0;
}

int
win_ctrlstate()
{
	return cs;
}

int
win_getch()
{
	INPUT_RECORD r;
	KEY_EVENT_RECORD kr;
	static int kbd_map[]={
		 0, 0, 0,79,71,75, 0,77,	/* HOME,LEFT,RIGHT,END,DEL */
		 0, 0, 0, 0, 0, 0,83, 0,
	};

	int n;
	int cks;
	static int k;
	int ret;

	SetConsoleMode(hIn,new_mode);

	if(k){
		ret=k;k=0;
		goto Lret;
	}

	for(;;){
		ReadConsoleInput(hIn,&r,1,&n);
		if(n==0)continue;

		if(r.EventType==KEY_EVENT){
			kr=r.Event.KeyEvent;
			if(kr.bKeyDown==TRUE){

				cks=kr.dwControlKeyState;cs=0;
				cs|=(cks&(SHIFT_PRESSED|CAPSLOCK_ON)?0x01:0x00);
				cs|=(cks&(LEFT_ALT_PRESSED|RIGHT_ALT_PRESSED)?0x02:0x00);
				cs|=(cks&(LEFT_CTRL_PRESSED|RIGHT_CTRL_PRESSED)?0x04:0x00);

				ret=kr.uChar.AsciiChar;
				if(ret)break;
				ret=kr.wVirtualKeyCode;
				if(0x20<ret && ret<0x2f){
					k=kbd_map[ret-0x20];
					ret=0;
					break;
				}
			}
		}
	}
	if(ret&&(cs&0x02)){
		ret=kbd[ret];
	}
Lret:
	SetConsoleMode(hIn,old_mode);
	return ret;
}
