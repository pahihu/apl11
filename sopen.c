/* sopen.c	         tcp_open(), udp_open() and resolve_name()

	does all the dirty work in opening a socket on a (possibly) remote
	system.

	tcp_open()           For TCP connections

	three input parameters:
		hostname	-- the name (or inet addr) of the remote end of the socket
		port		  -- the remote port socket number
                   if >= 0, use port# of service
                   if < 0,  bind a local reserved port
                   if > 0,  this is the port# (host byte order) of server
		service   -- the service to connect to, can be NULL if port > 0
		kind		  -- = 0 for clients, = 1 for servers
                     clients call connect() servers call bind()

	returns:
		< 0		if an error
		fd ( > 0)	the file discriptor for a successful open



  udp_open()             For UDP connections

         host          name of other system to communicate with
	       port          if >= 0, use port# of service
                       if > 0,  this is the port# (host byte order) of server
         service       name of service being requested, can be NULL iff port > 0
	       dontconn      if == 0, call connect(), else don't


	returns:
		< 0		if an error
		fd ( > 0)	the file discriptor for a successful open


           This code is partly based upon code in:


           W. Richard Stevens, 1990; Unix Network Programming,
           Prentice-Hall, Englewood Cliffs, N.J., 772 pages

*/

static char rcsid[] = "@(#)sopen.c	1.6 17:17:29 2/15/93   EFC";

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket	close
#endif

#include "sopen.h"

#ifndef bzero
#define	bzero(d,n)		memset(d,0,n)
#endif

#ifndef	bcopy
#define	bcopy(s,d,n)	memcpy(d,s,n)
#endif


#define CLIENT		0
#define SERVER		1

/* define SILENT to be either 0 or 1 */

#define SILENT          0

struct sockaddr_in sinhim = { AF_INET };

struct sockaddr_in       udp_srv_addr;
struct sockaddr_in       udp_cli_addr;

int tcp_open(char* hostname, int port,char* service, int kind)
{
	struct hostent *hp;
	struct servent *sp;
	int resvport;
	int fd = 0;

	bzero( (char *)&sinhim, sizeof(sinhim));
	sinhim.sin_family = AF_INET;

	if ( service != NULL ) {
		if ( (sp = getservbyname( service, "tcp" )) == NULL ) {
#if  (SILENT == 0)
			perror( "tcp_open: unknown service");
#endif
			return -1;
		}

		if ( port > 0 )
			sinhim.sin_port = htons( port );  /* callers value */
		else
			sinhim.sin_port = sp->s_port;     /* services value */
	} else {
		if ( port <= 0 ) {
#if (SILENT == 0)
			perror( "tcp_open: must specify either service or port" );
#endif
			return -2;
		}
		sinhim.sin_port = htons( port );
	}

	if (hostname != NULL) {
		if ( resolve_name( hostname, &hp, SILENT ) )
			return -3;

		bcopy ( hp->h_addr, &sinhim.sin_addr, sizeof (sinhim.sin_addr) );
		/* some systems need to do it this way, not sure what ones though */
		/* bcopy (&(hp->h_addr), &sinhim.sin_addr,sizeof( sinhim.sin_addr) ); */
	}

	if (port >= 0 ) {
    if ( (fd = socket( AF_INET, SOCK_STREAM, 0)) < 0) {
#if (SILENT == 0)
			perror("tcp_open: socket");
#endif
			return -4;
    }
	} else if ( port < 0 ) {
		resvport = IPPORT_RESERVED - 1;
		if ( (fd = rresvport(&resvport)) < 0 ) {
#if (SILENT == 0)
			perror("tcp_open: can't get a reserved TCP port" );
#endif
			return -7;
		}
	}

	if (kind == CLIENT) {
		if (connect(fd, &sinhim, sizeof(sinhim)) < 0) {
#if (SILENT == 0)
			perror("tcp_open: connect");
#endif
			return -5;
		}
	} else 	/* assume kind == SERVER */ {
		if ( bind(fd, &sinhim, sizeof(sinhim) ) < 0) {
#if (SILENT == 0 )
			perror("tcp_open: bind");
#endif
			return -6;
		}
	}

	return fd;
}


int udp_open(char *hostname, int port, char* service, int dontconn)
{
	int fd;
	unsigned long inaddr;
	struct servent *sp;
	struct hostent *hp;

	bzero( (char *)&udp_srv_addr, sizeof(udp_srv_addr));
	udp_srv_addr.sin_family = AF_INET;


        if ( service != NULL )
        {
             if ( (sp = getservbyname( service, "udp" )) == NULL )
             {
#if  (SILENT == 0)
                        perror( "udp_open: unknown service");
#endif
                        return -1;
             }


             if ( port > 0 )
                    udp_srv_addr.sin_port = htons( port );  /* callers value */
             else
                    udp_srv_addr.sin_port = sp->s_port;     /* services value */

        }
        else
        {
             if ( port <= 0 )
             {
#if (SILENT == 0)
                   perror( "udp_open: must specify either service or port" );
#endif
                   return -2;
             }
             udp_srv_addr.sin_port = htons( port );
        }


	if (hostname != NULL)
	{
		if ( resolve_name( hostname, &hp, SILENT ) )
			return -3;

	     bcopy ( hp->h_addr, &udp_srv_addr.sin_addr, sizeof (udp_srv_addr.sin_addr) );

             /* some systems need to do it this way, not sure what ones though */
     /*
	     bcopy (&(hp->h_addr), &udp_srv_addr.sin_addr,sizeof( udp_srv_addr.sin_addr) );
     */

	}



       if ( port < 0 )
       {
#if (SILENT == 0)
	 perror( "udp_open: reserved ports not implemented yet." );
#endif
	 return -7;
        }


        if ( (fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
#if (SILENT == 0)
                 perror("udp_open: can't create UDP socket" );
#endif
                 return -4;
        }


       if ( hostname != NULL )     /* presumably this means that this is a client */
       {
	 /* bind any local address */
	 bzero( (char *)&udp_cli_addr, sizeof(udp_cli_addr));
	 udp_cli_addr.sin_family = AF_INET;
	 udp_cli_addr.sin_addr.s_addr = htonl( INADDR_ANY );
	 udp_cli_addr.sin_port = htons( 0 );



       if ( bind( fd, (struct sockaddr *)&udp_cli_addr, sizeof(udp_cli_addr)) < 0)
       {
	          closesocket( fd );
#if (SILENT == 0)
	          perror( "udp_open: bind error" );
#endif
		  return -6;
       }

       }
       else                /* this must be a server */
       {

	 /* bind server address */

       if ( bind( fd, (struct sockaddr *)&udp_srv_addr, sizeof(udp_srv_addr)) < 0)
       {
	          closesocket( fd );
#if (SILENT == 0)
	          perror( "udp_open: bind error" );
#endif
		  return -8;
       }


       }


       /* This is usually used by most callers since the peer
          usually won't change.  By calling connect() the caller can use send()
          and recv(), otherwise sendto() and recvfrom() are necessary.
       */

       if ( dontconn == 0 )
       {
	   if ( connect(fd, &udp_srv_addr, sizeof( udp_srv_addr )) < 0 )
	   {
#if (SILENT == 0 )
	                perror( "udp_open: connect error" );
#endif
			return -5;
	    }

       }

       return( fd );


}




int resolve_name(char* hname, struct hostent **ph, int silent)
{
    long addr;

        if (isdigit(hname[0]))
        {
            addr = inet_addr(hname);
            *ph = gethostbyaddr(&addr, 4, AF_INET);
        }
        else
            *ph = gethostbyname(hname);

        if (*ph == (struct hostent *)NULL)
        {

	  if ( silent == 0 )
	    {
#ifdef BSD42
            fprintf(stderr,"host %s not found\n", hname);
#else
            switch (h_errno) {
                case HOST_NOT_FOUND: fprintf(stderr,"host %s not found\n", hname);
                                     break;
                case TRY_AGAIN: fprintf(stderr,"Try again later\n");
                                break;
                case NO_RECOVERY: fprintf(stderr,"No recovery possible\n");
                                  break;
                case NO_ADDRESS: fprintf(stderr,"No IP address\n");
                                 break;
                default: fprintf(stderr,"Unknown error: %d\n", h_errno);
                         break;
            }
#endif
	  }

            return 1;
        }

        (*ph)->h_addr[ (*ph)->h_length ] = NULL;

        return 0;
}





