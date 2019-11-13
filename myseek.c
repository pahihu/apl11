#include <stdio.h>
#include <unistd.h>

off_t myseek(char *fnm, int lno, int fd, off_t off, int whence)
{
	off_t ret;

	if (off > 1000) {
		fprintf(stderr,"BEFORE %s:%d: %d %ld %d\n",fnm,lno,fd,off,whence);
	}
	ret = lseek(fd, off, whence);
	if (off > 1000) {
		fprintf(stderr,"AFTER %s:%d: %d %ld %d\n",fnm,lno,fd,off,whence);
	}
	return ret;
}
