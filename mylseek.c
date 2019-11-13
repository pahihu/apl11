#include <stdio.h>
#include <unistd.h>

off_t mylseek(char *fnm, int lno, int fd, off_t off, int whence)
{
	off_t r;
#ifndef NDEBUG
	if (off > 10000)
		fprintf(stderr,"BEFORE %s:%d: %d %lld %d\n",fnm,lno,fd,off,whence);
#endif
	r = lseek(fd, off, whence);
#ifndef NDEBUG
	if (r > 10000 || r < 0)
		fprintf(stderr,"AFTER %s:%d: %d %lld %d\n",fnm,lno,fd,off,whence);
#endif
	return r;
}
