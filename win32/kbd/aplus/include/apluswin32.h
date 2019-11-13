#if !defined(__APLUSWIN32_H)
#define __APLUSWIN32_H

#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <io.h>
#include <float.h>
#include <process.h>
#include <sys/param.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <ieeefp.h>

#define  WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iphlpapi.h>
#include <winsock2.h>

#ifdef __cplusplus
extern "C" {
#endif

int aplusw32_init();
void aplusw32_end();

#if defined(__BUILD_APLUSWIN32__)
struct timezone {
	int	tz_minuteswest;
	int tz_dsttime;
};
int gettimeofday(struct timeval *tv,struct timezone *tz);
#endif

typedef char*		  	caddr_t;

#define LOG_NOTICE	5				/* syslog.h */
#define	LOG_INFO	6

#define LOG_PID		0x01
#define LOG_LOCAL1	(17<<3)

void openlog(char*,int,int);
void syslog(int,const char*,...);
void closelog(void);

#define MAXNAMLEN	MAX_PATH
#define MAX_LONG	LONG_MAX

typedef void (*_sig_func_ptr)(int);	/* sys/signal.h */

struct sigaction {
	_sig_func_ptr sa_handler;
	sigset_t sa_mask;
	int sa_flags;
};

#define SA_RESTART			0x10000000
#define	sigaddset(what,sig)	(*(what) |= (1<<(sig)))
#define sigemptyset(what)	(*(what) = 0)
int sigaction(int s,const struct sigaction *nh,struct sigaction *oh);

#define S_ISVTX			0

#define PROT_READ		1			/* sys/mman.h */
#define PROT_WRITE		2

#define MAP_SHARED		0x01
#define MAP_PRIVATE		0x02
#define MAP_FIXED		0x10
#define MAP_FAILED		((caddr_t)-1)

caddr_t mmap(caddr_t __addr,size_t __len,int __prot,int __flags,int __fd, off_t __off);
int munmap(caddr_t __addr,size_t __len);

#define	MS_ASYNC			1
#define	MS_SYNC				2
#define	MS_INVALIDATE	4

int msync(caddr_t __addr,size_t __len,int __flags);

#define	__P(protos)		protos		/* sys/cdefs.h */

#define RLIMIT_CORE		4
#define RLIM_INFINITY	(0xffffffffUL)

struct tms {						/* sys/times.h */
	clock_t tms_utime;
	clock_t tms_stime;
	clock_t tms_cutime;
	clock_t tms_cstime;
};

clock_t times(struct tms*);

int fsync(int);						/* unistd.h */
int ftruncate(int,off_t);
int truncate(const char*,off_t);
int lstat(const char*,struct stat*);

#define MAXHOSTNAMELEN	MAX_HOSTNAME_LEN

#ifndef EINPROGRESS
#define	EINPROGRESS	WSAEINPROGRESS
#endif
#ifndef EISCONN
#define	EISCONN		WSAEISCONN
#endif
#ifndef EWOULDBLOCK
#define	EWOULDBLOCK	WSAEWOULDBLOCK
#endif

/* double rint(double); */
int	iszero(double);
int isnegative(double);
/* int isnan(double); */

#define	_SC_CLK_TCK		2		/* sys/unistd.h */
#define _SC_OPEN_MAX	4
#define _SC_PAGESIZE	8
long sysconf(int __name);
long getpagesize();

#define	FP_SNAN		_FPCLASS_SNAN	/* float.h */
#define	FP_QNAN		_FPCLASS_QNAN
#define	FP_NINF		_FPCLASS_NINF
#define	FP_PINF		_FPCLASS_PINF

/* fake function definitions */
int aplusw32_fake(void);

#define	getuid		aplusw32_fake
#define	geteuid		aplusw32_fake
#define	link			aplusw32_fake
#define	symlink		aplusw32_fake

#ifndef uid_t
#define	uid_t	int
#endif
#ifndef gid_t
#define	gid_t	int
#endif

int chown(const char*,uid_t,gid_t);
int fchown(int,uid_t,gid_t);
int fchmod(int,mode_t);
gid_t getgid();
pid_t getppid();

void srandom(unsigned long);		/* random numbers */
long random();

char* readline(char*,int);			/* console I/O */

#define	LOCK_SH	1					/* file locking */
#define	LOCK_EX	2
#define LOCK_NB 4
#define LOCK_UN	8

int flock(int,int);

int kill(int pid,int sig);

/* console */
int waitch();
void mapch(int from,int to);
int getchex();
char* cgetsex(char*b,int n);
void apluskbd();

#define	KEY_ALT			0x010000
#define	KEY_CTRL		0x020000
#define	KEY_SHIFT		0x040000

#ifdef __cplusplus
}
#endif

#endif /* __APLUSWIN32_H */
