/* sopen.h         prototypes for TCP and UDP socket open calls

           This code is partly based upon code in:


           W. Richard Stevens, 1990; Unix Network Programming,
           Prentice-Hall, Englewood Cliffs, N.J., 772 pages


rcsid: @(#)sopen.h	1.1 00:36:18 2/8/94   EFC

*/

#ifndef SOPEN_H_
#define SOPEN_H_ 1.1

#ifdef WIN32
#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif


extern struct sockaddr_in       udp_srv_addr;
extern struct sockaddr_in       udp_cli_addr;

#ifdef __cplusplus
extern "C" {
#endif

int tcp_open(char* hostname, int port, char* service, int kind);
int udp_open(char *hostname, int port, char* service, int dontconn);
int resolve_name(char* hname, struct hostent **ph, int silent);

#ifdef __cplusplus
}
#endif

#endif
