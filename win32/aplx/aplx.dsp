# Microsoft Developer Studio Project File - Name="aplx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=aplx - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "aplx.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "aplx.mak" CFG="aplx - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "aplx - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "aplx - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "aplx - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /GB /W3 /GX /Ox /Ot /Og /Oi /Op /Oy /I ".." /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "aplx - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "aplx - Win32 Release"
# Name "aplx - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\a0.c
# End Source File
# Begin Source File

SOURCE=..\a1.c
# End Source File
# Begin Source File

SOURCE=..\a2.c
# End Source File
# Begin Source File

SOURCE=..\a3.c
# End Source File
# Begin Source File

SOURCE=..\a4.c
# End Source File
# Begin Source File

SOURCE=..\a5.c
# End Source File
# Begin Source File

SOURCE=..\a6.c
# End Source File
# Begin Source File

SOURCE=..\a7.c
# End Source File
# Begin Source File

SOURCE=..\a8.c
# End Source File
# Begin Source File

SOURCE=..\a9.c
# End Source File
# Begin Source File

SOURCE=..\aa.c
# End Source File
# Begin Source File

SOURCE=..\ab.c
# End Source File
# Begin Source File

SOURCE=..\ac.c
# End Source File
# Begin Source File

SOURCE=..\ad.c
# End Source File
# Begin Source File

SOURCE=..\ae.c
# End Source File
# Begin Source File

SOURCE=..\af.c
# End Source File
# Begin Source File

SOURCE=..\ag.c
# End Source File
# Begin Source File

SOURCE=..\ah.c
# End Source File
# Begin Source File

SOURCE=..\ai.c
# End Source File
# Begin Source File

SOURCE=..\aj.c
# End Source File
# Begin Source File

SOURCE=..\ak.c
# End Source File
# Begin Source File

SOURCE=..\al.c
# End Source File
# Begin Source File

SOURCE=..\am.c
# End Source File
# Begin Source File

SOURCE=..\ao.c
# End Source File
# Begin Source File

SOURCE=..\aq.c
# End Source File
# Begin Source File

SOURCE=..\at.c
# End Source File
# Begin Source File

SOURCE=..\aw.c
# End Source File
# Begin Source File

SOURCE=..\ax.c
# End Source File
# Begin Source File

SOURCE=..\az.c
# End Source File
# Begin Source File

SOURCE=..\dirent.c
# End Source File
# Begin Source File

SOURCE=..\gamma.c
# End Source File
# Begin Source File

SOURCE=..\rresvport.c
# End Source File
# Begin Source File

SOURCE=..\sopen.c
# End Source File
# Begin Source File

SOURCE=..\sysmingw.c
# End Source File
# Begin Source File

SOURCE=..\y.tab.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\apl.h
# End Source File
# Begin Source File

SOURCE=..\dirent.h
# End Source File
# Begin Source File

SOURCE=..\sopen.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
