/*** Simply unIX ***/

/* History
  	 20021202AP		I drop fchmod which enables me to eliminate the entire
	 				fd_map structure...

	 The issues:
	 - open() calls does not provide non-blocking I/O semantics
	 - the file and socket interface is split -> no read/write on sockets

	 The consequences:
	 - CygWin provides a POSIX emulation with good semantic coverage
	 - providing non-blocking I/O and unified file/socket handling
	   ends up in rewriting the entire I/O library to work properly
	 - not all Windows systems provide non-blocking I/O

	 So I decided to drop those features and add some new, which
	 more better suited to Win32. So I drop the fd_map structure.
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <float.h>

#include <apluswin32.h>

#ifndef	_RC_UP
#define	_RC_UP	0x00000200
#endif

#ifndef _FPCLASS_NZ
#define _FPCLASS_NZ	0x0020
#endif

#ifndef _FPCLASS_PZ
#define _FPCLASS_PZ	0x0040
#endif

typedef char C;
typedef int I;
typedef double F;
#define R return
#define Z static

Z HANDLE fdh(I fd)
{
	R (HANDLE)_get_osfhandle(fd);
}

#define TNX_BOOL(x)		((x)?0:-1)

/*** SYSTEM LOG ***/
Z I _log=0;
Z I _logopt;
Z I _logfac;

void openlog(C* file,I opt,I fac)
{
	_log=open(file,O_CREAT|O_WRONLY|O_TEXT,S_IWRITE);
	_logopt=opt;
}

void syslog(I pri,const C* fmt,...)
{
	C buf[1024];
	C t[10];
	va_list lst;

	if(_log){
		sprintf(buf,"%s ",_strdate(t)); write(_log,buf,strlen(buf));
		sprintf(buf,"%s ",_strtime(t)); write(_log,buf,strlen(buf));
		if(_logopt&LOG_PID){
			sprintf(buf,"[%d] ",getpid()); write(_log,buf,strlen(buf));
		}
		va_start(lst,fmt);
		vsprintf(buf,fmt,lst);
		va_end(lst);
		write(_log,buf,strlen(buf));
		fsync(_log);
	}
}

void closelog()
{
	if(_log){
		close(_log);
		_log=0;
	}
}

/*** SIGNAL HANDLING ***/
I sigaction(I s,const struct sigaction *nh,struct sigaction *oh)
{
	Z struct sigaction _oh;

	oh = &_oh;
	_oh.sa_handler = signal(s,nh->sa_handler);
	R (_oh.sa_handler == SIG_ERR) ? -1 : 0;
}

I kill(I pid,I sig)
{
	if(pid!=getpid())R -1;
	R raise(sig);
}

/*** FILE I/O ***/
I fsync(I __fd)
{
	R _commit(__fd);
}

I ftruncate(I __fd,off_t __length)
{
	R _chsize(__fd,__length);
}

I truncate(const C* file,off_t __length)
{
	I fd;
	I r;

	fd=open(file,O_BINARY|O_RDWR);
	r=ftruncate(fd,__length);
	close(fd);
	R r;
}

I flock(I __fd,I __op)
{
	BOOL r;
	OVERLAPPED	ovl;
	HANDLE h=fdh(__fd);
	I	 s=GetFileSize(h,NULL);
	I  f=0;

    ovl.OffsetHigh=ovl.Offset=0;
	ovl.hEvent=0;

	if(__op&LOCK_UN)r=UnlockFileEx(h,0,s,0,&ovl);
	else{
	  if(__op&LOCK_EX)f|=LOCKFILE_EXCLUSIVE_LOCK;
	  if(__op&LOCK_NB)f|=LOCKFILE_FAIL_IMMEDIATELY;
	  r=LockFileEx(h,f,0,s,0,&ovl);
	}
	if(r==FALSE&&__op&LOCK_NB)errno=EAGAIN;
	R TNX_BOOL(r);
}

I lstat(const char *__path,struct stat *__buf)
{
	return stat(__path,__buf);
}

/*** MEMORY-MAPPED FILES ***/
caddr_t mmap(caddr_t __addr,size_t __len,I __prot,I __flags,I __fd,off_t __off)
{
	DWORD	prot;
	DWORD	acc;
	HANDLE	hnd=fdh(__fd);
	HANDLE	hMap;
	caddr_t	addr;

	prot=(__prot&PROT_WRITE)?PAGE_READWRITE:PAGE_READONLY;
	if(__flags&MAP_PRIVATE) prot=PAGE_WRITECOPY;
	hMap=CreateFileMapping(hnd,NULL,prot,0,0,NULL);
	if(hMap==NULL)R MAP_FAILED;

	acc=(__prot&PROT_WRITE)?FILE_MAP_WRITE:FILE_MAP_READ;
	if(__flags&MAP_PRIVATE) acc=FILE_MAP_COPY;
	addr=MapViewOfFileEx(hMap,acc,0,__off,__len,NULL);
							/* (__flags&MAP_FIXED)?__addr:NULL); */
	if (addr==NULL){
		CloseHandle(hMap);
		R MAP_FAILED;
	}
	/* close handle, it still remains open when view exists */
	CloseHandle(hMap);
	R addr;
}

I munmap(caddr_t __addr,size_t __len)
{
	BOOL r;

	r=UnmapViewOfFile(__addr);
	if(r==FALSE)errno=EINVAL;
	R TNX_BOOL(r);
}

I msync(caddr_t __addr,size_t __len,I __flags)
{
	I r=FlushViewOfFile(__addr,__len);
	if(!r)errno=EINVAL;
	R TNX_BOOL(r);
}

/*** TIME MANAGEMENT ***/
clock_t times(struct tms* t)
{
	struct _timeb tb;

	t->tms_utime = clock();
	t->tms_stime = t->tms_cutime = t->tms_cstime = 0;
	_ftime(&tb);

	R tb.time*(CLOCKS_PER_SEC) + tb.millitm*(CLOCKS_PER_SEC/1000);
}

I gettimeofday(struct timeval *tv,struct timezone *tz)
{
	struct _timeb tb;

	_ftime(&tb);
	if (tv) {
		tv->tv_sec = tb.time;
		tv->tv_usec = 1000 * tb.millitm;
	}
	if (tz) {
		tz->tz_minuteswest = tb.timezone;
		tz->tz_dsttime = tb.dstflag;
	}
	R 0;
}

/*** MATH ***/
#if 0
double rint(double x)
{
	if (_isnan(x) || !_finite(x)) 
	{
		raise(SIGFPE);
		return 0.0;
	}
	else
	{
		unsigned int s = _controlfp(0,0);
		return (s&_RC_UP) ? (double)ceil(x) : (double)floor(x);
	}
}
#endif

I iszero(F x)
{
	I c=_fpclass(x);
	R ((c==_FPCLASS_PZ)||(c==_FPCLASS_NZ));
}

I isnegative(F x)
{
	R (x<0.0);
}

#if 0
int isnan(double x)
{
	return _isnan(x);
}
#endif

I finite(F x)
{
	R _finite(x);
}

/*** SYSCONF ***/
long sysconf(I __name)
{
	switch(__name) {
	case _SC_CLK_TCK:	R CLOCKS_PER_SEC; break;
	case _SC_OPEN_MAX:	R FOPEN_MAX; break;
	case _SC_PAGESIZE:	R getpagesize(); break;
	default:			R -1;
	}
}

long getpagesize()
{
	SYSTEM_INFO	si;

	GetSystemInfo(&si);
	R si.dwPageSize;
}


/*** RANDOM NUMBER GENERATION ***/
extern void init_genrand(unsigned long);
extern long genrand_int31(void);

void srandom(unsigned long __seed)
{
	init_genrand(__seed);
}

long random()
{
	R genrand_int31();
}

void sleep(I msec)
{
  Sleep(msec);
}

Z HANDLE hIn=0;
Z I kbd_map[256];
Z I signaled=0;

I waitch()
{
	R signaled?signaled:signaled=(WaitForSingleObject(hIn,0)==WAIT_OBJECT_0);
}

void mapch(I from,I to)
{
	kbd_map[0xFF&from]=(0x70000&from)|(0xFF&to);
}

I getchex()
{
	Z I i=0,n=0;
	I c,m;
	INPUT_RECORD r[128];
	KEY_EVENT_RECORD *ke;
	DWORD cks;
	
	c=0;
	while(!c){
		if(i==n){
			waitch();ReadConsoleInput(hIn,&r,128,&n);i=0;signaled=0;
		}
		for(;i<n;i++){
			if(r[i].EventType!=KEY_EVENT)continue;
			ke=&r[i].Event.KeyEvent;
			if(!ke->bKeyDown)continue;
			cks=ke->dwControlKeyState;c=ke->uChar.AsciiChar;
			if(cks&ENHANCED_KEY)c=0xE000|ke->wVirtualKeyCode;
			if(!c&0xFF)continue;
			if(cks&CAPSLOCK_ON)c|=KEY_SHIFT;
			if(cks&SHIFT_PRESSED)c=(c&KEY_SHIFT)?c&~KEY_SHIFT:c|KEY_SHIFT;
			if(cks&(LEFT_ALT_PRESSED|RIGHT_ALT_PRESSED))c|=KEY_ALT;
			if(cks&(LEFT_CTRL_PRESSED|RIGHT_CTRL_PRESSED))c|=KEY_CTRL;
		}
	}
	if(0x0E000!=(0x0FF00&c)){
  	if((m=kbd_map[c&0xFF])&&((0x70000&m)==(0x70000&c)))c=0xFFFF&m;
	}
	R c;
}

Z I insovr=0;		/* INSERT/OVERWRITE */

Z void _bell()
{
	putchar('\a');
}

Z void _goleft(I n)
{
	I i;for(i=0;i<n;i++)putchar('\b');
}

/* j - current pos in b, n - amount */
Z void _goright(C*b,I j,I n)
{
	I i;for(i=0;i<n;i++)putchar(b[j+i]);
}

Z void _delch(C*b,I j,I n,I bks)
{
	I i;
	_goleft(bks);
	for(i=j;i<n;i++){
		if(i<n-1)putchar(b[i]=b[i+1]);
		else putchar(' ');
	}
	_goleft(n-j);
}

Z void _insch(C*b,I j,I n)
{
	I i;
	for(i=n;i>j;i--)b[i]=b[i-1];
	_goright(b,j+1,n-j);
	_goleft(n-j);
}

#define	KEY_END		0x0E023
#define KEY_HOME	0x0E024
#define	KEY_LEFT	0x0E025
#define	KEY_UP		0x0E026
#define	KEY_RIGHT	0x0E027
#define	KEY_DOWN	0x0E028
#define	KEY_DEL		0x0E02E
#define KEY_INS		0x0E02D
#define	KEY_CTRLZ	0x2001A

C *h[64];
I ch=0;
I nh=0;

Z I _hist(C*b,I j,I n,I d)
{
	I i,nn,c;

	c=63&(ch+d);if(h[c])ch=c;else R 0;
	_goleft(j);
	strcpy(b,h[ch]);b[nn=strlen(b)-1]='\0';
	printf("%s",b);
	if(n>nn){for(i=nn;i<n;i++)putchar(' ');_goleft(n-nn);}
	R 1;
}

Z void _session(C*b)
{
	FILE *fout;
	if(h[nh&=63])free(h[nh]);h[nh++]=strdup(b);
  fout=fopen("session.log","a+");
	fprintf(fout,"%s",b);
	fclose(fout);
}

C* cgetsex(C*b,I n)
{
	I i,c,p,eof=0;

	ch=nh;p=0;i=0;n--;c=' ';
	while((i<n)&&(c!='\n')){
		c=getchex();if(c=='\r')c='\n';
		switch(c){
		case KEY_UP:		if(_hist(b,p,i,-1))p=i=strlen(b);continue;
		case KEY_DOWN:	if(_hist(b,p,i,1))p=i=strlen(b);continue;
		case KEY_INS:		insovr=1-insovr;continue;
		case KEY_HOME:	_goleft(p);p=0;continue;
		case KEY_END:		_goright(b,p,i-p);p=i;continue;
		case KEY_LEFT:	if(p){_goleft(1);p--;}continue;
		case KEY_RIGHT:	if(p<i){_goright(b,p++,1);}continue;
		case KEY_DEL:		if(p<i){_delch(b,p,i--,0);}continue;
		case'\b':				if(p){_delch(b,--p,i--,1);}else _bell();continue;
		case'\n':				putchar('\r');putchar(c);break;
		default:
			if(c==KEY_CTRLZ)eof=1;
			c&=~(KEY_SHIFT);
			if(c>255)continue;/* skips control chars! */
			putchar(c);if(!insovr){_insch(b,p,i);if(p<i)i++;}
		}
		if(c=='\n')b[i++]=c;else{b[p++]=c;if(p>i)i++;}
	}
	b[i]='\0';
	if(eof)R 0;
	if(strlen(b))_session(b);
	R b;
}

/*** FAKED FUNCTIONS ***/
I aplusw32_fake(void)
{
	errno = EACCES;
	R -1;
}


I chown(const C*a0,uid_t a1,gid_t a2){R aplusw32_fake();}
I fchown(I a0,uid_t a1,gid_t a2){R aplusw32_fake();}
I fchmod(I a0,mode_t a1){R aplusw32_fake();}
gid_t getgid(){R aplusw32_fake();}
pid_t getppid(){R aplusw32_fake();}

/*** APLUS Win32 layer ***/
I aplusw32_init()
{
	WORD	wVersionRequested;
	WSADATA	wsaData;
	I i,mode;

	atexit(aplusw32_end);

	_fmode = O_BINARY;
	init_genrand((unsigned int)time(NULL));

	hIn=GetStdHandle(STD_INPUT_HANDLE);
#if 0
	GetConsoleMode(hIn,&mode);
	mode&=~(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT);
	SetConsoleMode(hIn,mode);
#endif
	for(i=0;i<256;i++)kbd_map[i]=0;
	for(i=0;i<64;i++)h[i]=0;
	apluskbd();

	wVersionRequested = MAKEWORD(2,2);
	R WSAStartup(wVersionRequested,&wsaData);
}

void aplusw32_end()
{
	WSACleanup();
}

