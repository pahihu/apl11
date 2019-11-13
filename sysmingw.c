#define	WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <winsock2.h>

#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <fcntl.h>
#include <float.h>
#include <io.h>
#include <math.h>
#include <process.h>
#include <time.h>

#ifndef _MSC_VER
#define WIN_CONST CONST
#define WIN_IN	  IN
#undef 	CONST
#undef	IN
#endif

#include "apl.h"

extern printf11(char*,...);
void error(char*);
extern void srandom(unsigned long);
extern void ttyanswer();

typedef int I;
typedef char C;
typedef void Z;
#define	R	return

static I _fds[NFDS];

I fork(I arg){R -1;}
I waitpid(I p,I*s,I o){R _cwait(s,p,0);}
I pipe(I*pp){R _pipe(pp,256,O_BINARY);}
I kill(I pid,I sig){R -1;}
I nice(I arg){R(SetPriorityClass(GetCurrentProcess(),arg))?0:-1;}
I sleep(I arg){R Sleep(1000*arg),0;}

struct t {
	long proc_user_time;
	long proc_system_time;
	long child_user_time;
	long child_system_time;
};

I times(struct t *arg)
{
	clock_t t = clock();
	arg->proc_user_time=(long)(60.0*t/CLOCKS_PER_SEC);
	arg->proc_system_time=arg->child_user_time=arg->child_system_time=0L;
	R 0;
}
I spawn(const C *cmd,const C **argv){R _spawnv(P_NOWAIT,cmd,argv);}

void einval_handler(
   const wchar_t * expression,
   const wchar_t * function, 
   const wchar_t * file, 
   unsigned int line,
   uintptr_t pReserved
)
{
	return;
}

Z doneWin32(void){WSACleanup();}
I initWin32()
{
	I i;
	WSADATA d;
	WORD v = MAKEWORD(2,0);

#ifdef _MSC_VER
	_set_invalid_parameter_handler(einval_handler);
#endif
	srandom((unsigned int)clock());
	if(WSAStartup(v,&d)){fprintf(stderr,"WinSock init error\n");R 1;}
	atexit(doneWin32);
	_fpreset();
	for (i=0;i<NFDS;i++) _fds[i]=(i<3)?1:0;
	ttyanswer();
	R 0;
}

I os_close(I fd)
{
	I r;
	long hnd;
	if(fd<0){if((r=closesocket(-fd))==SOCKET_ERROR)R errno=WSAGetLastError();}
	else {
		if(0==_fds[fd])R 0;
		hnd=_get_osfhandle(fd);
		if(-1==hnd)R 0;
		r=_close(fd);
		if(fd<NFDS)_fds[fd]=0;
	}
	R r;
}

I os_read(I fd,C *b,I n)
{
	I r;
	if(fd<0){if((r=recv(-fd,b,n,0))==SOCKET_ERROR)R errno=WSAGetLastError();}
	else r=_read(fd,b,n);
	R r;
}

I os_write(I fd,C *b,I n)
{
	I r;
	if(fd<0){if((r=send(-fd,b,n,0))==SOCKET_ERROR)R errno=WSAGetLastError();}
	else r=_write(fd,b,n);
	R r;
}

I os_open(C*f,I m,I pm)
{
	I fd;
	fd=_open(f,m|O_BINARY,pm);
	if(fd<NFDS)_fds[fd]=1;
	R fd;
}

I os_creat(C*f,I m)
{
	I fd;
	fd = os_open(f,O_WRONLY|O_CREAT|O_TRUNC,m);
	if(fd<NFDS)_fds[fd]=1;
	R fd;
}

#ifdef _MSC_VER
I strncasecmp(C*s,C*d,I n){R _strnicmp(s,d,n);}
#endif

I _matherr(struct _exception *exc)
{
	static C* exclist[] = {
		"unknown fp error",
		"fp argument domain error",
		"fp argument singularity",
		"fp overflow range error",
		"fp partial loss of significance",
		"fp total loss of significance",
		"fp underflow"
	};
	I x;

	switch (exc->type) {
	case _DOMAIN:	x = 1; break;
	case _SING:		x = 2; break;
	case _OVERFLOW: x = 3; break;
	case _PLOSS:	x = 4; break;
	case _TLOSS:	x = 5; break;
	case _UNDERFLOW: x = 6; break;
	default:
		x = 0;
	}
	printf11("[matherr] (%d, %s)\n", exc->type, exc->name);
	R error(exclist[x]),0;
}

/*
#include <stdarg.h>
I oprintf(C*fmt,...)
{
	I r;
	va_list pvar;

	va_start(pvar,fmt);
	r = vprintf(fmt, pvar);
	va_end(pvar);
	R r;
}
*/
