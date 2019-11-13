#include <io.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	unsigned char buf1[256], buf2[256];
	int i, fd;
	extern int _fmode;

	for (i=0; i<256; i++) buf1[i]=i;
	fd = open("test.wks",O_WRONLY|O_CREAT|O_TRUNC|O_BINARY,0600);
	write(fd, buf1, 256);
	close(fd);

	fd = open("test.wks", O_RDONLY|O_BINARY);
	read(fd, buf2, 256);
	close(fd);

	if(memcmp(buf1, buf2, 256)!=0) {
		fprintf(stderr, "not equal!\n");
	} else fprintf(stderr, "equals!\n");
}
