/* filehdl.m4       Code for open, close, write, read  which access the
                    low level system I/O

	This file also implements interfaces to the following for TCP/IP
	I/O, if the token TCP_IP is defined


		LISTEN, ACCEPT (defined as SOCKET-ACCEPT to avoid
		a confict with Forth's ACCEPT) and my TCP/IP socket
		open routine SOPEN


                      (c) Copyright 1994, Everett F. Carter Jr.
                      Permission is granted by the author to use
		      this software for any application provided this
		      copyright notice is preserved.


*/

static char rcsid1[] = "@(#)filehdl.m4	1.6 12:14:52 3/2/95   EFC";




#include <stdio.h>
#include <ctype.h>
#include <errno.h>

ifdef(`TCP',`
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
')

/* Usage:	s" fname" int_mode open, returns fh on success -1 on failure */
Execution(`OPEN')
{	/* Standard C Library */
	int flags;

	/* Make NUL-terminated string at &data[pocket+1] from S[-1],S */
	move(&data[pocket + 1], &data[S[-1]], *S),
		data[pocket + *S + 1] = EOS,
			data[pocket] = *S;

	flags = top;

	S -= 2;
	top = (cell) open((char *)&data[pocket + 1], flags);
}
Done

Execution(`OPEN-CREATE')
{	/* Standard C Library */
	int flags, mode;

	/* Make NUL-terminated string at &data[pocket+1] from S[-2],S[-1] */
	move(&data[pocket + 1], &data[S[-2]], S[-1]),
		data[pocket + S[-1] +1 ] = EOS,
			data[pocket] = S[-1];

	mode = top;
	flags = *S | O_CREAT;

	S -= 3;
	top = (cell) open((char *)&data[pocket + 1], flags, mode);
}
Done


Execution(`CLOSE')
	top = (cell) close( top );
Done

Execution(`WRITE')
{
	int size, fh;
	char* ptr;

	fh = top;
	size = *S;
	ptr = (char *)&data[ S[-1] ];

	S -= 2;
	top = (cell)write( fh, ptr, size );
}
Done

Execution(`READ')
{
	int size, fh;
	char* ptr;

	fh = top;
	size = *S;
	ptr = (char *)&data[ S[-1] ];

	S -= 2;
	top = (cell)read( fh, ptr, size );
}
Done

#ifdef __SC__
typedef long off_t;
#endif

Execution(`LSEEK')
{
        off_t offset;
        int   fh, whence;
        
        fh = top;
	whence = *S;
	offset = (off_t) S[-1];        /* NOTE: ASSUMES off_t FITS IN ONE CELL!! */

	S -= 2;
	top = (off_t)lseek( fh, offset, whence );

}
Done

ifdef(`TCP',`

#include "sopen.h"

Execution(`SOPEN')
{
	int kind, port;

	/* Make NUL-terminated string at &data[pocket+1] from S[-2],S[-1] */
	if ( S[-1] > 0 )
		move(&data[pocket + 1], &data[S[-2]], S[-1]),
			data[pocket + S[-1] + 1] = EOS,
			data[pocket] = S[-1];
	else
		data[pocket] = 0;

	port = *S;
	kind = top;

	S -= 3;

	/*
	fprintf(stderr,"host: <%s>, port = %d, kind = %d\n", &data[pocket + 1],
							    port, kind);
	*/

	if ( data[pocket] > 0 )
	top = (cell)tcp_open( (char *)&data[pocket + 1], port, (char *)NULL, kind );
	else
	top = (cell)tcp_open( (char *)NULL, port, (char *)NULL, kind );
}
Done


Execution(`LISTEN')
{
	int backlog, fh;

	backlog = *S;
	fh   = top;

	S -= 1;
	top = (cell)listen( fh, backlog );
}
Done

Execution(`SOCKET-ACCEPT')
{
	int fh, alen;
	struct sockaddr *addr;

	fh = top;
	alen = *S;
	addr = (struct sockaddr *)&data[ S[-1] ];

	S -= 2;
	top = (cell)accept( fh, addr, &alen );
	/* *S = alen; */
}
Done


')





