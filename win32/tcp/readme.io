
	Documentation for Low-Level I/O and TCP/IP I/O V1.5 for ThisForth.


	The low level I/O functions OPEN, OPEN-CREATE, CLOSE, READ, WRITE and
	LSEEK	are implemented in the file 'filehdl.m4'.  If TCP/IP sockets
	are also wanted, then the macro OPTIONS in the Makefile must
	include -DTCP, this will add the words SOPEN, LISTEN, and SOCKET-ACCEPT.
	The Forth interface to these is also in 'filehdl.m4', the C implementation
	is partially in the file 'sopen.c'.

	If you want to compile these into your version of Forth, then
	you need to add the following line to 'custom.m4',

ifdef(`FILEHDL',`include(`filehdl.m4')')


	And append -DFILEHDL and -DTCP (for TCP/IP) to the OPTIONS line in
	the Makefile, eg.  change,

OPTIONS = -DFLOAT

	to,

OPTIONS = -DFLOAT -DFILEHDL -DTCP

	If you want to use TCP/IP then append 'sopen.o' to CUSTOM, e.g.,

CUSTOM = sopen.o


	In the Makefile where, rth.m4 is built,

rth.m4:	fo rth save-4th
	fo rth $(ELECTIVES) $(EXTRA) save-4th
	rm -f hi.tmp


	Add 'fcntl.fo' to the list of files, or put it in the definition of
	EXTRA.  So for example one way to do it is,

	fo rth $(ELECTIVES) $(EXTRA) fcntl.fo save-4th



	Check that the values of the constants used in 'fcntl.fo' correspond
	to what is used on your system; change them as necessary.


	Then build Forth as usual.





	The glossary for the additional words.


	OPEN ( str_addr count flag_int -- handle )

	This word implements the system function open().
	Opens the named file according to the INTEGER value of the
	mode.  The flag value is usually defined in fcntl.h according
	to the following (the values given here are typical, but not
	necessarily universal).


     0		O_RDONLY            open for reading only

     1		O_WRONLY            open for writing only

     2		O_RDWR              open for reading and writing

     4		O_NDELAY            do not block on open

     010	O_APPEND            append on each write

     00400	O_CREAT             create file if it does not exist

     01000	O_TRUNC             truncate size to 0

     02000	O_EXCL              error if create and file exists


	These values are defined in the file 'fcntl.fo'.


	On return OPEN leaves the integer file descriptor on the
	stack.  This value is negative if the open failed.



	OPEN-CREATE ( str_addr count flag_int mode_int -- handle )

	Does the same as OPEN with the flag set to O_CREAT, but provides
	for an additional parameter to set the file permissions. Using
        O_CREAT as part of the flag value is harmlessly redundant.





	CLOSE ( handle -- status )

	This word implements the system function close().
	Closes a file referenced by the file descriptor.  The
	status flag is zero if successful, and -1 if it failed.



	WRITE ( addr nbytes handle -- status )

	This word implements the system function write().
	Writes to a file referenced by the file descriptor, 'handle'.
	The start of the data to write is at 'addr' and there are
	'nbytes' bytes to write.  On return 'status' is the number
	of bytes successfuly written, or a -1 on failure.


	READ ( addr nbytes handle -- status )

	This word implements the system function read().
	Reads from a file referenced by the file descriptor, 'handle'.
	The start of the data to destination is at 'addr' and there are
	'nbytes' bytes to read.  On return 'status' is the number
	of bytes successfuly read.  If the status value is zero, then
	end-of-file has been reached.  If the status is negative, then
	an error occurred.



        LSEEK ( offset whence handle -- status )

	This word implements the system function lseek().
	Sets the file pointer to 'offset' bytes according to the value
	of 'whence' according to:


          If whence is L_SET, the pointer is set to offset bytes.

          If whence is L_INCR, the pointer is set to its current
          location plus offset.

          If whence is L_XTND, the pointer is set to the size of
          the file plus offset.

	The typical (but not universal) values of the whence constants
	are,

        0   L_SET             ( also known as   SEEK_SET )
        1   L_INCR            ( also known as   SEEK_CUR )
        2   L_XTND            ( also known as   SEEK_END )


	These values are also defined in 'fnctl.fo'


	LSEEK returns the pointer location in bytes from the beginning
	of the file if it succeeds.  On a failure the status value is -1
	(in which case the file pointer is unchanged).  NOTE: offset and
	status are 'off_t' data types, it is presumed that this data type
	will fit into a single cell.


	The following words are part of the TCP/IP interface

	

	SOPEN ( str_addr count socket type -- handle )

	This word encapsulates calls to several system functions in
	order to set up a TCP/IP connection as either a client or
	a server.  It was written so that opening a TCP/IP socket is
	just about as simple as opening a file.


	As a client 'str_addr' and 'count' represent the counted
	string that has the name of the server that the client wishes
	to contact.  'socket' is the TCP/IP socket that the server
	is using.  For clients, 'type' is just the number 0.

	As a server 'str_addr' and 'count' are 0 values.  The value
	of 'socket' is the TCP/IP socket number that this server will
	use to service requests.  For servers, 'type' is the number 1.


	In either case, the returned value on the stack is -1 if an
	error occurred.  If the stack value is > 0, then it is used
	exactly like a file handle for READ, WRITE and CLOSE.




	LISTEN ( backlog socket -- status )

	The server process needs to let the operating system know that
	it is willing to accept connections to its socket, and the
	queue limit for incoming connections.  The word LISTEN does
	this.  The value of 'socket' is the value returned by a
	previous successful call to SOPEN.  The value of 'backlog'
	is the queue limit (a value of 5 is usually used, other values
	may not be supported by your system).  On return a 0 status
	indicates success, a -1 indicates an error.  CLIENT PROCESSES
	DO NOT USE THIS WORD.



	SOCKET-ACCEPT ( sockaddr len socket -- nsock )

        For servers, this where the process actually waits until
        a connection is present.  SOCKET-ACCEPT is called after
        LISTEN.  The value of 'socket' is the value returned by a
        previous successful call to SOPEN.  'sockaddr' is the address
        of the data space for information on the connecting entity, it
        is filled in by SOCKET-ADDR.  The user generally won't need to
        be concerned with its internals.  CHECK WITH YOUR SYSTEM MANUALS
        ON THE SIZE OF THIS REGION, typically it is 16 bytes, the size of
        the region is the parameter 'len'.  On return the value 'nsock' is
        negative if SOCKET-ACCEPT failed, if it is positive, then this value
        is used as a file descriptor for use in READ, WRITE and CLOSE for
        this instance of the connection.   CLIENT PROCESSES DO NOT USE THIS WORD.




 Everett (Skip) Carter        Phone:  408-656-3318 FAX: 408-656-2712
 Naval Postgraduate School    INTERNET: skip@taygeta.oc.nps.navy.mil
 Dept. of Oceanography, Code OC/CR  UUCP:     ...!uunet!taygeta!skip
 Monterey, CA. 93943
                  WWW: http://taygeta.oc.nps.navy.mil/skips_home.html


