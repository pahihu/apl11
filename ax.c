/*
 *	Floating-point initialization and trap service
 *
 *	On the PDP-11's the code in "ax.pdp.s" is preferred
 *	since it enables the FPP for all floating exception
 *	traps and fishes out the cause of an exception from
 *	the FPP error register.
 */

#include <stdio.h>
#include <signal.h>
#ifdef WIN32
#include <float.h>
#endif

char *fpelist[] = {
#ifdef WIN32
	"floating exception",
	"invalid floating",
	"denormalized float",
	"floating divide by zero",
	"floating overflow",
	"floating underflow",
	"inexact float",
	"unemulated function",
	"floating exception",
	"floating stack overflow",
	"floating stack underflow"
#else
	"floating exception",
	"integer overflow",
	"integer divide by zero",
	"floating overflow",
	"floating divide by zero",
	"floating underflow",
	"decimal overflow",
	"subscript range",
	"floating overflow",
	"floating divide by zero",
	"floating underflow"
#endif
};

void
fpe(signo, param)
unsigned param;
{
#ifdef WIN32
	_fpreset();
	param -= 128;
#else
	signal(SIGFPE, fpe);
#endif
	printf("[fpe] (%d, %d)\n", signo, param);
	if (param >= sizeof fpelist/sizeof fpelist[0]) {
	  /* error("floating exception");*/
	  error("unknown floating exception");
	}

	else 
	  error(fpelist[param]);
}

void
fppinit()
{
	signal(SIGFPE, fpe);
}
