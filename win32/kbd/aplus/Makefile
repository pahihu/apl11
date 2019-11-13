CC	= gcc -mno-cygwin
CFLAGS	= -O6 -s -I. -Iinclude -D_WIN32=1 -D__BUILD_APLUSWIN32__
SRC	= apluswin32.c dlfcn.c mt19937ar-cok.c apluskbd.c #win32con.c
OBJ	= $(SRC:.c=.o)

all: libapluswin32.a tt.exe

libapluswin32.a: $(OBJ)
	$(AR) cru $@ $(OBJ)

tt.exe: t.o libapluswin32.a
	$(CC) -g -o $@ $< -L. -lapluswin32 -lws2_32 -liphlpapi

t.c: include/apluswin32.h

apluswin32.c: include/apluswin32.h

clean:
	$(RM) -f libapluswin32.a
	$(RM) -f $(OBJ)
	$(RM) -f t.o tt.exe
